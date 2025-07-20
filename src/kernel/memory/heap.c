/*
 * mmos/src/kernel/memory/heap.c
 * functions and definitions for heap memory
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#include "heap.h"
#include "paging/table_manager/pt_manager.h"
#include "paging/page_frame_allocator/allocator.h"

void* heap_start;
void* heap_end;
HEAP_SEGMENT_HEADER* heap_last_header;

void heap_init(void* heap_address, size_t page_count) {
    void* pos = heap_address;

    for (size_t i = 0; i < page_count; i++) {
        ptmanager_map_memory(global_pt_manager, pos, pfallocator_request_page(global_allocator));
        pos = (void*)((size_t)pos + 0x1000);
    }

    size_t heap_length = page_count * 0x1000;
    
    heap_start = heap_address;
    heap_end = (void*)((size_t)heap_start + heap_length);
    HEAP_SEGMENT_HEADER* start_segment = (HEAP_SEGMENT_HEADER*)heap_address;
    start_segment->length = heap_length - sizeof(HEAP_SEGMENT_HEADER);
    start_segment->next = NULL;
    start_segment->last = NULL;
    start_segment->free = true;
    heap_last_header = start_segment;
}

void* malloc(size_t size) {
    if (size == 0) return NULL;

    if (size % 0x10 > 0) {
        size -= (size % 0x10);
        size += 0x10;
    }

    HEAP_SEGMENT_HEADER* current_segment = (HEAP_SEGMENT_HEADER*)heap_start;
    while(true) {
        if (current_segment->free) {
            if (current_segment->length > size) {
                heapseg_split(current_segment, size);
            }
            current_segment->free = false;
            return (void*)((uint64_t)current_segment + sizeof(HEAP_SEGMENT_HEADER));
        }

        if (current_segment->next == NULL) break;
        current_segment = current_segment->next;
    }
    heap_expand(size);
    return malloc(size);
}

void free(void* address) {
    HEAP_SEGMENT_HEADER* segment = (HEAP_SEGMENT_HEADER*)address - 1;
    segment->free = true;
    heapseg_combine_forward(segment);
    heapseg_combine_backward(segment);
}

HEAP_SEGMENT_HEADER* heapseg_split(HEAP_SEGMENT_HEADER* segment, size_t length) { 
    if (length < 0x10) return NULL;
    int64_t split_segment_length = segment->length - length - (sizeof(HEAP_SEGMENT_HEADER));
    if (split_segment_length < 0x10) return NULL;
    
    HEAP_SEGMENT_HEADER* new_header = (HEAP_SEGMENT_HEADER*)((size_t)segment + length + sizeof(HEAP_SEGMENT_HEADER));
    segment->next->last = new_header;
    new_header->next = segment->next;
    segment->next = new_header;
    new_header->last = segment;
    new_header->length = split_segment_length;
    new_header->free = segment->free;
    segment->length = length;
    
    if (heap_last_header == segment) {
        heap_last_header = new_header;
    }

    return new_header;
}

void heapseg_combine_forward(HEAP_SEGMENT_HEADER* segment) {
    if (segment->next == NULL) return;
    if (!segment->next->free) return;

    if (segment->next == heap_last_header) heap_last_header = segment;

    if (segment->next->next != NULL) {
        segment->next->next->last = segment;
    }

    segment->next = segment->next->next;

    segment->length = segment->length = segment->next->length + sizeof(HEAP_SEGMENT_HEADER);
}

void heapseg_combine_backward(HEAP_SEGMENT_HEADER* segment) {
    if (segment->last != NULL && segment->last->free) heapseg_combine_forward(segment);
}

void heap_expand(size_t length) { 
    if (length % 0x1000) {
        length -= length % 0x1000;
        length += 0x1000;
    }

    size_t page_count = length / 0x1000;
    HEAP_SEGMENT_HEADER* new_segment = (HEAP_SEGMENT_HEADER*)heap_end;
    
    for (size_t i = 0; i < page_count; i++) {
        ptmanager_map_memory(global_pt_manager, heap_end, pfallocator_request_page(global_allocator));
        heap_end = (void*)((size_t)heap_end + 0x1000);
    }

    new_segment->free = true;
    new_segment->last = heap_last_header;
    heap_last_header->next = new_segment;
    heap_last_header = new_segment;

    new_segment->next = NULL;
    new_segment->length = length - sizeof(HEAP_SEGMENT_HEADER);
    heapseg_combine_backward(new_segment);
}
