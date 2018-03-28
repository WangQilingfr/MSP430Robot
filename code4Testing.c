#include <msp430g2553.h>
#include <stdint.h>

#include "display.h"
#include "control_motors.h"
#include "control_encoders.h"
#include "compass.h"
#include "system.h"



/*
 * EX1
 * test the display counting from 0 to 255
 * 
 */
static void exercice1( void ){
	int counter = 255;
	init_display ();
	show_byte(CLEAR_DISPLAY);

	for (counter = 0 ; counter < 256 ; counter ++){
		show_number (counter);
		__delay_cycles(2400000);
		if(counter == 255){
			counter = 0;
		}
	}
}

/*
 * EX2
 * test the motors
 * 
 */
static void exercice2( void ){
	int speedTab[3] = {20,50,100};
	int index;
	volatile int error;
	__enable_interrupt();
	Init_motors ();
	for (index = 0 ; index < 3 ; index ++){
		error = Speed_motor(speedTab[index],LEFT);//left wheel
	    error |= Speed_motor(speedTab[index],RIGHT);//right wheel
//		error = Speed_motor(150,RIGHT);//the case that the return value is 1 means that an error occurs
		__delay_cycles(1600000);
		if(index == 2){
			index = -1;
		}
	}
	emergency_stop();
}

/*
 * EX3_4
 * check the compass
 * 
 */
static void exercice3_4( void ){
   distance_type distance;
	volatile int errorReadDistance;
	volatile int errorSpeedMotor;

	Init_motors ();
	init_display ();
	show_byte(CLEAR_DISPLAY);
	Init_encoders_distance();
	Clear_Distance(LEFT);
	__enable_interrupt();

	while (1){
		//right wheel
		errorSpeedMotor = Speed_motor(20,RIGHT);//right wheel to turn to 20%
		errorReadDistance = Read_distance(RIGHT, &distance); // read the right side encoder

		//left wheel
		errorSpeedMotor = Speed_motor(20,LEFT);//left wheel to turn to 20%
		errorReadDistance = Read_distance(LEFT, &distance); // read the left side encoder

		__delay_cycles(16000);
		show_number (distance.turns);

	}
}


/*
 * EX5
 * check the compass : LOW
 * 
 */
static void exercice5( void ){
   uint8_t compassVal;
	volatile int errorCompass;
	init_display();
	show_byte(CLEAR_DISPLAY);
	errorCompass = compass_init();

	while(errorCompass == 0){
		compassVal = Read_compass_8();
		show_number(compassVal);
	}
}

/*
 * EX6
 * check the compass with the time out function: HIGH
 * 
 */
static void exercice6( void ){
    uint8_t *compassVal;
	volatile int errorCompass;

	init_display();
	show_byte(CLEAR_DISPLAY);
	errorCompass = compass_init();
	__enable_interrupt();

	while(errorCompass == 0){
		errorCompass = Read_compass_16(&compassVal);
		show_number(compassVal);
	}
}

static void exerciceM1( void ){
	uint8_t err;

    Clock_graceInit_DCO_12M();
	
    init_ALL();
	err  = straight_move(FORWARD, 20 ,10, 5 ); //direction F:foward, R:Reverse; speed; set_turns; set_steps
	err |= straight_move(REVERSE, 20 ,10, 5 ); //direction F:foward, R:Reverse; speed; set_turns; set_steps
}


static void init_ALL(void){

	init_display ();
	show_byte(CLEAR_DISPLAY);
	Clear_Distance(LEFT);
	Clear_Distance(RIGHT);
	Init_motors();
	Init_encoders_distance();
    __enable_interrupt();
}


/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    Clock_graceInit_DCO_12M();
	exercice1();
	return 0;
}


/*
 * tests for check_stop_steps in non_blocking_movements
 */
    do{
		Read_distance(LEFT, &distance);
		distance_to_stop.turns = 2 - distance.turns;
		distance_to_stop.steps = 0;
		show_number(distance_to_stop.turns*100 + distance_to_stop.steps);
    }while(check_stop_steps(distance_to_stop));
    show_number("5225");

