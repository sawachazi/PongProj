/*
 * display.h
 *
 *  Created on: Nov 7, 2018
 *      Author: zachwalters
 */

/*
 *  PB0-PB12 as GPIO outputs
 *  PB0-B7 connected to DISPLAY DB0-DB7
 *  PB8 connected to DISPLAY Chip Select 2 (CS2)
 *  PB9 connected to DISPLAY Chip Select 1 (CS1)
 *  PB10 connected to DISPLAY Reset signal (RES)
 *  PB11 connected to DISPLAY instruction code (D/I or RS)
 *  PB12 connected to DISPLAY Enable (E)
*/

#ifndef DISPLAY_H_
#define DISPLAY_H_

#define EN 0x1000
#define RS 0x800
#define CS2 0x100
#define CS1 0x200


void start_display();
void write_display(int);
void display_off();
void display_on();
void set_y_addr(int);
void set_display_start(int);
void set_x_addr(int);
void write_alot();


void clear_screen(void);
void initialize_display(void);

void display_ball(int, int);
void display_pong1(int);
void display_pong2(int);
void display_score(int, int);
void display_start_screen();
void display_winner(int);
void display_start_wait();

#endif /* DISPLAY_H_ */
