/*
 * mmos/src/bootloader/graphics/gop/gop.c
 * functions for accessing the graphics output protocol
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */

#include "gop.h"

void init_gop(EFI_SYSTEM_TABLE* SystemTable, FRAMEBUFFER* framebuffer) {
    EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;

    EFI_STATUS status = uefi_call_wrapper(SystemTable->BootServices->LocateProtocol, 3, &gop_guid, NULL, (void**)&gop);
    if (EFI_ERROR(status)) {
        Print(L"Unable to locate the graphics output protocol!\n");
        return;
    }
    Print(L"Located the graphics output protocol!\n");

    framebuffer->base_address = (void*)gop->Mode->FrameBufferBase;
    framebuffer->size = gop->Mode->FrameBufferSize;
    framebuffer->width = gop->Mode->Info->HorizontalResolution;
    framebuffer->height = gop->Mode->Info->VerticalResolution;
    framebuffer->pixels_per_scanline = gop->Mode->Info->PixelsPerScanLine;
}
