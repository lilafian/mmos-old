/*
 * mmos/src/kernel/misc/string/string.c
 * string functions
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#include "string.h"

size_t strlen(const char* string) {
    size_t len = 0;
    while (string[len] != '\0') {
        len++;
    }
    return len;
}

char* strcat(char* dest, const char* src) {
    char* ptr = dest;
    while (*ptr) ptr++;
    while (*src) *ptr++ = *src++;
    *ptr = '\0';
    return dest;
}

char* strchrcat(char* dest, const char src) {
    char* ptr = dest;
    while(*ptr) ptr++;
    *ptr++ = src;
    *ptr = '\0';
    return dest;
}

char* itoa(uint64_t val, int base) {
    if (base > 16) base = 16;
    if (val == 0) return "0";

    static char buf[64] = {0};

    int i = 62;
    for (; val && i; --i, val/=base) {
        buf[i] = "0123456789abcdef"[val % base];
    }

    return &buf[i+1];
}
