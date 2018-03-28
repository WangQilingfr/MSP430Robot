#ifndef COMPASS_H
#define COMPASS_H

#include <msp430g2553.h>
#include "stdint.h"

#define MAX_low_BEARING 255
#define MAX_BEARING 3599



uint8_t compass_init(void);
uint8_t compass_init_tout(void);
uint8_t Read_compass_8 (void); // Compass bearing as byte, 0 to 255 for a full circle
uint8_t Read_compass_16 (uint16_t *bearing); //Compass bearing  as word, reading two times


#endif

