/*
 * mmos/src/kernel/misc/init/init.h
 * kernel initialization functions
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */

#pragma once
#include "../../commondef.h"
#include "../../graphics/gop_framebuffer/framebuffer.h"
#include "../../graphics/fonts/psf/psf.h"
#include "../../memory/memory.h"
#include "../../devices/acpi/acpi.h"

void kinit_init_display(FRAMEBUFFER* framebuffer, PSF_FONT* font);
void kinit_init_memory(uint64_t* MMK_START_ADDRESS, uint64_t* MMK_END_ADDRESS, FRAMEBUFFER* framebuffer, EFI_MEMORY_MAP_INFO memory_map_info);
void kinit_init_pci(RSDP2* rsdp);
