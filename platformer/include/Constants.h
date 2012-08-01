#pragma once

const int WINDOW_WIDTH = 512;
const int WINDOW_HEIGHT = 320;

const int TILE_WIDTH = 16;
const int TILE_HEIGHT = 16; 
const int TILE_SIZE = 16; 

const int MAP_WIDTH = WINDOW_WIDTH/TILE_WIDTH;
const int MAP_HEIGHT = WINDOW_HEIGHT/TILE_HEIGHT;

const int TILE_TOTAL = 32;

const int PI = 3.14159; 

const float GRAVITY = 10.0f;

const int MAX_BACON = 64; 
const int MAX_LASERS = 64; 
const int PALLETE_MAX = 64; 

const int PLAYER_WIDTH = 16;
const int PLAYER_HEIGHT = 16; 

const int MAX_ANIMS = 5; 
const int MAX_FRAMES = 5; 

enum LAYERS
{
	COLLISION =0,
	MAIN,
	ENTITIES,
	BACKGROUND,

	TOTAL
};
