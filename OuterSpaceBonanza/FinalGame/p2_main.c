#include <lpc17xx.h>
#include <stdlib.h>
#include <stdio.h>
#include "GLCD.h"
#include "spece.h"
#include <cmsis_os2.h>
#include <os_tick.h>
#include "gameLogic.h"


actor* star;
actor* obstacle[2];
projectile* ball;
launcher* cannon;
spriteLauncher* launchSprites[10]; // array of spriteLauncher so we have different cannon animations.

/*
	we need to be able to stop the animation thread once one of the actors dies, so 
	we are keeping its ID global so that other threads checking on it can stop it
*/
osThreadId_t animateID;
osThreadId_t readInput;
osThreadId_t checkGameOver;



osMutexId_t mut;
const osMutexAttr_t Thread_Mutex_attr = {
	"myButtonMutex",
	0,
	NULL,
	0U
};
//set up Led's so they show how many lives left.
void ledMutex()
{
	while(1)
	{
		osMutexAcquire(mut, osWaitForever);
		LPC_GPIO2 ->FIODIR |= 1<<2;
		LPC_GPIO2 ->FIODIR |= 1<<3;
		LPC_GPIO2 ->FIODIR |= 1<<4;
		LPC_GPIO2 ->FIODIR |= 1<<5;
		LPC_GPIO2 ->FIODIR |= 1<<6;
		
		LPC_GPIO1 ->FIODIR |= 1<<28;
		LPC_GPIO1 ->FIODIR |= 1<<29;
		LPC_GPIO1 ->FIODIR |= 1<<31;


		if(ball->lives ==5)
		{
			LPC_GPIO2 ->FIOSET |= 1<<2;
			LPC_GPIO2 ->FIOSET |= 1<<3;
			LPC_GPIO2 ->FIOSET |= 1<<4;
			LPC_GPIO2 ->FIOSET |= 1<<5;
			LPC_GPIO2 ->FIOSET |= 1<<6;
		}
		else if(ball->lives == 4)
		{
			LPC_GPIO2 ->FIOCLR |= 1<<2;
			LPC_GPIO2 ->FIOSET |= 1<<3;
			LPC_GPIO2 ->FIOSET |= 1<<4;
			LPC_GPIO2 ->FIOSET |= 1<<5;
			LPC_GPIO2 ->FIOSET |= 1<<6;
		}
		else if(ball->lives == 3)
		{
			LPC_GPIO2 ->FIOCLR |= 1<<2;
			LPC_GPIO2 ->FIOCLR |= 1<<3;
			LPC_GPIO2 ->FIOSET |= 1<<4;
			LPC_GPIO2 ->FIOSET |= 1<<5;
			LPC_GPIO2 ->FIOSET |= 1<<6;
		}
		else if(ball->lives == 2)
		{
			LPC_GPIO2 ->FIOCLR |= 1<<2;
			LPC_GPIO2 ->FIOCLR |= 1<<3;
			LPC_GPIO2 ->FIOCLR |= 1<<4;
			LPC_GPIO2 ->FIOSET |= 1<<5;
			LPC_GPIO2 ->FIOSET |= 1<<6;
		}
		else if(ball->lives == 1)
		{
			LPC_GPIO2 ->FIOCLR |= 1<<2;
			LPC_GPIO2 ->FIOCLR |= 1<<3;
			LPC_GPIO2 ->FIOCLR |= 1<<4;
			LPC_GPIO2 ->FIOCLR |= 1<<5;
			LPC_GPIO2 ->FIOSET |= 1<<6;
		}
		osMutexRelease(mut);
	}
}
int main()
{
	//set up the main actors in the game
	initializeActors();
	SystemInit();
	
	printf("Test String");

	GLCD_Init();
	GLCD_Clear(Black);
	GLCD_SetTextColor(Green);
	printStar(star);
	printObstacle(obstacle[0]);
	printObstacle(obstacle[1]);

	//initialize the kernel so that we can create threads
	osKernelInitialize();
	
	mut = osMutexNew(&Thread_Mutex_attr);
	osMutexAcquire(mut, osWaitForever);
	osMutexRelease(mut);
	
  osThreadNew(ledMutex,NULL,NULL); // Thread that lights up led based of number of lives restricted by mutex.

	animateID = osThreadNew(animate,NULL,NULL); // Thread for animating the ball and cannon.
	
	readInput = osThreadNew(readPlayerInput,NULL, NULL); //Thread for reading players input

	checkGameOver = osThreadNew(checkEndGame,NULL,NULL); // Thread that checks if Game Over based on number of lives restricted by mutex.

	osKernelStart();
	
	//Theoretically we will only ever entire this loop if something goes wrong in the kernel
	while(1){};
}
