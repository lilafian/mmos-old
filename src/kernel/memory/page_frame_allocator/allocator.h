/*
 * mmos/src/kernel/memory/page_frame_allocator/allocator.h
 * functions and struct for the page allocator
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#pragma once

#include <stdint.h>
#include "../efi_map/efi_map.h"
#include "../bitmap.h"
#include "../memory.h"

typedef struct {
    BITMAP page_bitmap;
} PAGE_FRAME_ALLOCATOR;

void pfallocator_read_efi_memory_map(PAGE_FRAME_ALLOCATOR* allocator, EFI_MEMORY_MAP_INFO memory_map_info);
void __pfallocator_initialize_bitmap(PAGE_FRAME_ALLOCATOR* allocator, size_t bitmap_size, void* buffer_address);

void pfallocator_free_page(PAGE_FRAME_ALLOCATOR* allocator, void* address);
void pfallocator_lock_page(PAGE_FRAME_ALLOCATOR* allocator, void* address);
void __pfallocator_reserve_page(PAGE_FRAME_ALLOCATOR* allocator, void* address);
void __pfallocator_release_page(PAGE_FRAME_ALLOCATOR* allocator, void* address);

void pfallocator_free_pages(PAGE_FRAME_ALLOCATOR* allocator, void* address, uint64_t page_count);
void pfallocator_lock_pages(PAGE_FRAME_ALLOCATOR* allocator, void* address, uint64_t page_count);
void __pfallocator_reserve_pages(PAGE_FRAME_ALLOCATOR* allocator, void* address, uint64_t page_count);
void __pfallocator_release_pages(PAGE_FRAME_ALLOCATOR* allocator, void* address, uint64_t page_count);

void* pfallocator_request_page(PAGE_FRAME_ALLOCATOR* allocator);

uint64_t get_free_memory();
uint64_t get_used_memory();
uint64_t get_reserved_memory();
