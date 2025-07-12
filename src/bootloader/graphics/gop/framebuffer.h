/*
 * mmos/src/bootloader/graphics/gop/framebuffer.h
 * functions for accessing the graphics output protocol
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */

#pragma once

#include <stddef.h>

typedef struct {
    void* base_address;
    size_t size;
    unsigned int width;
    unsigned int height;
    unsigned int pixels_per_scanline;
} FRAMEBUFFER;
