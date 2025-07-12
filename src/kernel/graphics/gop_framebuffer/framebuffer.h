/*
 * mmos/src/kernel/graphics/gop_framebuffer/framebuffer.h
 * framebuffer functions and struct
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#pragma once

#include <stddef.h>
#include <stdint.h>

#define GOP_FB_BYTES_PER_PIXEL 4

typedef uint32_t PIXEL;
typedef uint32_t COLOR32;

typedef struct {
    void* base_address;
    size_t size;
    unsigned int width;
    unsigned int height;
    unsigned int pixels_per_scanline;
} FRAMEBUFFER;

void fb_clear(FRAMEBUFFER* framebuffer, COLOR32 color);
void fb_draw_pixel(FRAMEBUFFER* framebuffer, unsigned int x, unsigned int y, COLOR32 color);
