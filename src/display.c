/*
 * display.c
 *
 *  Created on: Nov 7, 2018
 *      Author: zachwalters
 */

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "display.h"

int ball_last_x = -1;
int ball_last_y = -1;
int paddle1_last_y = -1;
int paddle2_last_y = -1;

/*
 *  PB0-PB12 as GPIO outputs
 *  PB0-B7 connected to DISPLAY DB0-DB7
 *  PB8 connected to DISPLAY Chip Select 2 (CS2)
 *  PB9 connected to DISPLAY Chip Select 1 (CS1)
 *  PB10 connected to DISPLAY Reset signal (RES)
 *  PB11 connected to DISPLAY instruction code (D/I)
 *  PB12 connected to DISPLAY Enable (E)
*/


void display_off(){
    GPIOB->BRR = EN; // enable off
    GPIOB->BRR |= RS; // set D/I (RS) off
    nano_wait(500);
    GPIOB->BSRR = EN | 0x3e; // enable on and set data
    nano_wait(8000);
    GPIOB->BRR = EN; // enable off
    nano_wait(1000000000);
    GPIOB->BRR = RS | 0x3e; // set D/I (RS) and data off
    nano_wait(500);
    GPIOB->BSRR = EN; // set enable on
    nano_wait(500);

//    GPIOB->BSRR |= 0x3e;
//    GPIOB->BSRR = RS << 16;
//    GPIOB->BSRR |= (0xC1 << 16);
//    GPIOB->BSRR |= (0x1000);
//    nano_wait(100000);
//    GPIOB->BSRR |= (0x1000 << 16);
}

void display_on(){
    GPIOB->BRR = EN; // enable off
    GPIOB->BRR = RS; // set D/I (RS) off
    nano_wait(500);
    GPIOB->BSRR = EN | 0x3f; // enable on and set data
    nano_wait(8000);
    GPIOB->BRR = EN; // enable off
    nano_wait(100000000);
    GPIOB->BRR = RS | 0x3f; // set D/I (RS) and data off
    nano_wait(500);
    GPIOB->BSRR = EN; // set enable on
    nano_wait(500);
    //while(1){
        //set_col_addr(0);
        //set_display_start(0);
        //nano_wait(10000);
        //write_display();
    //}
//    GPIOB->BSRR |= 0x3f;
//    GPIOB->BSRR = RS << 16;
//    //GPIOB->BSRR |= (0xC0 << 16);
//    GPIOB->BSRR |= (0x1000);
//    nano_wait(8000);
//    GPIOB->BSRR |= (0x1000 << 16);
}

void set_col_addr(int x){
    // x addr must b 5 bit;
    if (x >  63){
        return;
    }
    GPIOB->BRR = EN; // enable off
    GPIOB->BRR = RS; // set D/I (RS) on/off
    nano_wait(500);
    GPIOB->BSRR = EN | 0x40; // enable on and set data
    GPIOB->BSRR = x;
    nano_wait(10000);
    GPIOB->BRR = EN; // enable off
    nano_wait(15000);
    GPIOB->BRR = RS | 0xff; // set D/I (RS) and data off
    nano_wait(500);
    GPIOB->BSRR = EN; // set enable on
    nano_wait(500);

//    GPIOB->BSRR |= x;
//    GPIOB->BSRR |= 0x40;
//    GPIOB->BSRR = RS << 16;
//    GPIOB->BSRR |= (0x1000);
//    nano_wait(100000);
//    GPIOB->BSRR |= (0x1000 << 16);
}

void set_display_start(int x){
    if (x > 63){
        return;
    }
    GPIOB->BRR = EN; // enable off
    GPIOB->BRR = RS; // set D/I (RS) on/off
    nano_wait(500);
    GPIOB->BSRR = EN | 0xc0; // enable on and set data
    GPIOB->BSRR = x;
    nano_wait(10000);
    GPIOB->BRR = EN; // enable off
    nano_wait(15000);
    GPIOB->BRR = RS | 0xff; // set D/I (RS) and data off
    nano_wait(500);
    GPIOB->BSRR = EN; // set enable on
    nano_wait(500);

//    GPIOB->BSRR |= x;
//    GPIOB->BSRR |= 0xc0;
//    GPIOB->BSRR = RS << 16;
//    GPIOB->BSRR = EN;
//    nano_wait(100000);
//    GPIOB->BRR = EN;
}

