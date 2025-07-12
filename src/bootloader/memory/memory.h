/*
 * mmos/src/bootloader/memory/memory.h
 * memory functions
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#pragma once

#include <stddef.h>
#include <efi/efi.h>

typedef struct {
    EFI_MEMORY_DESCRIPTOR* map;
    UINTN size;
    UINTN descriptor_size;
} EFI_MEMORY_MAP_INFO;

int memcmp(const void* a_ptr, const void* b_ptr, size_t n);
