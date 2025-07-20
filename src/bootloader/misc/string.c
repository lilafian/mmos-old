/*
 * mmos/src/bootloader/misc/string.c
 * string functions
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#include "string.h"

UINTN strncmp(CHAR8* a, CHAR8* b, UINTN length) {
    for (UINTN i = 0; i < length; i++) {
        if (*(a + i) != *(b + i)) return 1;
    }
    return 0;
}
