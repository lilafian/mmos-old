/*
 * mmos/src/kernel/entry.c
 * mmkernel entry point and initialization
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#include <stdint.h>
#include "misc/init/init.h"
#include "graphics/gop_framebuffer/framebuffer.h"
#include "graphics/fonts/psf/psf.h"
#include "memory/memory.h"
#include "devices/acpi/acpi.h"

void halt() {
    while(1) {
        asm("hlt");
    }
}

extern uint64_t _MMK_START_ADDRESS;
extern uint64_t _MMK_END_ADDRESS;

void mmk_entry(FRAMEBUFFER* framebuffer, PSF_FONT* font, EFI_MEMORY_MAP_INFO memory_map_info, RSDP2* rsdp) {
    kinit_init_display(framebuffer, font);
    kinit_init_memory(&_MMK_START_ADDRESS, &_MMK_END_ADDRESS, framebuffer, memory_map_info);
    kinit_init_pci(rsdp);

    halt();
}
