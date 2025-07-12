/*
 * mmos/src/bootloader/graphics/fonts/psf.h
 * functions and struct for the pc screen font format
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <efi/efi.h>

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

PSF_FONT* load_font_from_file(EFI_FILE* directory, CHAR16* filename, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);
