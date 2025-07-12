/*
 * mmos/src/kernel/memory/paging/map_indexer/map_indexer.c
 * utility for getting page map indexes of pages
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */

#include "map_indexer.h"

void map_index(PAGE_MAP_INDEXER* indexer, uint64_t virtual_address) {
    virtual_address >>= 12;
    indexer->p_index = virtual_address & 0x1ff;
    virtual_address >>= 9;
    indexer->pt_index = virtual_address & 0x1ff;
    virtual_address >>= 9;
    indexer->pd_index = virtual_address & 0x1ff;
    virtual_address >>= 9;
    indexer->pdp_index = virtual_address & 0x1ff;
}
