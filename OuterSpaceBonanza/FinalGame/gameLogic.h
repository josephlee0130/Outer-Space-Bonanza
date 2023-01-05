#ifndef GAMELOGIC
#define GAMELOGIC

#include <lpc17xx.h>
#include <stdlib.h>
#include <stdio.h>
#include "GLCD.h"
#include "spece.h"
#include <cmsis_os2.h>
#include <os_tick.h>

//these functions are all threads
void readPlayerInput(void *args);
void animate(void *args);
void checkEndGame(void* args);

//These functions are called by threads or main
void enemyShoot(actor* enemy);
void initializeActors(void);

#endif