void set_row_addr(int x){
    if (x > 7){
        return;
    }
    GPIOB->BRR = EN; // enable off
    GPIOB->BRR = RS; // set D/I (RS) on/off
    nano_wait(500);
    GPIOB->BSRR = EN | 0xb8; // enable on and set data
    GPIOB->BSRR = x;
    nano_wait(10000);
    GPIOB->BRR = EN; // enable off
    nano_wait(15000);
    GPIOB->BRR = RS | 0xff; // set D/I (RS) and data off
    nano_wait(500);
    GPIOB->BSRR = EN; // set enable on
    nano_wait(500);

//    GPIOB->BSRR |= x;
//    GPIOB->BSRR |= 0x38;
//    GPIOB->BSRR = RS << 16;
//    GPIOB->BSRR |= (0x1000);
//    nano_wait(100000);
//    GPIOB->BSRR |= (0x1000 << 16);
}

void write_display(int x){
    // write to display
    GPIOB->BRR = EN; // enable off
    GPIOB->BSRR = RS; // set D/I (RS) on/off
    nano_wait(500);
    GPIOB->BSRR = EN; // enable on and set data
    x &= 0xff;
    GPIOB->BSRR = x;
    //GPIOB->BRR = 0xff;
    nano_wait(10000);
    GPIOB->BRR = EN; // enable off
    nano_wait(15000);
    GPIOB->BRR = RS | 0xff; // set D/I (RS) and data off
    nano_wait(500);
    GPIOB->BSRR = EN; // set enable on
    nano_wait(500);


//    GPIOB->BSRR |= (0x800);
//    GPIOB->BSRR |= 0xff;
//    GPIOB->BSRR |= (0x1000);
//    nano_wait(8000);
//    GPIOB->BSRR |= (0x1000 << 16);
}

void write_alot(void){
    int x;
    int bits = 0xff;
    for(x = 0; x < 64;x++){
        write_display(bits);
        bits = bits << 1;
        if (bits == 0xff00){
            bits = 0xff;
        }
    }
}

void clear_screen(void){
    int y;
    int x;
    for (x = 0;x < 8; x++){
        set_row_addr(x);
        set_col_addr(0);
        for(y = 0;y < 64; y++){
            write_display(0x0);
        }
    }
}

// Zach
void initialize_display(){
    set_side(1);
    display_on();
    set_display_start(0);
    set_side(2);
    display_on();
    set_display_start(0);
    clear_screen();
    set_side(1);
    clear_screen();
    // add dotted line down center of display
    // every other "page"
//    int x;
//    for(x = 0;x < 8;x++){
//        set_side(1);
//        set_row_addr(x);
//        set_col_addr(62);
//        write_display(0xf);
//        write_display(0xf);
//        set_side(2);
//        set_row_addr(x);
//        set_col_addr(0);
//        write_display(0xf);
//        write_display(0xf);
//    }
//    set_display_start(0);
}

void get_ball_bits(int x, int * upper_bits, int * lower_bits, int * full_bits){
    if ((x % 8) >  (8 - BALL_S)){
        // if you must set bits in more than row
        *upper_bits = 0xff << (x % 8);
        *lower_bits = 0xff >> (8 - (x % 8 - BALL_S));
        *full_bits = 0x55;
    }else{
       *full_bits = 0xff >> (8 - BALL_S);
       *full_bits = *full_bits << x % 8;
       *lower_bits = 0x55;
       *upper_bits = 0x55;
    }
}

