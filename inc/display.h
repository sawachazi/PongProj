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


#define DISP_S 64 // display size
#define DISP_W 128 // display width
#define DISP_R 8 // display row size
#define BALL_S 4 // must stay under 8
#define PADDLE_BUFF 4 // buffer from edge of display
#define PADDLE_W 4
#define PADDLE_L 16

void write_display(int);
void display_off();
void display_on();
void set_col_addr(int);
void set_display_start(int);
void set_row_addr(int);
void write_alot();
void set_side(int);

void clear_screen(void);
void initialize_display(void);

void display_ball(int, int);
void display_pong1(int);
void display_pong2(int);
void display_score(int, int);
void display_start_screen();
void display_winner(int);
//void test_paddle();

#endif /* DISPLAY_H_ */
