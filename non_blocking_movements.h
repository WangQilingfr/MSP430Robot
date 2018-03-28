#ifndef NON_BLOCKING_MOVEMENTS_H_
#define NON_BLOCKING_MOVEMENTS_H_

#include <msp430g2553.h>
#include "stdint.h"
#include "sides.h"

#include "control_motors.h"
#include "control_encoders.h"
#include "system.h"
#include "display.h"


#define STOP 0
#define MOVE 1


uint8_t nb_straight_move(int8_t direction, uint8_t speedRobot, uint16_t set_turns, uint8_t set_steps);
uint8_t nb_spin_steps(int8_t spinDirection, uint8_t speedRobot, uint16_t set_turns, uint8_t set_steps);
/*
 * check if the robot has reached the turns and steps to stop using the values stored in distance_to_stop
 * return
 *  0- robot arrived
 * 	1- robot is not arrived
 */
uint8_t check_stop_steps();

#endif
