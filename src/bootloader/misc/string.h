/*
 * mmos/src/bootloader/misc/string.h
 * string functions
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#pragma once

#include <efi/efi.h>

UINTN strncmp(CHAR8* a, CHAR8* b, UINTN length);
