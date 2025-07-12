/*
 * mmos/src/bootloader/fs/fs.h
 * filesystem functions for mmload
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#pragma once
#include <efi/efi.h>
#include <efi/efilib.h>

EFI_FILE_HANDLE get_volume(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);
EFI_FILE* load_file(EFI_FILE* directory, CHAR16* filename, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);
UINT64 get_file_size(EFI_FILE* file, EFI_SYSTEM_TABLE* SystemTable);
