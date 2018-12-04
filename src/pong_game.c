/*
 * pong_game.c
 *
 *  Created on: Nov 18, 2018
 *      Author: zachwalters
 */

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

#include <string.h>
#include <stdlib.h> // for strtoul()
#include <stdio.h>


#include "pong_game.h"
#include "display.h"
#include "joy_stick.h"

void update_padd_pos(Paddle*);
void update_padd_vol(Paddle*, int);
void update_ball_pos(Ball*);

typedef enum GameScreen {
    STARTSCREEN,
    WAITSTART,
    START,
    INPLAY,
    END,
    ENDWAIT
} GameScreen;

int padd1PosY = PADDLE_BUFF + PADDLE_W;  // it'll be 5 pixels from edge

<<<<<<< HEAD

int ballSize = 2; // 2 pixels^2
int paddSizeLen = 16; // paddSizeLen
int paddSizeWidth = 8; // paddSizeWidth

int padd1PosH = 16;  // it'll be 5 pixels from edge
int padd2PosH = 123; // 128 - 10 + 5, it will be 5 pixels from edge

int pong1Pos = 64/2; // in center of game board
int pong2Pos = 64/2; // in center of game board
=======
int padd2PosY = DISP_W - PADDLE_BUFF - PADDLE_W; // 128 - 10 + 5, it will be 5 pixels from edge
>>>>>>> 0222ebc40bb4219c7848a930c4788fe1d99b91d9

int stdVol = 1; // 1 pixel per iteration
int lastWinner = -1;

GameScreen GAMESTATE = STARTSCREEN;


void play_game(uint16_t * controllers){
    int pOneScore = 0;
    int pTwoScore = 0;
    Ball square;
    Paddle padd1;
    Paddle padd2;
    srand(362);
    while(1){

        switch(GAMESTATE){

        case STARTSCREEN:
            // TODO ADD CODE TO INITIALIZE STARTSCREEN
            pOneScore = 0;
            pTwoScore = 0;
            display_start_screen();
            // DISPLAY "SLIDE RIGHT TO START"
            display_score(pOneScore, pTwoScore);
            // DISPLAY START SCORE TO 00
            display_pong1(DISP_S / 2 - PADDLE_W / 2);
            display_pong2(DISP_S / 2 - PADDLE_W / 2);
            // WAIT FOR CONTROLLER 1 or CONTROLLER 2 to SLIDE RIGHT
            GAMESTATE = WAITSTART;
            break;
        case WAITSTART:
            if (controllers[0] > (4095 / 4) * 3){
                GAMESTATE = START;
            }else if(controllers[1] > (4095 / 4) * 3){
                GAMESTATE = START;
            }
            break;
        case START:
            display_score(pOneScore, pTwoScore);
            // DISPLAY SCORE
            nano_wait(2000000000);
            // WAIT two seconds
            // Velocity/POS is set "randomly"
            // ball is sent towards last WINER
            square.posX = rand() % DISP_S; // 128/2
            square.posY = DISP_W / 2;
            square.volX = stdVol * 2;//rand() % 5 * lastWinner;
            square.volY = stdVol * 2;//rand() % 5;
            // initialize paddles
            padd1.pos = DISP_S / 2 - PADDLE_W / 2;
            padd1.vol = 0;
            padd2.pos = DISP_S / 2 - PADDLE_W / 2;
            padd2.vol = 0;
            // DISPALY INITIAL POS OF OBJECTS
            display_pong1(padd1.pos);
            display_pong2(padd2.pos);
            display_ball(square.posX, square.posY);
            GAMESTATE = INPLAY;
            break;
        case INPLAY:
            nano(100000000);
            // update position of ball
            update_ball_pos(&square);
            // update paddle1 velocity
            update_padd_vol(&padd1, controllers[0]);
            // update paddle2 velocity
            update_padd_vol(&padd1, controllers[1]);
            // update position of paddle1
            update_padd_pos(&padd1);
            // update position of paddle2
            update_padd_pos(&padd2);
            // check if position of ball is in colliding
            if (square.posX > DISP_S - BALL_S){
                square.volX *= -1;
            }
            if (square.posX < BALL_S){
                square.volX *= -1;
            }
            // paddles are 5pxl width and 5 pxls from edge of screen
            if (square.posY >= PADDLE_BUFF && square.posY <= PADDLE_BUFF + PADDLE_W){
                // collid with paddle 1
                if (square.posX >= padd1.pos && square.posX <= padd1.pos + PADDLE_L){
                    square.volY *= -1;
                }
            }
            if (square.posY <= DISP_W - PADDLE_BUFF && square.posY >= DISP_W - PADDLE_BUFF - PADDLE_W){
                // collide with paddle 2
                if (square.posX >= padd2.pos && square.posX <= padd2.pos + PADDLE_L){
                    square.volY *= -1;
                }
            }
            //      if so update velocity direction
            // check if ball is pass either goal
            if (square.posY < PADDLE_BUFF){
                pTwoScore++;
                display_score(pOneScore, pTwoScore);
                if (pTwoScore == 5){
                    GAMESTATE = END;
                    break;
                }
                GAMESTATE = START;
            }
            if (square.posY > DISP_W - PADDLE_BUFF){
                pOneScore++;
                display_score(pOneScore, pTwoScore);
                if (pOneScore == 5){
                    GAMESTATE = END;
                    break;
                }
                GAMESTATE = START;
            }
            //      if so update score and set GAMESTATE to START
            // check if points of game, if score past 5 set GAMESTATE to END
            break;
        case END:
            // DISPLAY FINAL SCORE
            display_score(pOneScore, pTwoScore);
            if (pOneScore == 5){
                display_winner(1);
            }else{
                display_winner(2);
            }
            //display_end_start();
            pOneScore = 0;
            pTwoScore = 0;
            GAMESTATE = WAITSTART;
            // DISPLAY "PLAYER X WON" under score
            // DISPLAY "SLIDE RIGHT" TO START NEW GAME
            break;
        }
    }
}


void update_ball_pos(Ball* square){
    square->posX += square->volX;
    square->posY += square->volY;
    return;
}

void update_padd_pos(Paddle* paddle){
    if (paddle->pos + paddle->vol + PADDLE_L > DISP_S || paddle->pos + paddle->vol < 0){
        return;
    }
    paddle->pos += paddle->vol;
    return;
}

void update_padd_vol(Paddle* paddle, int controller){
    if (controller > (4095 * 3)/4){
        paddle->vol = 3;
    }
    else if (controller < (4095 / 4)){
        paddle->vol = -3;
    }else{
        paddle->vol = 0;
    }

}

//



