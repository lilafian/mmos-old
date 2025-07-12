/*
 * mmos/src/kernel/memory/memory.c
 * memory functions
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#include "memory.h"

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

uint64_t get_memory_size(EFI_MEMORY_MAP_INFO memory_map_info) {
    // we only need to calculate it once
    static uint64_t memory_size_bytes = 0;
    if (memory_size_bytes != 0) return memory_size_bytes;

    uint64_t entry_count = memory_map_info.size / memory_map_info.descriptor_size;
    for (int i = 0; i < entry_count; i++) {
        EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint8_t*)memory_map_info.map + (i * memory_map_info.descriptor_size));
        if (descriptor->type != EFI_RESERVED_MEMORY_TYPE && descriptor->type != EFI_MEMORY_MAPPED_IO) {
            memory_size_bytes += descriptor->page_count * 4096;
        }
    }

    return memory_size_bytes;
}
