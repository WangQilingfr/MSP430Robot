/*
 * bearing.c
 *
 *  Created on: 20 mars 2018
 *      Author: wen.yang.13
 */
#include<msp430g2553.h>
#include<stdint.h>
#include "bearing.h"

//Compass bearing  as word, reading two times
//return -1 if error
//return value is between 0 and 3599
uint16_t new_bearing (uint16_t bearing,int16_t change){

	uint16_t sum;

	//check the bearing input
	if((bearing < 0)||(bearing > 3600 )){
		return -1;
	}
	//check the change input
	if((change <= -1800) || (change >1800) ){
		return -1;
	}

	sum = bearing + change;

	// correct the ouput value
	if ((sum< 0)&& (sum > -1800)){
		sum += 3600 ;
	}else if((sum >= 3600)){
		sum -= 3600 ;
	}

	return sum;
}



