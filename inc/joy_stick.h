/*
 * joy_stick.h
 *
 *  Created on: Nov 1, 2018
 *      Author: zachwalters
 */

#ifndef JOY_STICK_H_
#define JOY_STICK_H_
#include <stdint.h>

void calib_adc();
void run_adc(uint16_t* controllers);
void update_controllers(uint16_t *);

#endif /* JOY_STICK_H_ */
