/*
 * mmos/src/kernel/entry.c
 * mmkernel entry point and initialization
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#include <stdint.h>
#include "commondef.h"
#include "graphics/gop_framebuffer/framebuffer.h"
#include "graphics/fonts/psf/psf.h"
#include "console/basic_output_console/basic_output_console.h"
#include "misc/klog/klog.h"

void halt() {
    while(1) {
        asm("hlt");
    }
}

void mmk_entry(FRAMEBUFFER* framebuffer, PSF_FONT* font) {
    fb_clear(framebuffer, 0x00000000);
    
    BASIC_OUTPUT_CONSOLE con;
    boutcon_init(&con, framebuffer, font, 0xFFFFFFFF, 0x00000000);
    klog_init(KERNEL_LOG_MODE_CON_DISPLAYED, &con);

    klogf("Modern Minimal Operating System version %s", MMK_VERSION);

    halt();
}
