/*
 * mmos/src/kernel/memory/paging/table_manager/pt_manager.c
 * struct and functions for managing page tables of processes
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#include <stdint.h>
#include "pt_manager.h"
#include "../map_indexer/map_indexer.h"
#include "../page_frame_allocator/allocator.h"
#include "../../memory.h"

PAGE_TABLE_MANAGER* global_pt_manager;

void ptmanager_init(PAGE_TABLE_MANAGER* manager, PAGE_TABLE* pml4_address) {
    manager->pml4 = pml4_address;
}

void ptmanager_map_memory(PAGE_TABLE_MANAGER* manager, void* virtual_address, void* physical_address) {
    PAGE_MAP_INDEXER indexer;
    map_index(&indexer, (uint64_t)virtual_address);

    PAGE_DIRECTORY_ENTRY pml4e = manager->pml4->entries[indexer.pdp_index];
    PAGE_TABLE* pdp;
    if (!(pml4e & PAGE_PRESENT)) {
        pdp = (PAGE_TABLE*)pfallocator_request_page(global_allocator);
        memset(pdp, 0, 4096);
        manager->pml4->entries[indexer.pdp_index] =
            ((uint64_t)pdp & PAGE_ADDR_MASK) | PAGE_PRESENT | PAGE_RW;
    } else {
        pdp = (PAGE_TABLE*)((uint64_t)(pml4e & PAGE_ADDR_MASK));
    }

    PAGE_DIRECTORY_ENTRY pdpe = pdp->entries[indexer.pd_index];
    PAGE_TABLE* pd;
    if (!(pdpe & PAGE_PRESENT)) {
        pd = (PAGE_TABLE*)pfallocator_request_page(global_allocator);
        memset(pd, 0, 4096);
        pdp->entries[indexer.pd_index] =
            ((uint64_t)pd & PAGE_ADDR_MASK) | PAGE_PRESENT | PAGE_RW;
    } else {
        pd = (PAGE_TABLE*)((uint64_t)(pdpe & PAGE_ADDR_MASK));
    }

    PAGE_DIRECTORY_ENTRY pde = pd->entries[indexer.pt_index];
    PAGE_TABLE* pt;
    if (!(pde & PAGE_PRESENT)) {
        pt = (PAGE_TABLE*)pfallocator_request_page(global_allocator);
        memset(pt, 0, 4096);
        pd->entries[indexer.pt_index] =
            ((uint64_t)pt & PAGE_ADDR_MASK) | PAGE_PRESENT | PAGE_RW;
    } else {
        pt = (PAGE_TABLE*)((uint64_t)(pde & PAGE_ADDR_MASK));
    }

    pt->entries[indexer.p_index] =
        ((uint64_t)physical_address & PAGE_ADDR_MASK) | PAGE_PRESENT | PAGE_RW;
}
