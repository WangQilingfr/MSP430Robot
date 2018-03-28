
#include<msp430g2553.h>
#include<stdint.h>

#include "control_motors.h"
#include "control_encoders.h"
#include "system.h"
#include "non_blocking_movements.h"
#include "blocking_movements.h"
#include "display.h"
#include "bearing.h"
static distance_type distance_to_stop;
static uint16_t up_bearing;
static uint16_t low_bearing;

uint8_t nb_straight_move(int8_t direction, uint8_t speedRobot, uint16_t set_turns, uint8_t set_steps)
{
	distance_type distance;
	uint8_t errorSpeedMotorL, errorSpeedMotorR, errorReadDistance;

	 /* check the input params */
		if ((direction != FORWARD) && (direction != REVERSE) ){
			return 1;
		}

		if((speedRobot <0 )||( speedRobot > 100)){
			return 1;
		}

		if(set_steps >= 100 ){
				return 1;
			}
		if(set_steps >=12 ){
			return 1;
		}

	//if the direction is reverse
	if (direction == REVERSE ){speedRobot = 0-speedRobot;}
	
    /* run the motor*/
	errorSpeedMotorL = Speed_motor(speedRobot,LEFT);
	errorSpeedMotorR = Speed_motor(speedRobot+CORRECTION*direction,RIGHT);
	
    /* read and calcurate the number of turns and steps must reach and store in distance_to_stop */
		errorReadDistance = Read_distance(LEFT, &distance);
		distance_to_stop.steps = set_steps + distance.steps;
		distance_to_stop.turns = set_turns + distance.turns;

	/* check the number of steps are correcte */
	if(distance_to_stop.steps >= 12){
		distance_to_stop.steps = distance_to_stop.steps % 12;
		distance_to_stop.turns ++;
	}
	return 0;
}

//M5
uint8_t nb_spin_steps(int8_t spinDirection, uint8_t speedRobot, uint16_t set_turns, uint8_t set_steps)
{
	distance_type distance;
	uint8_t errorSpeedMotorL, errorSpeedMotorR, errorReadDistance, errorSpeedMotor;

	/* check the input params */
	if (checkParameters(spinDirection,speedRobot,set_turns,set_steps)){
	return 0;
	}
	
    /* run the motor*/
	if(!spin(spinDirection, speedRobot)){
		return 0;
	}
	
    /* read and calcurate the number of turns and steps must reach and store in distance_to_stop */
	errorReadDistance = Read_distance(LEFT, &distance);
	distance_to_stop.steps = set_steps + distance.steps;
	distance_to_stop.turns = set_turns + distance.turns;

	/* check the number of steps are correcte */
	if(distance_to_stop.steps >= 12){
		distance_to_stop.steps = distance_to_stop.steps % 12;
		distance_to_stop.turns ++;
	}
	return 0;
}

/*
 * check if the robot has reached the turns and steps to stop using the values stored in distance_to_stop
 * return
 *  0- robot arrived
 * 	1- robot is not arrived
 */
uint8_t check_stop_steps(){
	distance_type distance;
	uint8_t errorSpeedMotor1,errorSpeedMotor2;
	
	Read_distance(LEFT, &distance);
	// show_number (distance_to_stop.turns *100 + distance_to_stop.steps);
	show_number (distance.turns *100 + distance.steps);

	if((distance_to_stop.turns <= distance.turns) && (distance_to_stop.steps <= distance.steps)){
		//stop the robot
		errorSpeedMotor1 = Speed_motor(0,RIGHT);
		errorSpeedMotor2 = Speed_motor(0,LEFT);
		return 0;
	}else{
		return 1;
	}
}



uint8_t nb_spin_bearing (int8_t spinDirection, uint8_t speedRobot, uint16_t target_bearing){
	
	uint16_t delta_bearing = 50;
	up_bearing = new_bearing(target_bearing, delta_bearing);
	low_bearing = new_bearing(target_bearing, -delta_bearing);
	spin(spinDirection, speedRobot);
}

uint8_t check_stop_bearing(){
	
	uint16_t actual_bearing;
	uint8_t errorSpeedMotor1,errorSpeedMotor2;
	uint8_t errorCompass;
	
	errorCompass = Read_compass_16(&actual_bearing);
	show_number(actual_bearing);

	if ((actual_bearing < up_bearing) && (actual_bearing > low_bearing ) ){
		//STOP both wheels
		errorSpeedMotor1 = Speed_motor(0,RIGHT);
		errorSpeedMotor2 = Speed_motor(0,LEFT);
		return 0;
	}else{
		return 1;
	}
	
}
