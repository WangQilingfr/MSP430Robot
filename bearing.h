/*
 * Bearing.h
 *
 *  Created on: 20 mars 2018
 *      Author: wen.yang.13
 */

#ifndef BEARING_H_
#define BEARING_H_

#include <msp430g2553.h>
#include "stdint.h"


//Compass bearing  as word, reading two times
//return -1 if error
//return value is between 0 and 3599
uint16_t new_bearing (uint16_t bearing,int16_t change); //Compass bearing  as word, reading two times



#endif /* BEARING_H_ */
