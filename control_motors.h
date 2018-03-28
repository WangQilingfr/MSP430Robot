#ifndef CONTROL_MOTORS_H
#define CONTROL_MOTORS_H


#include <msp430g2553.h>
#include "stdint.h"
#include "sides.h"



void Init_motors(void);
uint8_t Speed_motor (int8_t percentage, uint8_t side);
void emergency_stop (void);

#endif

