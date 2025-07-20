/*
 * mmos/src/kernel/devices/pci/pci.h
 * pci functions and structs
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */

#pragma once

#include <stdint.h>
#include "../acpi/acpi.h"

typedef struct {
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t command;
    uint16_t status;
    uint8_t revision_id;
    uint8_t program_interface;
    uint8_t device_subclass;
    uint8_t device_class;
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
}__attribute__((packed)) PCI_DEVICE_HEADER;

void pci_enumerate(MCFG_HEADER* mcfg);

extern const char* PCI_DEVICE_CLASSES[];
const char* pci_get_vendor_name(uint16_t vendor_id);
const char* pci_get_device_name(uint16_t vendor_id, uint16_t device_id);
const char* pci_get_mass_storage_controller_subclass_name(uint8_t subclass_code);
const char* pci_get_serial_bus_controller_subclass_name(uint8_t subclass_code);
const char* pci_get_bridge_device_subclass_name(uint8_t subclass_code);
const char* pci_get_subclass_name(uint8_t class_code, uint8_t subclass_code);
const char* pci_get_program_interface_name(uint8_t class_code, uint8_t subclass_code, uint8_t program_interface);
