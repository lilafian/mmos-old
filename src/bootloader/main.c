/*
 * mmos/src/bootloader/main.c
 * mmload main, including setup, preparation, and kernel transfer
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#include <efi/efi.h>
#include <efi/efilib.h>
#include <elf.h>
#include "commondef.h"
#include "fs/fs.h"
#include "memory/memory.h"
#include "graphics/gop/gop.h"
#include "graphics/gop/framebuffer.h"
#include "graphics/fonts/psf.h"

void fail_load(CHAR16* message) {
    Print(L"Load failed: %s", message);
    while(1);
}

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    InitializeLib(ImageHandle, SystemTable);
    Print(L"Modern Minimal Loader version %s\n", MML_VERSION);

    EFI_FILE* kernel_file = load_file(NULL, L"mmkernel", ImageHandle, SystemTable);
    if (kernel_file == NULL) {
        fail_load(L"Failed to find or load /mmkernel on boot partition!\n");
    }
    Print(L"Found /mmkernel!\n");

    Print(L"Checking ELF header of kernel...\n");

    Elf64_Ehdr kernel_header; {
        UINTN file_info_size;
        EFI_FILE_INFO* file_info;
        uefi_call_wrapper(kernel_file->GetInfo, 4, kernel_file, &gEfiFileInfoGuid, &file_info_size, NULL);
        uefi_call_wrapper(SystemTable->BootServices->AllocatePool, 3, EfiLoaderData, file_info_size, (void**)&file_info);
        uefi_call_wrapper(kernel_file->GetInfo, 4, kernel_file, &gEfiFileInfoGuid, &file_info_size, (void**)&file_info);
        
        UINTN kheader_size = sizeof(kernel_header);
        EFI_STATUS status = uefi_call_wrapper(kernel_file->Read, 3, kernel_file, &kheader_size, &kernel_header);
        if (EFI_ERROR(status)) {
            Print(L"Error occured reading kernel: %r", status);
            fail_load(L"\n");
        }
    }

    if (memcmp(&kernel_header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 || kernel_header.e_ident[EI_CLASS] != ELFCLASS64 || kernel_header.e_ident[EI_DATA] != ELFDATA2LSB || kernel_header.e_type != ET_EXEC || kernel_header.e_machine != EM_X86_64 || kernel_header.e_version != EV_CURRENT) {
        fail_load(L"Kernel format is incorrect! Ensure your kernel is an executable ELF.\n");
    }
    Print(L"Kernel header verified! Format is good.\n");

    Print(L"Loading kernel resources...\n");

    EFI_FILE* kresources_dir = load_file(NULL, L"kernel_resources", ImageHandle, SystemTable);
    if (kresources_dir == NULL) {
        fail_load(L"Failed to find kernel resource directory on boot partition!\n");
    }
    EFI_FILE* kresources_fonts_dir = load_file(kresources_dir, L"fonts", ImageHandle, SystemTable);
    if (kresources_fonts_dir == NULL) {
        // TODO: make this not fail once kernel has serial and stuff
        fail_load(L"Failed to find /kernel_resources/fonts on boot partition!\n");
    }
    /*
     * Available fonts in /kernel_resources/fonts:
     * zap-ext-light32.psf
     * zap-ext-light20.psf
     */
    PSF_FONT* font = load_font_from_file(kresources_fonts_dir, L"zap-ext-light20.psf", ImageHandle, SystemTable);
    if (font == NULL) {
        // TODO: make this not fail once kernel has serial and stuff
        fail_load(L"Failed to load a valid PSF2 font from /kernel_resources/fonts/zap-ext-light20.psf on boot partition!\n");
    }
    Print(L"Loaded a valid PSF2 font from /kernel_resources/fonts/zap-ext-light20.psf!\n");

    Elf64_Phdr* program_headers; {
        uefi_call_wrapper(kernel_file->SetPosition, 2, kernel_file, kernel_header.e_phoff);
        UINTN size = kernel_header.e_phnum * kernel_header.e_phentsize;
        uefi_call_wrapper(SystemTable->BootServices->AllocatePool, 3, EfiLoaderData, size, (void**)&program_headers);
        uefi_call_wrapper(kernel_file->Read, 3, kernel_file, &size, program_headers);
    }

    for (
        Elf64_Phdr* program_header = program_headers;
        (char*)program_header < (char*)program_headers + kernel_header.e_phnum * kernel_header.e_phentsize;
        program_header = (Elf64_Phdr*)((char*)program_header + kernel_header.e_phentsize)
    ) {
        switch (program_header->p_type) {
            case PT_LOAD: {
                int pages = (program_header->p_memsz + 0x1000 - 1) / 0x1000;
                Elf64_Addr segment = program_header->p_paddr;
                uefi_call_wrapper(SystemTable->BootServices->AllocatePages, 4, AllocateAddress, EfiLoaderData, pages, &segment);

                uefi_call_wrapper(kernel_file->SetPosition, 2, kernel_file, program_header->p_offset);
                UINTN size = program_header->p_filesz;
                uefi_call_wrapper(kernel_file->Read, 3, kernel_file, &size, (void*)segment);
                break;
            }
        }
    }

    Print(L"Kernel loaded successfully!\n");

    FRAMEBUFFER fb;
    FRAMEBUFFER* framebuffer = &fb;

    init_gop(SystemTable, framebuffer);
   
    Print(L"Framebuffer Information:\n");
    Print(L"Base address: %p\n", framebuffer->base_address);
    Print(L"Size: %x\n", framebuffer->size);
    Print(L"Width: %d\n", framebuffer->width);
    Print(L"Height: %d\n", framebuffer->height);
    Print(L"Pixels per scan line: %d\n", framebuffer->pixels_per_scanline);
    Print(L"Transferring control to kernel...\n");

    void (*mmk_entry)(FRAMEBUFFER*, PSF_FONT*) = ((__attribute__((sysv_abi)) void (*)(FRAMEBUFFER*, PSF_FONT*) ) kernel_header.e_entry);

    Print(L"Goodbye! (hopefully)");
    mmk_entry(framebuffer, font);

    return EFI_SUCCESS;
}
