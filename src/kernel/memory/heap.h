/*
 * mmos/src/kernel/memory/heap.h
 * functions and definitions for heap memory
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct _HEAP_SEGMENT_HEADER {
    size_t length;
    struct _HEAP_SEGMENT_HEADER* next;
    struct _HEAP_SEGMENT_HEADER* last;
    bool free;
} HEAP_SEGMENT_HEADER;

void heapseg_combine_forward(HEAP_SEGMENT_HEADER* segment);
void heapseg_combine_backward(HEAP_SEGMENT_HEADER* segment);
HEAP_SEGMENT_HEADER* heapseg_split(HEAP_SEGMENT_HEADER* segment, size_t length);

void heap_init(void* heap_address, size_t page_count);
void heap_expand(size_t length);

void* malloc(size_t size);
void free(void* address);
