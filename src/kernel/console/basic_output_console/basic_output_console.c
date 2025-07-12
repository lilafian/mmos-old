/*
 * mmos/src/kernel/console/basic_output_console/basic_output_console.c
 * struct and functions for the basic output console
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */

#include "basic_output_console.h"
#include "../../misc/string/string.h"
#include "../../graphics/fonts/psf/psf.h"

BASIC_OUTPUT_CONSOLE* boutcon_init(BASIC_OUTPUT_CONSOLE* console, FRAMEBUFFER* framebuffer, PSF_FONT* font, COLOR32 foreground_color, COLOR32 background_color) {
    console->framebuffer = framebuffer;
    console->font = font;
    console->foreground_color = foreground_color;
    console->background_color = background_color;
    return console;
}

void boutcon_write_no_display(BASIC_OUTPUT_CONSOLE* console, char* message) {
    if (4096 - strlen(console->output_buffer) < strlen(message)) {
        return;
    }
    strcat(console->output_buffer, message);
}

void boutcon_write(BASIC_OUTPUT_CONSOLE* console, char* message) {
    boutcon_write_no_display(console, message);
    boutcon_display(console);
}

void boutcon_display(BASIC_OUTPUT_CONSOLE* console) {
    fb_clear(console->framebuffer, 0x00000000);
    fb_psf_draw_string(console->framebuffer, console->font, console->output_buffer, 0, 0, console->foreground_color, console->background_color);
}
