/*
 * mmos/src/bootloader/graphics/fonts/psf.c
 * functions and struct for the pc screen font format
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */

#include "psf.h"
#include "../../fs/fs.h"

PSF_FONT* load_font_from_file(EFI_FILE* directory, CHAR16* filename, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    EFI_FILE* font_file = load_file(directory, filename, ImageHandle, SystemTable);
    if (font_file == NULL) return NULL;

    PSF_FONT_HEADER* font_header;
    uefi_call_wrapper(SystemTable->BootServices->AllocatePool, 3, EfiLoaderData, sizeof(PSF_FONT_HEADER), (void**)&font_header);
    UINTN size = sizeof(PSF_FONT_HEADER);
    uefi_call_wrapper(font_file->Read, 3, font_file, &size, font_header);

    if (font_header->magic != PSF_MAGIC) return NULL;

    UINTN glyph_buffer_size = font_header->bytes_per_glyph * font_header->glyph_count;
    
    void* glyph_buffer; {
        uefi_call_wrapper(font_file->SetPosition, 2, font_file, sizeof(PSF_FONT_HEADER));
        uefi_call_wrapper(SystemTable->BootServices->AllocatePool, 3, EfiLoaderData, glyph_buffer_size, (void**)&glyph_buffer);
        uefi_call_wrapper(font_file->Read, 3, font_file, &glyph_buffer_size, glyph_buffer);
    }

    PSF_FONT* font;
    uefi_call_wrapper(SystemTable->BootServices->AllocatePool, 3, EfiLoaderData, sizeof(PSF_FONT), (void**)&font);
    font->header = font_header;
    font->glyph_buffer = glyph_buffer;
    
    return font;
}
