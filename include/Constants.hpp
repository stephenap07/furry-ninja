#pragma once


const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32; 
const int TILE_SIZE = 32; 

// 20x15 tiles
const int MAP_WIDTH = WINDOW_WIDTH/TILE_WIDTH;
const int MAP_HEIGHT = WINDOW_HEIGHT/TILE_HEIGHT;

const int CLEAR_TILE = 16;

const int TILE_TOTAL = 32;

const int PI = 3.14159; 

const float GRAVITY = 15.0f;

const int MAX_BACON = 64; 
const int MAX_LASERS = 64; 
const int PALLETE_MAX = 64; 

const int PLAYER_WIDTH = 19;
const int PLAYER_HEIGHT = 26; 

const int MAX_ANIMS = 2; 
const int MAX_FRAMES = 12; 

enum LAYERS
{
	COLLISION =0,
	MAIN,
	ENTITIES,
	BACKGROUND,

	TOTAL
};
