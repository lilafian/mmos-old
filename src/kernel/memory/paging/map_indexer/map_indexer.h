/*
 * mmos/src/kernel/memory/paging/map_indexer/map_indexer.h
 * utility for getting page map indexes of pages
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#pragma once

#include <stdint.h>

typedef struct {
    uint64_t pdp_index;
    uint64_t pd_index;
    uint64_t pt_index;
    uint64_t p_index;
} PAGE_MAP_INDEXER;

void map_index(PAGE_MAP_INDEXER* indexer, uint64_t virtual_address);
