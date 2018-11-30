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



int ballSize = 2; // 2 pixels^2
int paddSizeLen = 15; // paddSizeLen
int paddSizeWidth = 5; // paddSizeWidth

int padd1PosH = 10;  // it'll be 5 pixels from edge
int padd2PosH = 123; // 128 - 10 + 5, it will be 5 pixels from edge

int pong1Pos = 64/2; // in center of game board
int pong2Pos = 64/2; // in center of game board

int stdVol = 1; // 1 pixel per iteration
int lastWinner = -1;

GameScreen GAMESTATE = STARTSCREEN;


void play_game(uint16_t * controllers){
    int pOneScore = 0;
    int pTwoScore = 0;
    Ball* square;
    Paddle* padd1;
    Paddle* padd2;
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
            display_pong1(pong1Pos);
            display_pong2(pong2Pos);
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
            square->posX = 64; // 128/2
            square->posY = rand() % 64;
            square->volX = stdVol * 2;//rand() % 5 * lastWinner;
            square->volY = stdVol * 2;//rand() % 5;
            // initialize paddles
            padd1->posY = pong1Pos;
            padd1->volY = 0;
            padd2->posY = pong2Pos;
            padd2->volY = 0;
            // DISPALY INITIAL POS OF OBJECTS
            display_pong1(padd1->posY);
            display_pong2(padd2->posY);
            display_ball(square->posX, square->posY);
            GAMESTATE = INPLAY;
            break;
        case INPLAY:
            // update position of ball
            update_ball_pos(&square);
            // update position of paddle1
            update_padd_pos(&padd1);
            // update position of paddle2
            update_padd_pos(&padd2);
            // check if position of ball is in colliding
            if (square->posY > 62){
                square->volX *= -1;
            }
            if (square->posY < 2){
                square->volX *= -1;
            }
            // paddles are 5pxl width and 5 pxls from edge of screen
            if (square->posX > 4 && square->posX < 9){
                // collid with paddle 1
                if (square->posY > padd1->posY - 7 && square->posY < padd1->posY + 7){
                    square->volY *= -1;
                }
            }
            if (square->posX > 117 && square->posX < 122){
                // collide with paddle 2
                if (square->posY > padd2->posY - 7 && square->posY < padd2->posY + 7){
                    square->volY *= -1;
                }
            }
            //      if so update velocity direction
            // check if ball is pass either goal
            if (square->posX < 4){
                pTwoScore++;
                display_score(pOneScore, pTwoScore);
                if (pTwoScore == 5){
                    GAMESTATE = END;
                    break;
                }
                GAMESTATE = START;
            }
            if (square->posX > 122){
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
            display_end_start();
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
    if (paddle->posY + paddle->volY + 7 > 62 || paddle->posY + paddle->volY - 7 < 2){
        return;
    }
    paddle->posY += paddle->volY;
    return;
}

void update_padd_vol(Paddle* paddle, int controller){
    if (controller > (4095 * 3)/4){
        paddle->volY = 3;
    }
    else if (controller < (4095 / 4)){
        paddle->volY = -3;
    }else{
        paddle->volY = 0;
    }


}

//



