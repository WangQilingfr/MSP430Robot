/*
 * scheduler.c
 *
 *  Created on: 29/1/2018
 *      Author: amarti
 */

#include "scheduler.h"
#include "msp430g2553.h"


// uint16_t Tick_out = 0; //Maybe this is no the final place to declare this variable
static uint16_t Tick_out = 0;

/* Called from TA_0 ISR*/
void callback_2ms(void) 
{

 Tick_out = 1;

}

uint8_t check_and_clear_Tick_out(void){

	// TACTL has 16 bits
	uint16_t store ;
	uint8_t ret ;

//	TAIE
//	0 Interrupt disabled
//	1 Interrupt enabled
//  store the value
	store = TACTL & TAIE;

	//clear disable interruption
	TACTL &= ~TAIE;

	//get the return value
	ret = Tick_out;

	//clear Tick_out;
	Tick_out=0;

	//restore the value
	TACTL |= store;

	return ret;

}





