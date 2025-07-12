/*
 * mmos/src/kernel/console/basic_output_console/basic_output_console.h
 * struct and functions for the basic output console
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */

#pragma once

#include "../../graphics/gop_framebuffer/framebuffer.h"
#include "../../graphics/fonts/psf/psf.h"

#define BOUTCON_OUTBUFFER_SIZE 4096
typedef struct {
    char output_buffer[BOUTCON_OUTBUFFER_SIZE];
    FRAMEBUFFER* framebuffer;
    PSF_FONT* font;
    COLOR32 foreground_color;
    COLOR32 background_color;
} BASIC_OUTPUT_CONSOLE;

BASIC_OUTPUT_CONSOLE* boutcon_init(BASIC_OUTPUT_CONSOLE* console, FRAMEBUFFER* framebuffer, PSF_FONT* font, COLOR32 foreground_color, COLOR32 background_color);
void boutcon_write_no_display(BASIC_OUTPUT_CONSOLE* console, char* message);
void boutcon_write(BASIC_OUTPUT_CONSOLE* console, char* message);
void boutcon_display(BASIC_OUTPUT_CONSOLE* console);
