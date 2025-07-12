/*
 * mmos/src/kernel/graphics/gop_framebuffer/framebuffer.c
 * framebuffer functions
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#include "framebuffer.h"
#include "../../memory/memory.h"

void fb_clear(FRAMEBUFFER* framebuffer, COLOR32 color) {
    memset(framebuffer->base_address, color, framebuffer->size);
}

void fb_draw_pixel(FRAMEBUFFER* framebuffer, unsigned int x, unsigned int y, COLOR32 color) {
    *(unsigned int*)((x * GOP_FB_BYTES_PER_PIXEL) + (y * framebuffer->pixels_per_scanline * GOP_FB_BYTES_PER_PIXEL) + framebuffer->base_address) = color;
}
