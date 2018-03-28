/*
 * timeout.h
 *
 *  Created on: 23/2/2017
 *      Author: amarti
 */

#ifndef TIMEOUT_H_
#define TIMEOUT_H_

#include "msp430g2553.h"
#include <intrinsics.h>
#include <stdint.h>

 extern uint8_t TIME_OUT;

 void init_timeout (void);
 void start_timeout(void);
 void stop_timeout(void);

#endif /* TIMEOUT_H_ */
