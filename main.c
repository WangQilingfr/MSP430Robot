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
#include "bearing.h"

//extern uint16_t Tick_out; //M2

void init_ALL(void);

//exe M2
int main(void) {
	uint8_t err, in_movement = STOP;
	int8_t direction[3] = {FORWARD,REVERSE,FORWARD};
	int8_t mouvement_nb;
	uint16_t target_bearing;
	uint16_t target_bearing2;
	uint16_t actual_bearing;
	uint8_t errorCompass = 0;
	
	distance_type distance;
	distance_type distance_to_stop;

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	Clock_graceInit_DCO_12M();// don't add this for motors !!!!!

	init_ALL();
	mouvement_nb = 0;

	
//	M4
//	spin_steps(LEFT, 25, 5, 0);

//	test M5
//	nb_spin_steps(LEFT, 30, 10, 0);
//    do{
//    }while(check_stop_steps());
//	return 0;

// M6 TEST
//	target_bearing = new_bearing ( 920 , -900);
// if (target_bearing == -1)//error
//

//	 uint8_t errorCompass = 0;
//	 while(errorCompass == 0){
//			 errorCompass = Read_compass_16(&actual_bearing);
//			 target_bearing = new_bearing ( actual_bearing , -900);
//			 show_byte(CLEAR_DISPLAY);
//			 show_number(target_bearing);
//	 }


// M8
	// uint8_t error = 0;
	// target_bearing = 450 ;
	// target_bearing2 = 900 ;
	// do{
		// error = Read_compass_16(&actual_bearing);
	// }while (error);

	
	// target_bearing = new_bearing(actual_bearing,target_bearing);
	// target_bearing2 = new_bearing(actual_bearing,target_bearing2);

	// spin_bearing(RIGHT, 20, target_bearing);//M8
	// spin_bearing(LEFT, 30, target_bearing2);//M8
	// spin_bearing(RIGHT, 20, target_bearing);//M8
	
	/////////////////////////////
//	//M9
//	nb_spin_bearing(LEFT, 15, 450);
//	do{
//	}while(check_stop_bearing());
//	return 0;
/////////////////////////
	in_movement = STOP;
// Finite State Machine for M11
	while(1){
		do{
		errorCompass = Read_compass_16(&actual_bearing);
		}while (errorCompass);
		target_bearing = new_bearing ( actual_bearing , -900);
		target_bearing2 = new_bearing ( actual_bearing , 600);

		if(in_movement == STOP){
			switch (mouvement_nb){
				case 0:

						nb_spin_bearing(LEFT, 15, target_bearing);
//					nb_spin_steps(RIGHT, 40, 5, 0); //M5

//					nb_straight_move(FORWARD, 20, 4, 0);//M2
					in_movement = MOVE;
					mouvement_nb++;
					break;
				case 1:
					nb_spin_bearing(RIGHT, 15, target_bearing2);
//					nb_spin_bearing(LEFT, 15, target_bearing);
//					nb_spin_steps(LEFT, 40, 5, 0);//M5
					in_movement = MOVE;
					mouvement_nb++;
					break;
				case 2:
//					nb_spin_bearing(LEFT, 15, 900);
//					nb_spin_steps(RIGHT, 40, 5, 0);//M5
//					nb_straight_move(FORWARD, 25, 5, 0);//M2
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
				if (check_stop_bearing() == 0){
				   in_movement = STOP;
				}
			}
		}
	//show_number(mouvement_nb);
	}
	return 0;
}

void init_ALL(){

	init_display ();
	show_byte(CLEAR_DISPLAY);
	Clear_Distance(LEFT);
	Clear_Distance(RIGHT);
	Init_motors();
	compass_init();
	Init_encoders_distance();
    __enable_interrupt();
}
