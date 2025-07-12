/*
 * mmos/src/kernel/memory/efi_map.h
 * efi memory map interaction
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */

#pragma once

#include <stdint.h>

#define EFI_RESERVED_MEMORY_TYPE        0
#define EFI_LOADER_CODE                 1
#define EFI_LOADER_DATA                 2
#define EFI_BOOT_SERVICES_CODE          3
#define EFI_BOOT_SERVICES_DATA          4
#define EFI_RUNTIME_SERVICES_CODE       5
#define EFI_RUNTIME_SERVICES_DATA       6
#define EFI_CONVENTIONAL_MEMORY         7
#define EFI_UNUSABLE_MEMORY             8
#define EFI_ACPI_RECLAIM_MEMORY         9
#define EFI_ACPI_MEMORY_NVS            10
#define EFI_MEMORY_MAPPED_IO           11
#define EFI_MEMORY_MAPPED_IO_PORT_SPACE 12
#define EFI_PAL_CODE                   13

typedef struct {
    uint32_t type;
    uint32_t __pad0;
    void* physical_address;
    void* virtual_address;
    uint64_t page_count;
    uint64_t attributes;
    uint32_t __pad1;
} EFI_MEMORY_DESCRIPTOR;

extern const char* EFI_MEMORY_TYPE_STRINGS[];
