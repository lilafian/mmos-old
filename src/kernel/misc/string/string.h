/*
 * mmos/src/kernel/misc/string/string.h
 * string functions
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#pragma once

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* string);
char* strcat(char* dest, const char* src);
char* strchrcat(char* dest, const char src);
char* itoa(uint64_t val, int base);
