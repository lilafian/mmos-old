/*
 * mmos/src/kernel/memory/bitmap.c
 * utilities for creating bitmap data structures
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#include "bitmap.h"

bool bitmap_get(BITMAP* bitmap, uint64_t index) {
    uint64_t byte_index = index / 8;
    uint8_t bit_index = index % 8;
    uint8_t bit_mask = 0b10000000 >> bit_index;
    if ((bitmap->buffer[byte_index] & bit_mask) > 0) {
        return true;
    }
    return false;
}

void bitmap_set(BITMAP* bitmap, uint64_t index, bool value) {
    uint64_t byte_index = index / 8;
    uint8_t bit_index = index % 8;
    uint8_t bit_mask = 0b10000000 >> bit_index;

    bitmap->buffer[byte_index] &= ~bit_mask;
    if (value) {
        bitmap->buffer[byte_index] |= bit_mask;
    }
}
