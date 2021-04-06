#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

// TODO: Create any necessary structs

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/


typedef struct {
    int height;
    int width;

    int x;
    int y;

   int justMovedRight;
   int justMovedLeft;
   int justMovedUp;
   int justMovedDown;
   int right;

   int shootLeft;
   int shootRight;
   int health;

} Samus;

typedef struct {
    int height;
    int width;

    int x;
    int y;

    int moveRight;
    int moveLeft;
    int moveUp;
    int moveDown;
    int dead;
   int right;
   

} Enemy;
typedef struct {
    int x;
    int y;
    int width;
    int height;
    int right;
    int exist;
    int counter;
} Bullet;

typedef struct { 
    
    int score;
    Samus samus;
    Enemy enemy;
    Bullet bullet;
    int win;

} gameState;





#endif
