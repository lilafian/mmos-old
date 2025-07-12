/*
 * mmos/src/kernel/misc/klog/klog.h
 * kernel logging functions
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#pragma once

#include <stdarg.h>
#include "../../console/basic_output_console/basic_output_console.h"

#define KERNEL_LOG_MODE_CON_DISPLAYED 0
#define KERNEL_LOG_MODE_CON_NODISPLAY 1
typedef uint8_t KERNEL_LOG_MODE;

extern uint8_t kernel_log_mode;
extern BASIC_OUTPUT_CONSOLE* kernel_log_boutcon;

void klog(char* message);
void klogf(char* fmt, ...);

void klog_init(KERNEL_LOG_MODE mode, BASIC_OUTPUT_CONSOLE* boutcon);
void klog_set_log_mode(KERNEL_LOG_MODE mode);
