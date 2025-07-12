/*
 * mmos/src/kernel/memory/efi_map.c
 * efi memory map interaction
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */

#include "efi_map.h"

const char* EFI_MEMORY_TYPE_STRINGS[] = {
    "EfiReservedMemoryType",
    "EfiLoaderCode",
    "EfiLoaderData",
    "EfiBootServicesCode",
    "EfiBootServicesData",
    "EfiRuntimeServicesCode",
    "EfiRuntimeServicesData",
    "EfiConventionalMemory",
    "EfiUnusableMemory",
    "EfiACPIReclaimMemory",
    "EfiACPIMemoryNVS",
    "EfiMemoryMappedIO",
    "EfiMemoryMappedIOPortSpace",
    "EfiPalCode"
};
