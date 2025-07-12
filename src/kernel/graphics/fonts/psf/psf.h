/*
 * mmos/src/kernel/graphics/fonts/psf/psf.h
 * struct and functions for the pc screen font format
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "../../gop_framebuffer/framebuffer.h"

#define PSF_MAGIC 0x864ab572

typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t header_size;
    uint32_t flags;
    uint32_t glyph_count;
    uint32_t bytes_per_glyph;
    uint32_t height;
    uint32_t width;
} PSF_FONT_HEADER;

typedef struct {
    PSF_FONT_HEADER* header;
    void* glyph_buffer;
} PSF_FONT;

void fb_psf_draw_character(FRAMEBUFFER* framebuffer, PSF_FONT* font, char character, int x, int y, COLOR32 foreground, COLOR32 background);
void fb_psf_draw_string(FRAMEBUFFER* framebuffer, PSF_FONT* font, char* string, int x, int y, COLOR32 foreground, COLOR32 background);
