/*
 * mmos/src/kernel/memory/page_frame_allocator/allocator.c
 * functions and struct for the page allocator
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#include "allocator.h"

uint64_t free_memory;
uint64_t reserved_memory;
uint64_t used_memory;
bool initialized = false;

PAGE_FRAME_ALLOCATOR global_allocator;

void pfallocator_read_efi_memory_map(PAGE_FRAME_ALLOCATOR* allocator, EFI_MEMORY_MAP_INFO memory_map_info) {
    if (initialized) return;
    initialized = true;

    uint64_t memory_map_entries = memory_map_info.size / memory_map_info.descriptor_size;
    void* largest_free_memory_segment = NULL;
    size_t largest_free_memory_segment_size = 0;
    for (int i = 0; i < memory_map_entries; i++) {
        EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)memory_map_info.map + (i * memory_map_info.descriptor_size));
        if (descriptor->type == EFI_CONVENTIONAL_MEMORY) {
            if (descriptor->page_count * 4096 > largest_free_memory_segment_size) {
                largest_free_memory_segment = descriptor->physical_address;
                largest_free_memory_segment_size = descriptor->page_count * 4096;
            }
        }
    }

    uint64_t memory_size = get_memory_size(memory_map_info);
    free_memory = memory_size;
    
    uint64_t bitmap_size = memory_size / 4096 / 8 + 1;
    __pfallocator_initialize_bitmap(allocator, bitmap_size, largest_free_memory_segment);

    pfallocator_lock_pages(allocator, &allocator->page_bitmap, allocator->page_bitmap.size / 4096 + 1);

       
    for (int i = 0; i < memory_map_entries; i++) {
        EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)memory_map_info.map + (i * memory_map_info.descriptor_size));
        if (descriptor->type != EFI_CONVENTIONAL_MEMORY && descriptor->type != EFI_RESERVED_MEMORY_TYPE) {
            __pfallocator_reserve_pages(allocator, descriptor->physical_address, descriptor->page_count);
        }
    }
}

void __pfallocator_initialize_bitmap(PAGE_FRAME_ALLOCATOR* allocator, size_t bitmap_size, void* buffer_address) {
    allocator->page_bitmap.size = bitmap_size;
    allocator->page_bitmap.buffer = (uint8_t*)buffer_address;
    memset((void*)allocator->page_bitmap.buffer, 0, bitmap_size);
}

void pfallocator_free_page(PAGE_FRAME_ALLOCATOR* allocator, void* address) {
    uint64_t index = (uint64_t)address / 4096;
    if (bitmap_get(&allocator->page_bitmap, index) == false) return;
    bitmap_set(&allocator->page_bitmap, index, false);
    free_memory += 4096;
    used_memory -= 4096;
}

void pfallocator_free_pages(PAGE_FRAME_ALLOCATOR* allocator, void* address, uint64_t page_count) {
    for (int i = 0; i < page_count; i++) {
        pfallocator_free_page(allocator, (void*)((uint64_t)address + (i * 4096)));
    }
}

void pfallocator_lock_page(PAGE_FRAME_ALLOCATOR* allocator, void* address) {
    uint64_t index = (uint64_t)address / 4096;
    if (bitmap_get(&allocator->page_bitmap, index) == true) return;
    bitmap_set(&allocator->page_bitmap, index, true);
    free_memory -= 4096;
    used_memory += 4096;
}

void pfallocator_lock_pages(PAGE_FRAME_ALLOCATOR* allocator, void* address, uint64_t page_count) {
    for (int i = 0; i < page_count; i++) {
        pfallocator_lock_page(allocator, (void*)((uint64_t)address + (i * 4096)));
    }
}

void __pfallocator_reserve_page(PAGE_FRAME_ALLOCATOR* allocator, void* address) {
    uint64_t index = (uint64_t)address / 4096;
    if (bitmap_get(&allocator->page_bitmap, index) == true) return;
    bitmap_set(&allocator->page_bitmap, index, true);
    free_memory -= 4096;
    reserved_memory += 4096;
}

void __pfallocator_reserve_pages(PAGE_FRAME_ALLOCATOR* allocator, void* address, uint64_t page_count) {
    for (int i = 0; i < page_count; i++) {
        __pfallocator_reserve_page(allocator, (void*)((uint64_t)address + (i * 4096)));
    }
}

void __pfallocator_release_page(PAGE_FRAME_ALLOCATOR* allocator, void* address) {
    uint64_t index = (uint64_t)address / 4096;
    if (bitmap_get(&allocator->page_bitmap, index) == false) return;
    bitmap_set(&allocator->page_bitmap, index, false);
    free_memory += 4096;
    reserved_memory -= 4096;
}

void __pfallocator_release_pages(PAGE_FRAME_ALLOCATOR* allocator, void* address, uint64_t page_count) {
    for (int i = 0; i < page_count; i++) {
        __pfallocator_release_page(allocator, (void*)((uint64_t)address + (i * 4096)));
    }
}

void* pfallocator_request_page(PAGE_FRAME_ALLOCATOR* allocator) {
    for (uint64_t index = 0; index < allocator->page_bitmap.size * 8; index++) {
        if (bitmap_get(&allocator->page_bitmap, index) == true) continue;
        pfallocator_lock_page(allocator, (void*)(index * 4096));
        return (void*)(index * 4096);
    }

    // TODO: implement swap here
    return NULL;
}

uint64_t get_free_memory() {
    return free_memory;
}

uint64_t get_used_memory() {
    return used_memory;
}

uint64_t get_reserved_memory() {
    return reserved_memory;
}