void delete_old_ball(int x, int y){
    int upper_bits;
    int lower_bits;
    int full_bits;
    int row;
    int i;

    if (x > DISP_S - BALL_S){
        x = DISP_S - BALL_S;
    }else if(x < 0){
        x = 0;
    }
    if (y < 0){
        y = 0;
    }else if(y > DISP_W - BALL_S){
        y = DISP_W - BALL_S;
    }

    row = x / 8;
    //row = x / DISP_R;
    if(y > DISP_S - 1){
        set_side(2);
        set_col_addr(y - DISP_S);
        get_ball_bits(x, &upper_bits, &lower_bits, &full_bits);
        if (full_bits == 0x55){
            set_row_addr(row);
            for (i = 0; i < BALL_S;i++){
                write_display(0x00);
            }
            set_col_addr(y - DISP_S);
            set_row_addr(row+1);
            for(i = 0; i < BALL_S;i++){
                write_display(0x00);
            }
        }else{
           set_row_addr(row);
           for(i = 0; i < BALL_S;i++){
               write_display(0x00);
           }
        }
    }else{
        get_ball_bits(x, &upper_bits, &lower_bits, &full_bits);
        if (y + BALL_S > DISP_S){
            // Select CS1 set bits
            if (full_bits == 0x55){
                set_side(1);
                set_row_addr(row);
                set_col_addr(y);
                // write upper bits
                for(i = y; i < (y + BALL_S);i++){
                    if (i == DISP_S - 1){
                        // Select CS2 set bits
                        write_display(0x00);
                        set_side(2);
                        set_col_addr(0);
                        set_row_addr(row);
                    }else{
                        write_display(0x00);
                    }
                }
                set_side(1);
                set_row_addr(row + 1);
                set_col_addr(y);
                // write lower bits
                for(i = y; i < (y + BALL_S);i++){
                    if (i == DISP_S - 1){
                        // Select CS2 set bits
                        write_display(0x00);
                        set_side(2);
                        set_col_addr(0);
                        set_row_addr(row + 1);
                    }else{
                        write_display(0x00);
                    }
                }
            }else{
                set_side(1);
                set_row_addr(row);
                set_col_addr(y);
                for(i = y;i < (y+BALL_S);i++){
                    if (i == DISP_S - 1){
                        // Select CS2 set bits
                        write_display(0x00);
                        set_side(2);
                        set_row_addr(row);
                        set_col_addr(0);
                    }
                    write_display(0x00);
                }
            }

        }else{
            // you need to write to left screen and not to right.
            set_side(1);
            if (full_bits == 0x55){
                set_row_addr(row);
                set_col_addr(y);
                for(i = 0;i < BALL_S;i++){
                    write_display(0x00);
                }
                set_row_addr(row+1);
                set_col_addr(y);
                for(i = 0;i < BALL_S;i++){
                    write_display(0x00);
                }
            }else{
                set_row_addr(row);
                set_col_addr(y);
                for(i = 0;i < BALL_S;i++){
                    write_display(0x00);
                }
            }
        }
    }
}

