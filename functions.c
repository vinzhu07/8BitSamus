#include "functions.h"
#include "gba.h"
#include "images/samus.h"
#include "images/samusleft.h"
#include "images/jigglypuff.h"
#include "images/jigglypuffleft.h"
#include "images/blast.h"
#include "images/blastleft.h"

#include <stdio.h>
#include <stdlib.h>

#include "main.h"

gameState setUpGame(gameState state)
{
    fillScreenDMA(BLACK);
    drawImageDMA(50, 50, 30, 40, samus);

    Samus samus;
    samus.height = 40;
    samus.width = 30;
    samus.x = 50;
    samus.y = 50;
    samus.justMovedDown = 0;
    samus.justMovedLeft = 0;
    samus.justMovedRight = 0;
    samus.justMovedUp = 0;
    samus.right = 1;
    samus.health = 4;

    state.samus = samus;

    drawImageDMA(50, 150, 23, 26, jigglypuff);
    Enemy jigglypuff;
    jigglypuff.height = 26;
    jigglypuff.width = 23;
    jigglypuff.right = 1;
    jigglypuff.x = 50;
    jigglypuff.y = 150;
    jigglypuff.dead = 0;
    jigglypuff.moveDown = 0;
    jigglypuff.moveRight = 0;
    jigglypuff.moveLeft = 0;
    jigglypuff.moveUp = 0;
    state.enemy = jigglypuff;

    state.score = 0;
    char score[12];
    sprintf(score, "Score: %d", state.score);

    drawString(10, 10, score, WHITE);
    //drawImageDMA(100, 150, 6, 2, blast);
    Bullet bullet;
    bullet.exist = 0;
    bullet.width = 6;
    bullet.height = 2;
    bullet.x = 0;
    bullet.y = 0;
    bullet.counter = 0;
    state.bullet = bullet;
    state.win = 0;
    return state;
}

gameState process(gameState state, u32 current, u32 previous)
{
    if (KEY_DOWN(BUTTON_RIGHT, current))
    {
        state.samus.justMovedRight = 1;
    }
    if ((KEY_DOWN(BUTTON_LEFT, current)))
    {
        state.samus.justMovedLeft = 1;
    }
    if (KEY_DOWN(BUTTON_DOWN, current))
    {
        state.samus.justMovedDown = 1;
    }
    if (KEY_DOWN(BUTTON_UP, current))
    {
        state.samus.justMovedUp = 1;
    }

    if (state.samus.x > state.enemy.x)
    {
        state.enemy.moveDown = 1;
    }
    if (state.samus.x < state.enemy.x)
    {
        state.enemy.moveUp = 1;
    }
    if (state.samus.y > state.enemy.y)
    {
        state.enemy.moveRight = 1;
    }
    if (state.samus.y < state.enemy.y)
    {
        state.enemy.moveLeft = 1;
    }
    // if (state.bullet.counter == 0) {
    //     state.bullet.exist = 0;
    // }
    if (KEY_DOWN(BUTTON_A, current) && state.bullet.exist == 0)
    {
        //fillScreenDMA(WHITE);
        state.bullet.x = state.samus.x + 8;

        if (state.samus.right)
        {
            state.bullet.right = 1;
            state.samus.shootRight = 1;
            state.bullet.y = state.samus.y + state.samus.width + 10;

            drawImageDMA(state.bullet.x, state.samus.y + state.samus.width + 10, state.bullet.width, state.bullet.height, blast);
        }
        else
        {
            state.bullet.right = 0;
            state.samus.shootLeft = 1;
            state.bullet.y = state.samus.y - state.bullet.width - 10;
            drawImageDMA(state.samus.x + 6, state.samus.y - 10, state.bullet.width, state.bullet.height, blastleft);
        }

        state.bullet.exist = 1;
        state.bullet.counter++;
    }

    if (state.enemy.dead)
    {
        int newx = -1;
        int newy = -1;
        newx = rand() % 200;
        newy = rand() % 160;
        while (newx < 0 || newy < 0 || abs(newx - state.samus.x) < 50 || abs(newy - state.samus.y) < 50)
        {
            newx = rand() % 200;
            newy = rand() % 130;
        }
        state.enemy.x = newx;
        state.enemy.y = newy;
        drawImageDMA(newx, newy, state.enemy.width, state.enemy.height, jigglypuff);
        state.enemy.dead = 0;
    }

    UNUSED(previous);
    UNUSED(current);
    return state;
}

