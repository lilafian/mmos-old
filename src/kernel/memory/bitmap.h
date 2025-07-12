/*
 * mmos/src/kernel/memory/bitmap.h
 * utilities for creating bitmap data structures
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    size_t size;
    uint8_t* buffer;
} BITMAP;

bool bitmap_get(BITMAP* bitmap, uint64_t index);
bool bitmap_set(BITMAP* bitmap, uint64_t index, bool value);
