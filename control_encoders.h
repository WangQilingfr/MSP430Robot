#ifndef CONTROL_ENCODERS_H
#define CONTROL_ENCODERS_H

#include <msp430g2553.h>
#include "stdint.h"
#include "sides.h"


#define MAX_STEPS 12

typedef struct struct_distance{
  uint8_t steps;
  uint16_t turns;
  uint8_t centimeters;
  uint16_t meters;
} distance_type;

void Init_encoders_distance();
void Clear_Distance(uint8_t side);
uint8_t Read_distance (uint8_t side, distance_type *dist);

#endif

