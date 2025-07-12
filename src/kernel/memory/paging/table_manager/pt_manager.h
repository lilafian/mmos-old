/*
 * mmos/src/kernel/memory/paging/table_manager/pt_manager.h
 * struct and functions for managing page tables of processes
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#pragma once

#include "../paging.h"

typedef struct {
    PAGE_TABLE* pml4;
} PAGE_TABLE_MANAGER;

void ptmanager_init(PAGE_TABLE_MANAGER* manager, PAGE_TABLE* pml4_address);
void ptmanager_map_memory(PAGE_TABLE_MANAGER* manager, void* virtual_address, void* physical_address);
