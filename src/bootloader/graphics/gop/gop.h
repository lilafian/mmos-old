/*
 * mmos/src/bootloader/graphics/gop/gop.h
 * functions for accessing the graphics output protocol
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */

#pragma once

#include <stddef.h>
#include <efi/efi.h>
#include <efi/efilib.h>
#include "framebuffer.h"

void init_gop(EFI_SYSTEM_TABLE* SystemTable, FRAMEBUFFER* framebuffer);