gameState update(gameState *state)
{
    //fillScreenDMA(RED);
    if (state->samus.justMovedRight)
    {
        if (state->samus.y + state->samus.width + 3 < WIDTH)
        {
            drawRectDMA(state->samus.x, state->samus.y - 2, 5, state->samus.height, BLACK);
            drawImageDMA(state->samus.x, state->samus.y, state->samus.width, state->samus.height, samus);
            state->samus.y += 3;
            state->samus.right = 1;
        }
        state->samus.justMovedRight = 0;
    }

    if (state->samus.justMovedLeft)
    {
        if (state->samus.y - 3 > 0)
        {
            drawImageDMA(state->samus.x, state->samus.y, state->samus.width, state->samus.height, samusleft);
            drawRectDMA(state->samus.x, state->samus.y + state->samus.width, 3, state->samus.height, BLACK);
            //drawImageDMA(state->samus.x, state->samus.y, state->samus.width, state->samus.height, samusleft);
            state->samus.y -= 3;
            state->samus.right = 0;
        }
        state->samus.justMovedLeft = 0;
    }

    if (state->samus.justMovedUp)
    {
        if (state->samus.x - 3 > 0)
        {
            if ((state->samus.right) == 0)
            {
                drawImageDMA(state->samus.x - 3, state->samus.y, state->samus.width, state->samus.height, samusleft);
            }
            else
            {

                drawImageDMA(state->samus.x - 3, state->samus.y, state->samus.width, state->samus.height, samus);
            }
            drawRectDMA(state->samus.x + state->samus.height - 3, state->samus.y - 2, state->samus.width + 5, 6, BLACK);
            //drawImageDMA(state->samus.x, state->samus.y, state->samus.width, state->samus.height, samusleft);
            state->samus.x -= 3;
        }
        state->samus.justMovedUp = 0;
    }

    if (state->samus.justMovedDown)
    {
        if (state->samus.x + state->samus.height + 3 < HEIGHT)
        {
            if ((state->samus.right) == 0)
            {

                drawImageDMA(state->samus.x + 3, state->samus.y, state->samus.width, state->samus.height, samusleft);
            }
            else
            {
                drawImageDMA(state->samus.x + 3, state->samus.y, state->samus.width, state->samus.height, samus);
            }
            //drawImageDMA(state->samus.x, state->samus.y, state->samus.width, state->samus.height, samusleft);
            drawRectDMA(state->samus.x, state->samus.y, state->samus.width, 3, BLACK);
            drawRectDMA(state->samus.x + state->samus.height - 3, state->samus.y - 2, state->samus.width + 2, 4, BLACK);

            state->samus.x += 3;
        }
        state->samus.justMovedDown = 0;
    }

    if (state->enemy.moveLeft)
    {
        drawRectDMA(state->enemy.x, state->enemy.y, state->enemy.width, state->enemy.height, BLACK);
        drawImageDMA(state->enemy.x, state->enemy.y - 1, state->enemy.width, state->enemy.height, jigglypuffleft);
        state->enemy.y -= 1;
        state->enemy.moveLeft = 0;
        state->enemy.right = 0;
    }

    if (state->enemy.moveRight)
    {
        drawRectDMA(state->enemy.x, state->enemy.y, state->enemy.width, state->enemy.height, BLACK);
        drawImageDMA(state->enemy.x, state->enemy.y + 1, state->enemy.width, state->enemy.height, jigglypuffleft);
        state->enemy.y += 1;
        state->enemy.moveRight = 0;
        state->enemy.right = 1;
    }
    if (state->enemy.moveUp)
    {
        drawRectDMA(state->enemy.x, state->enemy.y, state->enemy.width, state->enemy.height, BLACK);
        if (state->enemy.right)
        {
            drawImageDMA(state->enemy.x - 1, state->enemy.y, state->enemy.width, state->enemy.height, jigglypuff);
        }
        else
        {
            drawImageDMA(state->enemy.x - 1, state->enemy.y, state->enemy.width, state->enemy.height, jigglypuffleft);
        }
        state->enemy.x -= 1;

        state->enemy.moveUp = 0;
    }
    if (state->enemy.moveDown)
    {
        drawRectDMA(state->enemy.x, state->enemy.y, state->enemy.width, state->enemy.height, BLACK);
        if (state->enemy.right)
        {
            drawImageDMA(state->enemy.x + 1, state->enemy.y, state->enemy.width, state->enemy.height, jigglypuff);
        }
        else
        {
            drawImageDMA(state->enemy.x + 1, state->enemy.y, state->enemy.width, state->enemy.height, jigglypuffleft);
        }
        state->enemy.x += 1;

        state->enemy.moveDown = 0;
    }

    char score[12];
    sprintf(score, "Score: %d", state->score);

    drawRectDMA(10, 10, 50, 15, BLACK);
    drawString(10, 10, score, WHITE);
    // drawRectDMA(state->bullet.x, state->bullet.y, state->bullet.width, state->bullet.height, BLACK);
    // drawImageDMA(state->bullet.x+6, state->bullet.y, state->bullet.width, state->bullet.height, blast);

    if (state->bullet.exist && (vBlankCounter % 3 == 0))
    {

        if (state->bullet.right)
        {
            drawRectDMA(state->bullet.x, state->bullet.y, state->bullet.width + 3, state->bullet.height, BLACK);

            state->bullet.y += 4;

            drawImageDMA(state->bullet.x, state->bullet.y, state->bullet.width, state->bullet.height, blast);
        }
        else
        {
            drawRectDMA(state->bullet.x - 2, state->bullet.y, state->bullet.width * 2 + 2, state->bullet.height, BLACK);
            state->bullet.y -= 4;

            drawImageDMA(state->bullet.x - 2, state->bullet.y, state->bullet.width, state->bullet.height, blastleft);
        }
        //drawRectDMA(state->bullet.x, state->bullet.y, state->bullet.width+10, state->bullet.height+10, BLUE);
        //drawImageDMA(state->bullet.x, state->bullet.y+20, state->bullet.width, state->bullet.height, blast);
        if (state->bullet.y + state->bullet.width > WIDTH || state->bullet.y - state->bullet.width < 0)
        {
            state->bullet.exist = 0;
            drawRectDMA(state->bullet.x, state->bullet.y, state->bullet.width, state->bullet.height, BLACK);
        }
    }

    if (state->bullet.x >= state->enemy.x && state->bullet.x <= state->enemy.x + state->enemy.height && state->bullet.y <= state->enemy.y && state->bullet.y + state->bullet.width > state->enemy.y && state->enemy.dead == 0)
    {
        //
        state->bullet.exist = 0;
        state->enemy.dead = 1;
        state->bullet.x = -100;
        state->bullet.y = -100;
        // drawRectDMA(state->bullet.x, state->bullet.y, state->bullet.width, state->bullet.height, RED);
        // drawRectDMA(state->enemy.x, state->enemy.y, state->enemy.width, state->enemy.height, BLACK);
        fillScreenDMA(BLACK);
        if ((state->samus.right) == 0)
        {

            drawImageDMA(state->samus.x, state->samus.y, state->samus.width, state->samus.height, samusleft);
        }
        else
        {
            drawImageDMA(state->samus.x, state->samus.y, state->samus.width, state->samus.height, samus);
        }
        state->score++;
    }
    else if (state->bullet.x >= state->enemy.x && state->bullet.x <= state->enemy.x + state->enemy.height && state->bullet.y <= state->enemy.y + state->enemy.width && state->bullet.y + state->bullet.width > state->enemy.y + state->enemy.width && state->enemy.dead == 0)
    {
        //
        state->bullet.exist = 0;
        state->enemy.dead = 1;
        state->bullet.x = -100;
        state->bullet.y = -100;
        // drawRectDMA(state->bullet.x, state->bullet.y, state->bullet.width, state->bullet.height, BLACK);
        // drawRectDMA(state->enemy.x, state->enemy.y, state->enemy.width, state->enemy.height, BLACK);
        fillScreenDMA(BLACK);
        if ((state->samus.right) == 0)
        {

            drawImageDMA(state->samus.x, state->samus.y, state->samus.width, state->samus.height, samusleft);
        }
        else
        {
            drawImageDMA(state->samus.x, state->samus.y, state->samus.width, state->samus.height, samus);
        }
        state->score++;
    }
    if (state->score == 5)
    {
        state->win = 1;
    }

    if ((state->samus.x >= state->enemy.x && state->samus.x <= state->enemy.x + state->enemy.height && state->samus.y <= state->enemy.y && state->samus.y + state->samus.width > state->enemy.y && state->enemy.dead == 0) || ((state->samus.x >= state->enemy.x && state->samus.x <= state->enemy.x + state->enemy.height && state->samus.y <= state->enemy.y + state->enemy.width && state->samus.y + state->samus.width > state->enemy.y + state->enemy.width && state->enemy.dead == 0)) 
    || (((state->enemy.y >=state->samus.y && state->enemy.y <=state->samus.y + state->samus.width)||((state->enemy.y +state->enemy.width>=state->samus.y && state->enemy.y+state->enemy.width <=state->samus.y + state->samus.width))) && (state->enemy.x <= state->samus.x+state->samus.height && state->enemy.x >= state->samus.x) && state->enemy.dead == 0) 
    || (((state->enemy.y >=state->samus.y && state->enemy.y <=state->samus.y + state->samus.width)||((state->enemy.y +state->enemy.width>=state->samus.y && state->enemy.y+state->enemy.width <=state->samus.y + state->samus.width))) && (state->enemy.x <= state->samus.x && state->enemy.x +state->enemy.height>= state->samus.x) && state->enemy.dead == 0) )
    {
        state->enemy.dead = 1;
        fillScreenDMA(BLACK);
        if ((state->samus.right) == 0)
        {

            drawImageDMA(state->samus.x, state->samus.y, state->samus.width, state->samus.height, samusleft);
        }
        else
        {
            drawImageDMA(state->samus.x, state->samus.y, state->samus.width, state->samus.height, samus);
        }
        state->samus.health--;
        if (state->samus.health <= 0) {
            state->win = -1;
        }
    }

    return *state;
}
