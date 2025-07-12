/*
 * mmos/src/bootloader/fs/fs.c
 * filesystem functions for mmload
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#include "fs.h"

EFI_FILE_HANDLE get_volume(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    EFI_LOADED_IMAGE* loaded_image = NULL;
    EFI_GUID loaded_image_protocol_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_FILE_IO_INTERFACE* io_volume;
    EFI_GUID simple_fs_protocol_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_FILE_HANDLE volume;

    EFI_STATUS status = uefi_call_wrapper(SystemTable->BootServices->HandleProtocol, 3, ImageHandle, &loaded_image_protocol_guid, (void**)&loaded_image);
    if (EFI_ERROR(status)) {
        Print(L"Failed to load volume - HandleProtocol(LoadedImageProtocol) failed with error: %r\n", status);
        return NULL;
    }
    status = uefi_call_wrapper(SystemTable->BootServices->HandleProtocol, 3, loaded_image->DeviceHandle, &simple_fs_protocol_guid, (void*)&io_volume);
    if (EFI_ERROR(status)) {
        Print(L"Failed to load volume - HandleProtocol(SimpleFileSystemProtocol) failed with error: %r\n", status);
        return NULL;
    }
    status = uefi_call_wrapper(io_volume->OpenVolume, 2, io_volume, &volume);
    if (EFI_ERROR(status)) {
        Print(L"Failed to load volume - OpenVolume failed with error: %r\n", status);
        return NULL;
    }
    return volume;
}

EFI_FILE* load_file(EFI_FILE* directory, CHAR16* filename, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    EFI_FILE* loaded_file;

    if (directory == NULL) {
        directory = get_volume(ImageHandle, SystemTable);
    }

    EFI_STATUS status = uefi_call_wrapper(directory->Open, 5, directory, &loaded_file, filename, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if (EFI_ERROR(status)) {
        Print(L"Failed to find file %s with error: %r\n", filename, status);
        return NULL;
    }

    return loaded_file;
}

UINT64 get_file_size(EFI_FILE* file, EFI_SYSTEM_TABLE* SystemTable) {
    UINTN file_info_size;
    EFI_FILE_INFO* file_info;
    uefi_call_wrapper(file->GetInfo, 4, file, &gEfiFileInfoGuid, &file_info_size, NULL);
    uefi_call_wrapper(SystemTable->BootServices->AllocatePool, 3, EfiLoaderData, file_info_size, (void**)&file_info);
    uefi_call_wrapper(file->GetInfo, 4, file, &gEfiFileInfoGuid, &file_info_size, (void**)&file_info);

    UINT64 size = file_info->FileSize;
    uefi_call_wrapper(SystemTable->BootServices->FreePool, 1, file_info);
    return size;
}
