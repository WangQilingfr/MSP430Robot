#ifndef DISPLAY_H
#define DISPLAY_H

#define CLEAR_DISPLAY 0x76

#include <msp430g2553.h>
#include "stdint.h"

void init_display (void);
void show_number (uint16_t number);
void show_string (const uint8_t *msg);
void show_byte (uint8_t byte);

#endif

