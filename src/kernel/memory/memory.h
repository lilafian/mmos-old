/*
 * mmos/src/kernel/memory/memory.h
 * memory functions
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#pragma once

#include <stdint.h>
#include <stddef.h>
#include "efi_map/efi_map.h"

typedef struct {
    EFI_MEMORY_DESCRIPTOR* map;
    uint64_t size;
    uint64_t descriptor_size;
} EFI_MEMORY_MAP_INFO;

void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* s, int c, size_t n);
void* memmove(void* dest, const void* src, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);
uint64_t get_memory_size(EFI_MEMORY_MAP_INFO memory_map_info);
