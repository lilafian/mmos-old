/*
 * mmos/src/kernel/devices/acpi/acpi.c
 * acpi functions and structs
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */

#include "acpi.h"
#include <stddef.h>

void* acpi_find_table(SDT_HEADER* sdt_header, char* signature) {
    int entries = (sdt_header->length - sizeof(SDT_HEADER)) / 8;
    for (int i = 0; i < entries; i++) {
        SDT_HEADER* new_sdt = (SDT_HEADER*)*(uint64_t*)((uint64_t)sdt_header + sizeof(SDT_HEADER) + (i * 8));
        for (int j = 0; j < 4; j++) {
            if (new_sdt->signature[j] != signature[j]) break;
            if (j == 3) return new_sdt;
        }
    }
    return NULL;
}
