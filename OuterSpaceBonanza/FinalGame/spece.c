#include "spece.h"
void drawBigPixelAt(int x, int y, int scale)
{
	int i;
	int j;
	for(i = 0; i < scale; i++)
	{
		for(j = 0; j < scale;j++)
			GLCD_PutPixel(x+i,y+j);
	}
}

//specifies top left corner.
void printSpriteAt(int x, int y,char* spriteBMP)
{
	//print the sprite left to right. Remember, x is the vertical component and y is the horizontal
	//given the orientation of the screen, joystick, and button
	int i;
	int j;
	int spriteIndex = SPRITE_ROWS-1;
	int spriteShift = 0;
	
	for(i = (SPRITE_ROWS-1)*SPRITE_SCALE; i >= 0; i-=SPRITE_SCALE)
	{
		spriteShift = 0;
		for(j = 0; j < SPRITE_COLS*SPRITE_SCALE; j+=SPRITE_SCALE)
		{
			if((spriteBMP[spriteIndex] >> spriteShift)&1)
			{
				drawBigPixelAt(x+((SPRITE_ROWS-1)*SPRITE_SCALE-i),y+j,SPRITE_SCALE);
			}
			spriteShift++; //This is the column coordinate of the sprite's bitmap
		}
		spriteIndex--; //this is the row coordinate
	}
}

// displaying game entities functions(e.g.star, obstacles, cannon, ball).
void printStar(actor* shiny)
{
	GLCD_SetTextColor(Black);
	printSpriteAt(shiny->verticalPosition,shiny->horizontalPosition,shiny->sprite);
	
	GLCD_SetTextColor(Yellow);
	printSpriteAt(shiny->verticalPosition,shiny->horizontalPosition,shiny->sprite);
}
void printObstacle(actor* obst)
{
	GLCD_SetTextColor(Black);
	printSpriteAt(obst->verticalPosition,obst->horizontalPosition,obst->sprite);
	
	GLCD_SetTextColor(Red);
	printSpriteAt(obst->verticalPosition,obst->horizontalPosition,obst->sprite);
}

void printBall(projectile* beam)
{
	
	GLCD_SetTextColor(Black);
	printSpriteAt(beam->verticalPosition,beam->horizontalPosition,beam->sprite);
	//makes sure ball bounces of the border of the screen.
	if(beam->verticalPosition > VERTICAL_LIM)
	{
		beam->yChange *= (-1);
		beam -> hitsLeft -= 1;
	}
	if(beam->verticalPosition < 0)
	{
		beam->yChange *= (-1);
		beam -> hitsLeft -= 1;
	}
	if(beam->horizontalPosition > HORIZONTAL_LIM)
	{
		beam->xChange *= (-1);
		beam -> hitsLeft -= 1;
	}
	if(beam->horizontalPosition < 0)
	{
		beam->xChange *= (-1);
		beam -> hitsLeft -= 1;
	}
	//if ball hits the wall 5 times, loss a life.
	if(beam -> hitsLeft < 1) 
	{
			GLCD_SetTextColor(Black);
			beam -> horizontalPosition = 20;
			beam -> verticalPosition = 20;
			beam -> fired = false;
			beam -> hitsLeft = 5;
			beam -> yChange = 1;
			beam -> xChange = 5;
			if(beam ->lives != 5)
				beam ->lives -= 1;
	}
	//move the ball depending on the launch slope.
	beam->verticalPosition += beam->yChange;
	beam->horizontalPosition += beam->xChange; 

	GLCD_SetTextColor(Blue);
	printSpriteAt(beam->verticalPosition,beam->horizontalPosition,beam->sprite);
}
void printCannon(launcher* blast)
{
char blackout[] = {0xFF,0xFF,0xFF, 0xFF, 0xFF, 0xFF}; 
	GLCD_SetTextColor(Black);
	printSpriteAt(blast->verticalPosition,blast->horizontalPosition,blackout);

	GLCD_SetTextColor(Yellow);
	printSpriteAt(blast->verticalPosition,blast->horizontalPosition,blast->sprite);
}

//determine if any part of Act 1 overlaps with Act 2
bool checkCollision(actor* act, projectile* bal)
{
	//extract the coordinates. It's a LOT easier this way
	//top corner act 1
	int x11;
	int y11;
	
	//top corner act 2
	int x21;
	int y21;
	
	//bottom corner act 1
	int x12;
	int y12;
	
	//bottom corner act 2
	int x22;
	int y22;
	x11 = act->horizontalPosition;
	y11 = act->verticalPosition;
	x12 = act->horizontalPosition + SPRITE_COLS*SPRITE_SCALE;
	y12 = act->verticalPosition + SPRITE_ROWS*SPRITE_SCALE;
	
	
	x21 = bal->horizontalPosition;
	y21 = bal->verticalPosition;
	x22 = bal->horizontalPosition + SPRITE_COLS*SPRITE_SCALE;
	y22 = bal->verticalPosition + SPRITE_ROWS*SPRITE_SCALE;
	
	//if the second argument is a laser, the sprite is far bigger than it needs to be, so we need to compensate
	//Note to students: you can play with these numbers to get it more precise. I didn't feel like doing that
	
	return (x11<x22 && x12 > x21 && y11 < y22 && y12 > y21);
		
	
}