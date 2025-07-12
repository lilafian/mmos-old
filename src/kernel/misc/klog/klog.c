/*
 * mmos/src/kernel/misc/klog/klog.h
 * kernel logging functions
 * licensed under the gnu general public license version 3. see mmos/LICENSE for more information.
 */ 

#include "klog.h"
#include "../string/string.h"

KERNEL_LOG_MODE kernel_log_mode;
BASIC_OUTPUT_CONSOLE* kernel_log_boutcon;

void klog(char* message) {
    switch (kernel_log_mode) {
        case KERNEL_LOG_MODE_CON_DISPLAYED:
            boutcon_write(kernel_log_boutcon, message);
            break;
        case KERNEL_LOG_MODE_CON_NODISPLAY:
            boutcon_write_no_display(kernel_log_boutcon, message);
            break;
        default:
            kernel_log_mode = 0;
            klog(message);
    }
}

void klogf(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char buffer[1024];
    buffer[0] = '\0';

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch(*fmt) {
                case 's': { // string
                    char* string = va_arg(args, char*);
                    strcat(buffer, string);
                    break;
                }
                case 'c': { // character
                    char character = va_arg(args, char);
                    strchrcat(buffer, character);
                    break;
                }
                case 'd': { // decimal uint
                    uint64_t value = va_arg(args, uint64_t);
                    strcat(buffer, itoa(value, 10));
                    break;
                }
                case 'x': {// hex uint
                    uint64_t value = va_arg(args, uint64_t);
                    strcat(buffer, itoa(value, 16));
                    break;
                }
                case 'p': {
                    void* value = va_arg(args, void*);
                    strcat(buffer, itoa((uint64_t)value, 16));
                    break;
                }
                case '%': {
                    strcat(buffer, "%");
                    break;
                }
                default: {
                    strcat(buffer, "%?");
                    break;
                }
            }
        } else {
            char temp[2] = {*fmt, '\0'};
            strcat(buffer, temp);
        }
        fmt++;
    }

    va_end(args);

    klog(buffer);
}

void klog_init(KERNEL_LOG_MODE mode, BASIC_OUTPUT_CONSOLE* boutcon) {
    kernel_log_mode = mode;
    kernel_log_boutcon = boutcon;
}

void klog_set_log_mode(KERNEL_LOG_MODE mode) {
    kernel_log_mode = mode;
}
