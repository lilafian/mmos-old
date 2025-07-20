/*
 * mmos/src/kernel/devices/pci/pci.c
 * pci functions and structs
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */

#include "pci.h"
#include "../../memory/paging/table_manager/pt_manager.h"
#include "../../misc/klog/klog.h"

void pci_enumerate_function(uint64_t device_address, uint64_t function) {
    uint64_t offset = function << 12;
    uint64_t function_address = device_address + offset;
    ptmanager_map_memory(global_pt_manager, (void*)function_address, (void*)function_address);

    PCI_DEVICE_HEADER* device_header = (PCI_DEVICE_HEADER*)device_address;
    if (device_header->device_id == 0 || device_header->device_id == 0xFFFF) return;
}

void pci_enumerate_device(uint64_t bus_address, uint64_t device) {
    uint64_t offset = device << 15;
    uint64_t device_address = bus_address + offset;

    ptmanager_map_memory(global_pt_manager, (void*)device_address, (void*)device_address);

    PCI_DEVICE_HEADER* device_header = (PCI_DEVICE_HEADER*)device_address;
    if (device_header->device_id == 0 || device_header->device_id == 0xFFFF) return;
    
    // Uncomment this for some readable device info
    // klogf("%s / %s / %s / %s / %s\n", pci_get_vendor_name(device_header->vendor_id), pci_get_device_name(device_header->vendor_id, device_header->device_id), PCI_DEVICE_CLASSES[device_header->device_class], pci_get_subclass_name(device_header->device_class, device_header->device_subclass), pci_get_program_interface_name(device_header->device_class, device_header->device_class, device_header->program_interface));
    
    for (uint64_t function = 0; function < 8; function++) {
        pci_enumerate_function(device_address, function);
    }
}

void pci_enumerate_bus(uint64_t base_address, uint64_t bus) {
    uint64_t offset = bus << 20;
    uint64_t bus_address = base_address + offset;
    ptmanager_map_memory(global_pt_manager, (void*)bus_address, (void*)bus_address);

    PCI_DEVICE_HEADER* device_header = (PCI_DEVICE_HEADER*)bus_address;
    if (device_header->device_id == 0 || device_header->device_id == 0xFFFF) return;
    
    for (uint64_t device = 0; device < 32; device++) {
        pci_enumerate_device(bus_address, device);
    }
}

void pci_enumerate(MCFG_HEADER* mcfg) {
    int entries = ((mcfg->header.length) - sizeof(MCFG_HEADER)) / sizeof(ACPI_DEVICE_CONFIG);
    for (int i = 0; i < entries; i++) {
        ACPI_DEVICE_CONFIG* new_config = (ACPI_DEVICE_CONFIG*)((uint64_t)mcfg + sizeof(MCFG_HEADER) + (sizeof(ACPI_DEVICE_CONFIG) * i));
        pci_enumerate_bus(new_config->base_address, 0);
        // TODO: MAKE THIS ACTUALLY GO THROUGH EVERY BUS, IT IS BUGGY RN
    }
}