// Zach
void display_ball(int x, int y){
    // turn off bits of last position
    // pos is relative to top left corner
    int row;
    int upper_bits;
    int lower_bits;
    int full_bits;
    int i;
    if (x > DISP_S - BALL_S){
        x = DISP_S - BALL_S;
    }else if(x < 0){
        x = 0;
    }
    if (y < 0){
        y = 0;
    }else if(y > DISP_W - BALL_S){
        y = DISP_W - BALL_S;
    }

    row = x / 8;
    if (ball_last_x != -1 && ball_last_y != -1){
        delete_old_ball(ball_last_x, ball_last_y);
    }

    // turn on bits of new position
    if(y >= DISP_S){
        set_side(2);
        set_col_addr(y - DISP_S);
        get_ball_bits(x, &upper_bits, &lower_bits, &full_bits);
        if (full_bits == 0x55){
            set_row_addr(row);
            for (i = 0; i < BALL_S;i++){
                write_display(upper_bits);
            }
            set_row_addr(row+1);
            set_col_addr(y - DISP_S);
            for(i = 0; i < BALL_S;i++){
                write_display(lower_bits);
            }
        }else{
           set_row_addr(row);
           for(i = 0; i < BALL_S;i++){
               write_display(full_bits);
           }
        }
    }else{
        get_ball_bits(x, &upper_bits, &lower_bits, &full_bits);
        if (y + BALL_S > DISP_S){
            // Select CS1 set bits
            if (full_bits == 0x55){
                set_side(1);
                set_row_addr(row);
                set_col_addr(y);
                // write upper bits
                for(i = y; i < (y + BALL_S);i++){
                    if (i == DISP_S - 1){
                        // Select CS2 set bits
                        write_display(upper_bits);
                        set_side(2);
                        set_col_addr(0);
                        set_row_addr(row);
                    }else{
                        write_display(upper_bits);
                    }
                }
                set_side(1);
                set_row_addr(row + 1);
                set_col_addr(y);
                // write lower bits
                for(i = y; i < (y + BALL_S);i++){
                    if (i == DISP_S - 1){
                        // Select CS2 set bits
                        write_display(lower_bits);
                        set_side(2);
                        set_col_addr(0);
                        set_row_addr(row + 1);
                    }else{
                        write_display(lower_bits);
                    }
                }
            }else{
                set_side(1);
                set_row_addr(row);
                set_col_addr(y);
                for(i = y;i < (y+BALL_S);i++){
                    if (i == DISP_S - 1){
                        // Select CS2 set bits
                        write_display(full_bits);
                        set_side(2);
                        set_row_addr(row);
                        set_col_addr(0);
                    }
                    write_display(full_bits);
                }
            }
        }else{
            // you need to write to left screen and not to right.
            set_side(1);
            if (full_bits == 0x55){
                set_row_addr(row);
                set_col_addr(y);
                for(i = 0;i < BALL_S;i++){
                    write_display(upper_bits);
                }
                set_row_addr(row+1);
                set_col_addr(y);
                for(i = 0;i < BALL_S;i++){
                    write_display(lower_bits);
                }
            }else{
                set_row_addr(row);
                set_col_addr(y);
                for(i = 0;i < BALL_S;i++){
                    write_display(full_bits);
                }
            }
        }
    }
    ball_last_x = x;
    ball_last_y = y;
    // assign new last_x and last_y
}

void display_test(){
    int x;
    int y;
    y = 72;
    for(x = 5; x < 64 - BALL_S;x += BALL_S){
       // for(y = BALL_S;y < 128 - BALL_S;y += BALL_S){
            display_ball(x, y);
            nano_wait(100000000);
            y -= BALL_S;
       // }
    }
//    display_ball(5, 67);
//    nano_wait(100000000);
//    display_ball(5, 63);
//    nano_wait(100000000);
//    display_ball(5, 59);
}

void test_line(int x){
    set_row_addr(x);
    set_col_addr(0);
    int i;
    for(i = 0;i < 64;i++){
        write_display(0xff);
    }
    //GPIOB->BSRR = CS1 | CS2;
}

void set_side(int x){
    nano_wait(2000);
    if (x == 1){
        GPIOB->BRR = CS2;
        GPIOB->BSRR = CS1;
    }else if(x == 2){
        GPIOB->BRR = CS1;
        GPIOB->BSRR = CS2;
    }else{
        GPIOB->BSRR = CS1;
        GPIOB->BSRR = CS2;
    }
}


void display_pong1(int y){
	set_side(1);
	set_col_addr(PADDLE_BUFF);
	int rem = y % 8;
	int rem2 = 0;
	int rem3 = 0;
	int x = 0;

	//turn off old bits
	if (paddle1_last_y >= 0){
		if (paddle1_last_y < 8){
			rem = paddle1_last_y;
		}
		if (paddle1_last_y > 56){
				paddle1_last_y = 48;
		}
		set_row_addr(paddle1_last_y / 8); //initial page of paddle
		for (x = 0; x < PADDLE_W;  x++){
			set_col_addr(PADDLE_BUFF + x);
			write_display(0x0);
		}
		rem2 = 16 - (8 - rem);
		set_row_addr((paddle1_last_y / 8) + 1); //second page of paddle
		for (x = 0; x < PADDLE_W;  x++){
			set_col_addr(PADDLE_BUFF + x);
			write_display(0x0); //will always write 8 bits to second called page for paddle
		}
		if (rem2 > 8){ //check to verify a 3rd page needed
			set_row_addr((paddle1_last_y / 8) + 2);
			rem3 = 16 - rem2;
			for (x = 0; x < PADDLE_W;  x++){
				set_col_addr(PADDLE_BUFF + x);
				write_display(0x0);
			}
		}
	}

	//turn on new bits;
	rem = y % 8;
	if (y < 8){
		rem = y;
	}
	if (y > 48){
		y = 48;
		rem = 0;
	}
	set_row_addr(y / 8); //initial page of paddle
	for (x = 0; x < PADDLE_W;  x++){
		set_col_addr(PADDLE_BUFF + x);
		write_display(0xff << rem);
	}
	rem2 = 16 - (8 - rem);
	set_row_addr((y / 8) + 1); //second page of paddle
	for (x = 0; x < PADDLE_W;  x++){
		set_col_addr(PADDLE_BUFF + x);
		write_display(0xff); //will always write 8 bits to second called page for paddle
	}
	if (rem2 > 8){ //check to verify a 3rd page needed
		set_row_addr((y / 8) + 2);
		rem3 = 16 - rem2;
		for (x = 0; x < PADDLE_W;  x++){
			set_col_addr(PADDLE_BUFF + x);
			write_display(0xff >> (rem3));
		}
	}
	//update last position
	paddle1_last_y = y;
	}

