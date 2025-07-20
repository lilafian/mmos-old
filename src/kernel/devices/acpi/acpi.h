/*
 * mmos/src/kernel/devices/acpi/acpi.h
 * acpi functions and structs
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */

#pragma once

#include <stdint.h>

typedef struct {
    unsigned char signature[8]; // "RSD PTR "
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    uint32_t rsdt_address;
    uint32_t length;
    uint64_t xsdt_address;
    uint8_t extended_checksum;
    uint8_t _reserved[3];
}__attribute__((packed)) RSDP2;

typedef struct {
    unsigned char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    uint8_t oem_id[6];
    uint8_t oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
}__attribute__((packed)) SDT_HEADER;

typedef struct {
    SDT_HEADER header;
    uint64_t _reserved;
}__attribute__((packed)) MCFG_HEADER;

typedef struct {
    uint64_t base_address;
    uint16_t pci_segment_group;
    uint8_t start_bus;
    uint8_t end_bus;
    uint32_t reserved;
}__attribute__((packed)) ACPI_DEVICE_CONFIG;

void* acpi_find_table(SDT_HEADER* sdt_header, char* signature);