/*
 * M2
 * tests for nb_straight_move & check_stop_steps in non_blocking_movements
 */	
	nb_straight_move(FORWARD,20,2,0);
	
	
    do{
    }while(check_stop_steps());


	return 0;

/*
exe M2
*/
#include "scheduler.h"
#include <msp430g2553.h>
#include <stdint.h>

#include "display.h"
#include "control_motors.h"
#include "control_encoders.h"
#include "compass.h"
#include "system.h"
#include "blocking_movements.h"
#include "non_blocking_movements.h"
extern uint16_t Tick_out;

void init_ALL(void);


int main(void) {
	uint8_t err, in_movement = STOP;
	int8_t direction[3] = {FORWARD,REVERSE,FORWARD};
	int8_t mouvement_nb;
	
	distance_type distance;
	distance_type distance_to_stop;

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	Clock_graceInit_DCO_12M();// don't add this for motors !!!!!

	init_ALL();

	mouvement_nb = 0;
	
	while(1){
		if(in_movement == STOP){
			switch (mouvement_nb){
				case 0:
					nb_straight_move(FORWARD, 20, 4, 0);
					in_movement = MOVE;
					mouvement_nb++;
					break;
				case 1:
					nb_straight_move(REVERSE, 50, 3, 0);
					in_movement = MOVE;
					mouvement_nb++;
					break;
				case 2:
					nb_straight_move(FORWARD, 80, 5, 0);
					in_movement = MOVE;
					mouvement_nb++;
					break;
				default:
					mouvement_nb = 0;
					break;
			}
		}
		// if robot is moving
		else {
			if(Tick_out){
				// check if the robot must stop
				if (check_stop_steps() == 0){
				   in_movement = STOP;
				}
				Tick_out=0;
			}
		}
	show_number(mouvement_nb);
	}
	return 0;
}

void init_ALL(){

	init_display ();
	show_byte(CLEAR_DISPLAY);
	Clear_Distance(LEFT);
	Clear_Distance(RIGHT);
	Init_motors();
	Init_encoders_distance();
    __enable_interrupt();
}


/*M3 
adding avoiding race condition
*/
#include "scheduler.h"
#include <msp430g2553.h>
#include <stdint.h>

#include "display.h"
#include "control_motors.h"
#include "control_encoders.h"
#include "compass.h"
#include "system.h"
#include "blocking_movements.h"
#include "non_blocking_movements.h"
extern uint16_t Tick_out;

void init_ALL(void);

int main(void) {
	uint8_t err, in_movement = STOP;
	int8_t direction[3] = {FORWARD,REVERSE,FORWARD};
	int8_t mouvement_nb;
	
	distance_type distance;
	distance_type distance_to_stop;

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	Clock_graceInit_DCO_12M();// don't add this for motors !!!!!

	init_ALL();

	mouvement_nb = 0;
	
	while(1){
		if(in_movement == STOP){
			switch (mouvement_nb){
				case 0:
					nb_straight_move(FORWARD, 20, 4, 0);
					in_movement = MOVE;
					mouvement_nb++;
					break;
				case 1:
					nb_straight_move(REVERSE, 50, 3, 0);
					in_movement = MOVE;
					mouvement_nb++;
					break;
				case 2:
					nb_straight_move(FORWARD, 80, 5, 0);
					in_movement = MOVE;
					mouvement_nb++;
					break;
				default:
					mouvement_nb = 0;
					break;
			}
		}
		// if robot is moving
		else {
			if(check_and_clear_Tick_out()){
				// check if the robot must stop
				if (check_stop_steps() == 0){
				   in_movement = STOP;
				}
			}
		}
	show_number(mouvement_nb);
	}
	return 0;
}

void init_ALL(){

	init_display ();
	show_byte(CLEAR_DISPLAY);
	Clear_Distance(LEFT);
	Clear_Distance(RIGHT);
	Init_motors();
	Init_encoders_distance();
    __enable_interrupt();
}
