#include "gameLogic.h"

extern actor* obstacle[2];
extern actor* star;
extern projectile* ball;
extern launcher* cannon;
extern spriteLauncher* launchSprites[10];
extern osThreadId_t animateID;
extern osThreadId_t readInput;
extern osThreadId_t checkGameOver;
extern osMutexId_t mut;

char obstacleSprite[]= {0xFF,0xA5,0x99, 0x99, 0xA5, 0xFF};
char starSprite[] = {0xC3,0x24,0x18, 0x18, 0x24, 0xC3};
char ballSprite[] = {0x18,0x3C,0x7E, 0x7E, 0x3C, 0x18};

//differnet cannon orientations
char launchSprite1[] = {0x00,0x00,0x00, 0x00, 0x83, 0x03}; 
char launchSprite2[] = {0x00,0x00,0x00, 0x80, 0x03, 0x03}; 
char launchSprite3[] = {0x00,0x00,0x80, 0x00, 0x03, 0x03}; 
char launchSprite4[] = {0x00,0x80,0x00, 0x00, 0x03, 0x03}; 
char launchSprite5[] = {0x80,0x00,0x00, 0x00, 0x03, 0x03}; 
char launchSprite6[] = {0x40,0x00,0x00, 0x00, 0x03, 0x03}; 
char launchSprite7[] = {0x20,0x00,0x00, 0x00, 0x03, 0x03}; 
char launchSprite8[] = {0x10,0x00,0x00, 0x00, 0x03, 0x03}; 
char launchSprite9[] = {0x08,0x00,0x00, 0x00, 0x03, 0x03}; 
char launchSprite10[] = {0x04,0x00,0x00, 0x00, 0x03, 0x03}; 

int cannonStart_x = 5;
int cannonStart_y = 5;

void readPlayerInput(void *args)
{
	while(1)
	{
		//check the joystick cannon angle. 
		if(!(LPC_GPIO1->FIOPIN &= (1<<23)))
		{
			osDelay(100U);
			if(ball->yChange < 10)
			ball->yChange += 1;
		}
		else if(!(LPC_GPIO1->FIOPIN &= (1<<25)))
		{
			osDelay(100U);
			if(ball->yChange > 1)
			ball->yChange -= 1;
		}
		
		//check the button for cannon shot.
		if(!(LPC_GPIO2->FIOPIN & (1<<10)))
		{
				ball -> fired = true;
		}
		osThreadYield();
	}
}

void animate(void *args)
{
	while(1){
		cannon -> sprite = launchSprites[(ball->yChange)-1]->sprite; // obtains the correct sprite for the cannon
		printCannon(cannon);
		if(ball->fired == true)
			printBall(ball);
	
		osDelay(5*(ball->yChange)/1U); //less delay for lower yChnage, ,more delay for higher yChange
	}
}


//thread to check for end-game conditions
void checkEndGame(void* args)
{
	while(1)
	{
		osMutexAcquire(mut, osWaitForever);
		//checks to see if the ball hits the star, if true, Player Wins.
		if(checkCollision(star, ball))
		{
				osThreadTerminate(animateID);
				GLCD_Clear(Black);
				GLCD_SetTextColor(Yellow);
				GLCD_SetBackColor(Black);
				GLCD_DisplayString(1, 1, 1, "WINNER!");
				osThreadTerminate(readInput);
				osThreadTerminate(checkGameOver);
		}
		
		//checks if ball hits a obstacle, if true and no more lives, Player Loses.
		else if(checkCollision(obstacle[0], ball) || checkCollision(obstacle[1], ball))
		{
			if(ball->lives == 1)
			{
				osThreadTerminate(animateID);
				GLCD_Clear(Black);
				GLCD_SetTextColor(Red);
				GLCD_SetBackColor(Black);
				GLCD_DisplayString(1, 1, 1, "GAME OVER!");
				osThreadTerminate(animateID);
				osThreadTerminate(readInput);
				osThreadTerminate(checkGameOver);
			}
			//if ball hits obstacle with extra lives, revert back
			else
			{
				GLCD_SetTextColor(Black);
				printSpriteAt(ball->verticalPosition,ball->horizontalPosition,ball->sprite);
				ball -> horizontalPosition = cannonStart_x;
				ball -> verticalPosition = cannonStart_y;
				ball ->lives -= 1;
				ball -> fired = false;
				ball -> hitsLeft = 5;
				ball -> yChange = 1;
				ball -> xChange = 5;
			}
		}
		osMutexRelease(mut);
	}
}

void initializeActors()
{
	ball = malloc(sizeof(projectile));
	ball ->verticalPosition = cannonStart_y;
	ball ->horizontalPosition = cannonStart_x;
	ball ->sprite = ballSprite;
	ball -> fired = false;
	ball -> yChange = 1;
	ball -> xChange = 5;
	ball -> lives = 5;
	
	cannon = malloc(sizeof(launcher));
	cannon ->horizontalPosition = cannonStart_x;
	cannon ->verticalPosition = cannonStart_y;
	cannon -> sprite = launchSprites[(ball->yChange)-1]->sprite;
	
	obstacle[0] = malloc(sizeof(actor));
	obstacle[0] -> horizontalPosition = 140;
	obstacle[0] -> verticalPosition = 140;
	obstacle[0] -> sprite = obstacleSprite;
	obstacle[0] -> isStar = false;
	
	obstacle[1] = malloc(sizeof(actor));
	obstacle[1] -> horizontalPosition = 30;
	obstacle[1] -> verticalPosition = 100;
	obstacle[1] -> sprite = obstacleSprite;
	obstacle[1] -> isStar = false;
	
	launchSprites[0] = malloc(sizeof(spriteLauncher));
	launchSprites[0]->sprite = launchSprite1; 
	launchSprites[1] = malloc(sizeof(spriteLauncher));
	launchSprites[1]->sprite = launchSprite2; 
	launchSprites[2] = malloc(sizeof(spriteLauncher));
	launchSprites[2]->sprite = launchSprite3; 
	launchSprites[3] = malloc(sizeof(spriteLauncher));
	launchSprites[3]->sprite = launchSprite4; 
	launchSprites[4] = malloc(sizeof(spriteLauncher));
	launchSprites[4]->sprite = launchSprite5; 
	launchSprites[5] = malloc(sizeof(spriteLauncher));
	launchSprites[5]->sprite = launchSprite6; 
	launchSprites[6] = malloc(sizeof(spriteLauncher));
	launchSprites[6]->sprite = launchSprite7; 
	launchSprites[7] = malloc(sizeof(spriteLauncher));
	launchSprites[7]->sprite = launchSprite8; 
	launchSprites[8] = malloc(sizeof(spriteLauncher));
	launchSprites[8]->sprite = launchSprite9; 
	launchSprites[9] = malloc(sizeof(spriteLauncher));
	launchSprites[9]->sprite = launchSprite10;
	
	star= malloc(sizeof(actor));
	star-> horizontalPosition = 20;
	star->verticalPosition = 280;
	star->sprite = starSprite;
}


