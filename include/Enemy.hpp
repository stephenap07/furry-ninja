#ifndef ENEMY_H_
#define ENEMY_H_

/* Simple Enemy 
 * Just goes back and forth 
 */
class Enemy 
{

public:

	sf::Sprite sprite;
	bool inuse;

	int velX;
	int velY;

	Enemy (sf::Texture & tileset) :velX(2), velY(0)
	{
		sprite.setTexture(tileset);
    sprite.setTextureRect(sf::IntRect(153,58,11,10));
	}

	void think(float dt)
	{
		static float inc = 0.0f;

		inc += 1*dt;

		if( inc >= 2*PI) inc = 0.0f;

		sprite.move( std::sin(inc*velX), 0);
	}

};

#endif