void display_pong2(int y){
	set_side(2);
	set_col_addr(DISP_S - PADDLE_BUFF);
	int rem = y % 8;
	int rem2 = 0;
	int rem3 = 0;
	int x = 0;

	//turn off old bits
	if (paddle2_last_y >= 0){
		if (paddle2_last_y > 8){
			rem = paddle2_last_y;
		}
		if (paddle2_last_y > 56){
			paddle2_last_y = 48;
		}
		set_row_addr(paddle2_last_y / 8); //initial page of paddle
		for (x = 0; x < PADDLE_W;  x++){
			set_col_addr(DISP_S - (PADDLE_BUFF) - x);
			write_display(0x0);
		}
		rem2 = 16 - (8 - rem);
		set_row_addr((paddle2_last_y / 8) + 1); //second page of paddle
		for (x = 0; x < PADDLE_W;  x++){
			set_col_addr(DISP_S - (PADDLE_BUFF) - x);
			write_display(0x0);
		}
		if (rem2 > 8){ //check to verifpaddle2_last_y a 3rd page needed
			set_row_addr((paddle2_last_y / 8) + 2);
			rem3 = 16 - rem2;
			for (x = 0; x < PADDLE_W;  x++){
				set_col_addr(DISP_S - (PADDLE_BUFF) - x);
				write_display(0x0);
			}
		}
	}
	//turn on new bits;
	rem = y % 8;
	if (y < 8){
		rem = y;
	}
	if (y > 48){
		y = 48;
		rem = 0;
	}
	set_row_addr(y / 8); //initial page of paddle
	for (x = 0; x < PADDLE_W;  x++){
		set_col_addr(64 - PADDLE_BUFF - x);
		write_display(0xff << rem);
	}
	rem2 = 16 - (8 - rem);
	set_row_addr((y / 8) + 1); //second page of paddle
	for (x = 0; x < PADDLE_W;  x++){
		set_col_addr(64 - PADDLE_BUFF - x);
		write_display(0xff); //will always write 8 bits to second called page for paddle
	}
	if (rem2 > 8){ //check to verify a 3rd page needed
		set_row_addr((y / 8) + 2);
		rem3 = 16 - rem2;
		for (x = 0; x < PADDLE_W;  x++){
			set_col_addr(64 - PADDLE_BUFF - x);
			write_display(0xff >> (rem3));
		}
	}
	//update last position
	paddle2_last_y = y;
}

void display_score(int score1, int score2){
    char line[16];
    sprintf(line, "P1 score: %d", score1);
    display1(line);
    sprintf(line, "P2 score: %d", score2);
    display2(line);
}
void display_start_screen(){
    char line[16];
    sprintf(line, "SLIDE RIGHT");
    display1(line);
}
void display_winner(int winner){
    char line[16];
    sprintf(line, "P%d is the winner!", winner);
    display1(line);
    sprintf(line, "Slide to start", winner);
    display2(line);
}


//void (int);
//void display_pong2(int);

//void display_score(int, int);
//void display_start_screen();
//void display_winner(int);
