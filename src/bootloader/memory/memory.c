/*
 * mmos/src/bootloader/memory/memory.c
 * memory functions
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#include "memory.h"

// THIS FUNCTION SUCKS BUT ITS STANDARD SO DONT TOUCH IT (its actuallly not but still dont)
int memcmp(const void* a_ptr, const void* b_ptr, size_t n) {
    const unsigned char* a = (const unsigned char*)a_ptr;
    const unsigned char* b = (const unsigned char*)b_ptr;
    for (size_t i = 0; i < n; i++) {
        if (a[i] < b[i]) return -1;
        else if (a[i] > b[i]) return 1;
    }
    return 0;
}
