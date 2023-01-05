#ifndef SPECE
#define SPECE

#include "GLCD.h"
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

//sprites are 6x8 bitmaps, scaled by SPRITE_SCALE
#define SPRITE_ROWS 6
#define SPRITE_COLS 8
#define SPRITE_SCALE 3

//size of the screen
#define VERTICAL_LIM 320
#define HORIZONTAL_LIM 240


typedef struct actor_struct{
	int verticalPosition;
	int horizontalPosition;
	char* sprite;
	bool isStar;
}actor;

typedef struct ball_struct{
	int verticalPosition;
	int horizontalPosition;
	char* sprite;
	int xChange;
	int yChange;
	bool fired;
	int hitsLeft;
	int lives;
}projectile;

typedef struct launch_struct{
	int verticalPosition;
	int horizontalPosition;
	char* sprite;
	int xChange;
	int yChange;
}launcher;

typedef struct sprite_struct{
	char* sprite;
}spriteLauncher;


//Prints a sprite starting at that sprite's top-left corner
void printSpriteAt(int x, int y,char* spriteBMP);

/*
	draws a scale x scale square of pixels. This function allows us
	to specify sprites as 6x8 bitmaps but scale them as much as we want, 
	which saves memory at the expense of CPU cycles
*/
void drawBigPixelAt(int x, int y, int scale);

/*
	Enemy movement logic is a bit complicated, so it is done in two functions. updateEnemyPosition
	moves the enemy and changes its direction if it hits a wall, printEnemy clears the sprite and re-draws it
	in the new position
*/
//updates x and y position of the enemy


void printObstacle(actor* obst);

void printStar(actor* shiny);

void printTarget(actor* target);

void printBall(projectile* beam);

void printCannon(launcher* blast);


//determines if any part of act1 overlaps with act2
bool checkCollision(actor* act, projectile* bal);

#endif
