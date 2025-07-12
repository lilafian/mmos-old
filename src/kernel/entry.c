/*
 * mmos/src/kernel/entry.c
 * mmkernel entry point and initialization
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#include <stdint.h>
#include "commondef.h"
#include "graphics/gop_framebuffer/framebuffer.h"
#include "graphics/fonts/psf/psf.h"
#include "console/basic_output_console/basic_output_console.h"
#include "misc/klog/klog.h"
#include "memory/memory.h"
#include "memory/bitmap.h"
#include "memory/paging/page_frame_allocator/allocator.h"
#include "memory/paging/paging.h"
#include "memory/paging/table_manager/pt_manager.h"

void halt() {
    while(1) {
        asm("hlt");
    }
}

extern uint64_t _MMK_START_ADDRESS;
extern uint64_t _MMK_END_ADDRESS;

void mmk_entry(FRAMEBUFFER* framebuffer, PSF_FONT* font, EFI_MEMORY_MAP_INFO memory_map_info) {
    fb_clear(framebuffer, 0x00000000);
    
    BASIC_OUTPUT_CONSOLE con;
    boutcon_init(&con, framebuffer, font, 0xFFDDDDDD, 0x00000000);
    klog_init(KERNEL_LOG_MODE_CON_DISPLAYED, &con);

    klogf("Modern Minimal Operating System version %s\n", MMK_VERSION);
    klogf("Total detected memory size: %d MiB\n", get_memory_size(memory_map_info) / 0x100000);
    
    klogf("Reading EFI memory map and allocating free memory...\n");
    pfallocator_read_efi_memory_map(&global_allocator, memory_map_info);

    klogf("Ensuring kernel pages are locked/reserved...\n");
    uint64_t kernel_size = (uint64_t)&_MMK_END_ADDRESS - (uint64_t)&_MMK_START_ADDRESS;
    uint64_t kernel_pages = kernel_size / 4096;
    pfallocator_lock_pages(&global_allocator, &_MMK_START_ADDRESS, kernel_pages);

    klogf("Map read and system memory reserved!\nFree memory: %d KB\nUsed memory: %d KB\nReserved memory %d KB\n", get_free_memory() / 1024, get_used_memory() / 1024, get_reserved_memory() / 1024);

    klogf("Setting up new page tables...\n");
    PAGE_TABLE* pml4 = (PAGE_TABLE*)pfallocator_request_page(&global_allocator);
    memset(pml4, 0, 4096);

    PAGE_TABLE_MANAGER pt_manager;
    ptmanager_init(&pt_manager, pml4);

    klogf("Mapping full memory range...\n");
    for (uint64_t i = 0; i < get_memory_size(memory_map_info); i += 4096) {
        ptmanager_map_memory(&pt_manager, (void*)i, (void*)i);
    }

    klogf("Mapping kernel...\n");
    for (uint64_t i = (uint64_t)&_MMK_START_ADDRESS; i < (uint64_t)&_MMK_END_ADDRESS; i += 4096) {
        ptmanager_map_memory(&pt_manager, (void*)i, (void*)i);
    }

    klogf("Mapping framebuffer...\n");
    uint64_t framebuffer_base = (uint64_t)framebuffer->base_address;
    uint64_t framebuffer_size = (uint64_t)framebuffer->size + 0x1000;

    for (uint64_t i = framebuffer_base; i < framebuffer_base + framebuffer_size; i += 4096) {
        ptmanager_map_memory(&pt_manager, (void*)i, (void*)i);
    }

    klogf("Loading new page tables...\n");
    asm ("mov %0, %%cr3" :: "r" (pml4));
    klogf("Page tables loaded!\n");

    halt();
}
