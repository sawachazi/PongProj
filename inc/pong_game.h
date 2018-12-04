/*
 * pong_game.h
 *
 *  Created on: Nov 18, 2018
 *      Author: zachwalters
 */

#ifndef PONG_GAME_H_
#define PONG_GAME_H_

typedef struct PongBall{
    int posX;
    int posY;
    int volX;
    int volY;
} Ball;


typedef struct PongPaddle{
    int pos;
    int vol;
} Paddle;

#endif /* PONG_GAME_H_ */
