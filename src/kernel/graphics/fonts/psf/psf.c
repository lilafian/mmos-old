/*
 * mmos/src/kernel/graphics/fonts/psf/psf.c
 * struct and functions for the pc screen font format
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */

#include "psf.h"
#include "../../../misc/string/string.h"

void fb_psf_draw_character(FRAMEBUFFER* framebuffer, PSF_FONT* font, char character, int x, int y, COLOR32 foreground, COLOR32 background) {
    const PSF_FONT_HEADER* header = font->header;
    if (header->magic != PSF_MAGIC) return;

    int bytes_per_row = (header->width + 7) / 8;
    const unsigned char* glyph = (const unsigned char*)font->glyph_buffer + (character * header->bytes_per_glyph);

    for (uint32_t dy = 0; dy < header->height; dy++) {
        const unsigned char* row = glyph + dy * bytes_per_row;
        for (uint32_t dx = 0; dx < header->width; dx++) {
            int byte_in_row = dx / 8;
            int bit_in_byte = 7 - (dx % 8);
            if (row[byte_in_row] & (1 << bit_in_byte)) {
                fb_draw_pixel(framebuffer, x + dx, y + dy, foreground);
            } else {
                fb_draw_pixel(framebuffer, x + dx, y + dy, background);
            }
        }
    }
}

void fb_psf_draw_string(FRAMEBUFFER* framebuffer, PSF_FONT* font, char* string, int x, int y, COLOR32 foreground, COLOR32 background) {
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] == '\n') {
            y += font->header->height;
            continue;
        }
        fb_psf_draw_character(framebuffer, font, string[i], i * font->header->width + x, y, foreground, background);
    }
}
