/*
 * blocking_movements.c
 *
 *  Created on: 7 mars 2018
 *      Author: wen.yang.13
 */


#include<msp430g2553.h>
#include<stdint.h>

#include "control_motors.h"
#include "control_encoders.h"
#include "system.h"
#include "blocking_movements.h"
#include "display.h"
#include "bearing.h"

uint8_t checkParameters(int8_t direction, uint8_t speedRobot, uint16_t set_turns, uint8_t set_steps){
	/*
    * check the input params
    */
	if ((direction != LEFT) && (direction != RIGHT) ){
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
	return 0;
}

//M5
uint8_t spin(int8_t spinDirection, uint8_t speedRobot){
	uint8_t errorSpeedMotorL, errorSpeedMotorR;
	if(spinDirection == RIGHT){
		errorSpeedMotorL = Speed_motor(speedRobot,LEFT);//left wheel to turn to 20%
		errorSpeedMotorR = Speed_motor(-(speedRobot+CORRECTION),RIGHT);//right wheel to turn to 20%
	}else{
		errorSpeedMotorL = Speed_motor(-speedRobot,LEFT);//left wheel to turn to 20%
		errorSpeedMotorR = Speed_motor(speedRobot+CORRECTION,RIGHT);//right wheel to turn to 20%
	}
	if( (errorSpeedMotorL) && (errorSpeedMotorR) ){
		return 0;
	}else{
		return 1;
	}
}

/*
* straight_move.c
* go straight for n turns and m steps
* parameters :
* > int8_t direction: direction of mouvement
* > uint8_t speedRobot: 0-100
* > uint16_t set_turns : set turns that the robot must move
* > uint8_t set_steps : set steps that the robot must move
*/

//M8
uint8_t spin_bearing (int8_t spinDirection, uint8_t speedRobot, uint16_t target_bearing){
	uint16_t actual_bearing;
	uint16_t up_bearing;
	uint16_t low_bearing;
	uint16_t delta_bearing = 50;
	uint8_t errorCompass;
	uint16_t objet = 0;

	up_bearing = new_bearing(target_bearing, delta_bearing);
	low_bearing = new_bearing(target_bearing, -delta_bearing);

	// run the motor
	spin(spinDirection, speedRobot);

	// find the condition to stop
	while (!objet)
	{
		errorCompass = Read_compass_16(&actual_bearing);
		show_number(actual_bearing);//1600

		if ((actual_bearing < up_bearing) && (actual_bearing > low_bearing ) ){
			//STOP both wheels
			Speed_motor(0,LEFT);
			Speed_motor(0,RIGHT);
			objet = 1;
			return 1;
		}
	}
 }
//M8 end

uint8_t straight_move(int8_t direction, uint8_t speedRobot, uint16_t set_turns, uint8_t set_steps) {
	//variables
	uint8_t errorSpeedMotorL, errorSpeedMotorR, errorReadDistance;
	uint8_t max_steps;
	uint16_t max_turns;
	distance_type distance;
	int8_t speedMotor = speedRobot;
	
	/*
    * check the input params
    */
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
	if (direction == REVERSE ){speedMotor = 0-speedMotor;}

    /*
     * read the right side encoder
     */
	Clear_Distance(LEFT);
	Clear_Distance(RIGHT);
	errorReadDistance = Read_distance(RIGHT, &distance);
    max_steps = set_steps + distance.steps;
    max_turns = set_turns + distance.turns;

    if(max_steps >= 12)
    {
    	max_steps = max_steps % 12;
    	max_turns ++;
    }

    /*
    * Move left and right wheels
    */
	errorSpeedMotorL = Speed_motor(speedMotor,LEFT);//left wheel to turn to 20%
	errorSpeedMotorR = Speed_motor(speedMotor+CORRECTION*direction,RIGHT);//right wheel to turn to 20%

	while( distance.turns < max_turns )
	{
		Read_distance(LEFT, &distance);
		show_number (distance.turns *100);
	}
	show_byte(CLEAR_DISPLAY);
	while( distance.steps < max_steps )
	{
		Read_distance(LEFT, &distance);
		show_number (distance.turns *100 + distance.steps);
	}

	//STOP both wheels
	errorSpeedMotorL = Speed_motor(0,LEFT);
	errorSpeedMotorR = Speed_motor(0,RIGHT);
   return 1 ;
}

/*m4  rotation*/
uint8_t spin_steps(int8_t spinDirection, uint8_t speedRobot, uint16_t set_turns, uint8_t set_steps){
	uint8_t errorSpeedMotorL, errorSpeedMotorR, errorReadDistance;
	uint8_t max_steps;
	uint16_t max_turns;
	distance_type distance;

	if (checkParameters(spinDirection,speedRobot,set_turns,set_steps)){
		return 0;
	}
	
	Clear_Distance(LEFT);
	Clear_Distance(RIGHT);

	errorReadDistance = Read_distance(RIGHT, &distance);
	max_steps = set_steps + distance.steps;
	max_turns = set_turns + distance.turns;

	if(max_steps >= 12)
	{
		max_steps = max_steps % 12;
		max_turns ++;
	}

	if(spinDirection == LEFT){
		errorSpeedMotorL = Speed_motor(speedRobot,LEFT);//left wheel to turn to 20%
		errorSpeedMotorR = Speed_motor(-(speedRobot+CORRECTION),RIGHT);//right wheel to turn to 20%
	}else{
		errorSpeedMotorL = Speed_motor(-speedRobot,LEFT);//left wheel to turn to 20%
		errorSpeedMotorR = Speed_motor(speedRobot+CORRECTION,RIGHT);//right wheel to turn to 20%
	}
	
	while( distance.turns < max_turns )
	{
		Read_distance(LEFT, &distance);
		show_number (distance.turns *100);
	}
	show_byte(CLEAR_DISPLAY);
	while( distance.steps < max_steps )
	{
		Read_distance(LEFT, &distance);
		show_number (distance.turns *100 + distance.steps);
	}

	//STOP both wheels
	errorSpeedMotorL = Speed_motor(0,LEFT);
	errorSpeedMotorR = Speed_motor(0,RIGHT);
	
	show_byte(CLEAR_DISPLAY);
	show_number (0000);

		return 1;
}
