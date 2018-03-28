/*
 * blocking_movements.h
 *
 *  Created on: 7 mars 2018
 *      Author: wen.yang.13
 */

#ifndef BLOCKING_MOVEMENTS_H_
#define BLOCKING_MOVEMENTS_H_

#define FORWARD 1
#define REVERSE -1
#define CORRECTION 2

uint8_t checkParameters(int8_t direction, uint8_t speedRobot, uint16_t set_turns, uint8_t set_steps);
uint8_t spin(int8_t spinDirection, uint8_t speedRobot);
uint8_t straight_move(int8_t direction, uint8_t speedRobot, uint16_t set_turns, uint8_t set_steps);
uint8_t spin_steps(int8_t spinDirection, uint8_t speedRobot, uint16_t set_turns, uint8_t set_steps);
uint8_t spin_bearing (int8_t spinDirection, uint8_t speedRobot, uint16_t target_bearing);

#endif /* BLOCKING_MOVEMENTS_H_ */
