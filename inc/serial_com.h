/*
 * serial_com.h
 *
 *  Created on: Nov 14, 2018
 *      Author: zachwalters
 */

#ifndef SERIAL_COM_H_
#define SERIAL_COM_H_

#include "joy_stick.h"
#include "display.h"
#include <stdint.h>

void init_usart2(void);
void action(char **words, uint16_t * controllers);
void testbench(uint16_t * controllers);


#endif /* SERIAL_COM_H_ */
