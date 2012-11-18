#ifndef COLLISION_H_
#define COLLISION_H_

#include "Constants.hpp"

int map[MAP_HEIGHT][MAP_WIDTH] = 
{ { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, },
  { 0 , 0 , 0 , 16, 16, 16, 16, 16, 16, 0 , 0 , 0 , 0 , 16, 16, 16, 16, 16, 16, 16, },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, },
  { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , }
};

int collisionMap[MAP_HEIGHT][MAP_WIDTH];

/*
*=================================*
*                                 *
*	    Checking Collisions         *
*                                 *
*=================================*
*/

inline bool Collision_Horz_Down(int x, int y, int width, int & tileY)
{
	int tilePixels = x - (x % TILE_SIZE);
	int testEnd = x + width;

	tileY = y/TILE_SIZE;

	int tileX = tilePixels/TILE_SIZE;

	while( tilePixels <= testEnd) {
		if( map[tileY][tileX] < CLEAR_TILE )
			return true;

		tileX++;
		tilePixels += TILE_SIZE;
	}

	return false;
}

inline bool Collision_Horz_Up(int x, int y, int width, int & tileY)
{
	int tilePixels = x-(x % TILE_SIZE);
	int testEnd = x + width;

	tileY = y/TILE_SIZE;

	int tileX = tilePixels/TILE_SIZE;

	while( tilePixels <= testEnd) {
		if( map[tileY][tileX] < CLEAR_TILE )
			return true;

		tileX++;
		tilePixels += TILE_SIZE;
	}

	return false;
}

inline bool Collision_Ver(int x, int y, int height, int & tileX)
{
	int tilePixels = y - ( y % TILE_SIZE );
	int testEnd = y + height;

	tileX = x/TILE_SIZE;

	int tileY = tilePixels/TILE_SIZE;

	while( tilePixels < testEnd) {
		if(map[tileY][tileX] < CLEAR_TILE)
			return true;

		tileY++;
		tilePixels += TILE_SIZE;
	}

	return false;
}

inline bool RectCollision(const sf::Sprite & a, const sf::Sprite & b)
{
	sf::IntRect ar = a.getTextureRect();
	sf::IntRect br = b.getTextureRect();

	if( a.getPosition().x + ar.width < b.getPosition().x  ||
		a.getPosition().x > b.getPosition().x + br.width  ||
		a.getPosition().y + ar.height < b.getPosition().y ||
		a.getPosition().y > b.getPosition().y + br.height )
		return false;

	return true;
}

inline void Gravity(float dt, float &velY)
{
	velY += GRAVITY*dt; 
}

void PhysicsDos( float dt, Player* ply )
{
	int tileCoord;

	//X AXIS

  float x = ply->sprite.getPosition().x;
  float y = ply->sprite.getPosition().y; 
  int width = ply->sprite.getTextureRect().height; 
  int height = ply->sprite.getTextureRect().height; 
  float velX = ply->velX; 

	if( ply->velX > 0 ) { //moving right

		if( Collision_Ver(x + width + velX*dt, y, height, tileCoord ) ) {
			ply->sprite.setPosition( sf::Vector2f(tileCoord*TILE_SIZE - width, y) );
		}
		else
			ply->sprite.move( sf::Vector2f(ply->velX*dt, 0) );
	}

	else if( ply->velX < 0) { //moving left

		if( Collision_Ver( ply->sprite.getPosition().x + ply->velX*dt, ply->sprite.getPosition().y, ply->sprite.getTextureRect().height, tileCoord ) ) { //collision on the left side
		  ply->sprite.setPosition( sf::Vector2f((tileCoord + 1)*TILE_SIZE, ply->sprite.getPosition().y) );	//move to the edge of the tile
		}
		else 
      ply->sprite.move( sf::Vector2f(ply->velX*dt, 0) ); 
	}

	//Y AXIS

	if( ply->velY < 0) { //moving up

		ply->grounded = false;
    ply->sprite.move( 0, ply->velY );
    Gravity(dt, ply->velY);

		if( Collision_Horz_Up(ply->sprite.getPosition().x, ply->sprite.getPosition().y + ply->velY*dt, ply->sprite.getTextureRect().width, tileCoord ) ) {
			ply->sprite.setPosition( sf::Vector2f(ply->sprite.getPosition().x, ( tileCoord + 1 ) * TILE_SIZE) );
			ply->sprite.move( sf::Vector2f(ply->velX*dt,0) );
			ply->velY = 0;
		}
	}

	else { //moving down / on the ground
    ply->sprite.move( 0, ply->velY );
    Gravity(dt, ply->velY);
    ply->grounded = false;

    if(ply->velY >= TILE_SIZE)
      ply->velY = TILE_SIZE;

		if( Collision_Horz_Down( ply->sprite.getPosition().x, ply->sprite.getPosition().y + ply->sprite.getTextureRect().height + ply->velY*dt, ply->sprite.getTextureRect().width, tileCoord ) ) { //on the ground
			ply->sprite.setPosition( sf::Vector2f(ply->sprite.getPosition().x, tileCoord * TILE_SIZE - height) );
			ply->velY = 0;
			ply->grounded = true;
		}
	}

}

#endif
