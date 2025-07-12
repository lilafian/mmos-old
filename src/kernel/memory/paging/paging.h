/*
 * mmos/src/kernel/memory/paging/paging.h
 * utilities for paging
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#pragma once

#include <stdint.h>

#define PAGE_PRESENT   (1ULL << 0)
#define PAGE_RW        (1ULL << 1)
#define PAGE_USER      (1ULL << 2)
#define PAGE_PWT       (1ULL << 3)
#define PAGE_PCD       (1ULL << 4)
#define PAGE_ACCESSED  (1ULL << 5)
#define PAGE_DIRTY     (1ULL << 6)
#define PAGE_PS        (1ULL << 7)
#define PAGE_GLOBAL    (1ULL << 8)
#define PAGE_NX        (1ULL << 63)
#define PAGE_ADDR_MASK 0x000FFFFFFFFFF000ULL

typedef uint64_t PAGE_DIRECTORY_ENTRY;

typedef struct {
    PAGE_DIRECTORY_ENTRY entries[512];
} __attribute__((aligned(0x1000))) PAGE_TABLE;
