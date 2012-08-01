// Third Party Library includes 
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// Standard Libary
#include <cmath>
#include <iostream>
#include <cassert>

// My indludes
#include "../include/Constants.h"

//Textures
sf::Texture tileset;        
sf::Texture playerTexture;  
sf::Texture logTexture;
sf::Texture beaverTexture;
sf::Texture backgroundTexture;
sf::Texture treesTexture;
sf::Image   icon;

//sprites
sf::Sprite background;
sf::Sprite trees;


//sounds
sf::SoundBuffer soundJump;
sf::SoundBuffer soundPickUp;
sf::SoundBuffer soundShoot;

sf::Sound PickUpSound;

sf::RenderWindow app;

typedef sf::IntRect frame_t; 

const int CLEAR_TILE = 16;
int map[MAP_HEIGHT][MAP_WIDTH] = 
{ { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 }, 
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
  { 0 , 0 , 0 , 16, 16, 16, 16, 16, 16, 0 , 0 , 0 , 0 , 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 3 , 3 , 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
  { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0 , 3 , 3 , 3 , 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
  { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }		
};  

int collisionMap[MAP_HEIGHT][MAP_WIDTH];

//pallete of tiles available
sf::Sprite pallete[PALLETE_MAX]; 
int numTiles; 

class Entity
{
private:
	//animations indexes
	enum { ANIM_LEFT=0, ANIM_RIGHT, ANIM_JUMP };

	int m_currentFrame; //current frame
	int m_currentAnim;
	
	frame_t m_frames [MAX_ANIMS][MAX_FRAMES];
	float m_nextAnim;

	int m_jumpPosition;
	bool m_jumpKey;
	int jumpHeight;
	bool m_lockJump; 

	//sounds
	sf::Sound m_soundJump;
	sf::Sound m_soundPickUp;
	sf::Sound m_soundShoot;

	//laser stuff
	sf::RectangleShape m_lasers[MAX_LASERS];

	int m_lasersDrawn;
	int m_laserVelX;
	float m_laserTimer;
	bool m_canShoot;

	void UpdateAnimations(float dt)
	{
		static float timer = 0.0f; 
		timer += dt;

		if(velX > 0) {
			m_currentAnim = ANIM_RIGHT;
		}
		else if(velX < 0) {
			m_currentAnim = ANIM_LEFT;
		}

		sprite.setTextureRect( m_frames[m_currentAnim][m_currentFrame] );

		if(timer < m_nextAnim) { return; }
		else timer = 0.0f;

		if(velX != 0) {
			m_currentFrame++;
			if(m_currentFrame > 1) m_currentFrame = 0;
		} 
		else if(m_currentAnim == ANIM_RIGHT ) m_currentFrame = 0;
			else m_currentFrame  = 1; 
	}

	void Jump()
	{
		velY = -3;
	}

	void ShootLaser(float dt)
	{
		if(m_lasersDrawn >= MAX_LASERS)
			return;

		m_soundShoot.play();

		if(m_lasersDrawn > 0)
			m_lasersDrawn++;

    sf::IntRect rect = sprite.getTextureRect(); 
    m_lasers[m_lasersDrawn].move(sf::Vector2f(0, rect.height/2 + 3));

		if(m_currentAnim == ANIM_RIGHT) { // going right 
			m_lasers[m_lasersDrawn].move(sf::Vector2f(rect.width + velX*dt + 3, 0));
			m_laserVelX = 300;
		}
		else if(m_currentAnim == ANIM_LEFT) { //going left
			m_lasers[m_lasersDrawn].move(sf::Vector2f(velX*dt + 3, 0));
			m_laserVelX = -300;
		}
		else {
			m_lasers[m_lasersDrawn].move(sf::Vector2f(rect.width + velX*dt + 3, 0));
			m_laserVelX = 300;
    }

		if(m_lasersDrawn == 0)
			m_lasersDrawn++;
	}

	void UpdateLasers(float dt)
	{
		static float timer = 0.0f;
		timer += dt; 

		for(int i = 0; i < m_lasersDrawn; i++) {
			m_lasers[i].move(m_laserVelX*dt, 0);
			if(m_lasers[i].getPosition().x > WINDOW_WIDTH) m_lasersDrawn--; // This will need to change once you have moving views
			else if(m_lasers[i].getPosition().x < 0) m_lasersDrawn--;
		}

		//lasers only last 10 seconds until they disappear for now
		if(timer >= 10.0f) {
			timer = 0.0f;
			if(m_lasersDrawn > 0)
				m_lasersDrawn--;
		}
	}

public:
	//public variables
	sf::Sprite sprite;
	float nextThink;
	float velX;
	float velY;
	bool grounded;
	bool physicsObject;

	bool inuse;

	//functions
	
	Entity() :m_currentFrame(0), m_lockJump(false), velX(0), velY(0),
            m_currentAnim(ANIM_RIGHT), m_nextAnim(0.2f), nextThink(0.01f), jumpHeight(64), m_jumpPosition(0), m_jumpKey(false),
            physicsObject(true), inuse(false), m_lasersDrawn(0), m_laserTimer(0.2f), m_canShoot(false)
	{
		//set the image
    sf::Texture texture;
    texture.loadFromFile("data/abe.png"); 
		sprite.setTexture(texture);

		//set up animation frames
		m_frames[ANIM_LEFT][0] = frame_t(13, 8, PLAYER_WIDTH, PLAYER_HEIGHT); 
		m_frames[ANIM_LEFT][1] = frame_t(55, 8, PLAYER_WIDTH, PLAYER_HEIGHT);

		m_frames[ANIM_RIGHT][0] = frame_t(95, 8, PLAYER_WIDTH, PLAYER_HEIGHT);
		m_frames[ANIM_RIGHT][1] = frame_t(134, 8, PLAYER_WIDTH, PLAYER_HEIGHT);

		//set the current animations and current frame
		sprite.setTextureRect(m_frames[m_currentAnim][m_currentFrame]); 

		//set up sounds
		m_soundJump.setBuffer(soundJump);
		m_soundPickUp.setBuffer(soundPickUp);	
		m_soundShoot.setBuffer(soundShoot);

		//set up lasers
		for(int i = 0; i < MAX_LASERS; i++) {
			m_lasers[i].setSize(sf::Vector2f(5,2));
      m_lasers[i].setPosition(sf::Vector2f(0,0)); 
      m_lasers[i].setFillColor(sf::Color::Red); 
		}
	}

	void Think(float dt)
	{

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			velX = 220;
		} 
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			velX = -220;
		}else velX = 0;

		if( sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && m_lockJump == false && grounded ) {
			m_lockJump = true;
			m_soundJump.play();
		}

		static float laserTime = 0.0f;
		laserTime += dt;

		if(laserTime >= m_laserTimer) {
			laserTime = 0.0f;
			m_canShoot = true;
		}

		//laser shot
		if( sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && m_canShoot) {
			ShootLaser(dt);
			m_canShoot = false;
		}

		if(m_lockJump) { //if we pressed the jump button
			Jump();
			m_lockJump = false;
		}

		UpdateAnimations(dt);

		UpdateLasers(dt);
	}

	void Draw()
	{
		app.draw(sprite);

		for(int i = 0; i < m_lasersDrawn; i++) {
			app.draw(m_lasers[i]);
		}
	}

};

class ItemBacon 
{

private:

	float m_impulse;
	float m_limit;
	float m_rate;
	bool m_change;

public:

	bool inuse;
	sf::Sprite sprite;

	ItemBacon() :m_limit(0.5f), m_impulse(m_limit), m_rate(2.0f), m_change(false), inuse(false)
	{
		sprite.setTexture(logTexture);
	}

	void Think(float dt)
	{
		sprite.move( 0, m_impulse);

		if(m_impulse >= m_limit) {
			m_change = false;
		}
		else if(m_impulse <= -m_limit) {
			m_change = true;
		}
		
		if(!m_change) {
			m_impulse -= m_rate*dt;
		}
		else if(m_change) {
			m_impulse += m_rate*dt;
		}
	}

	void Draw()
	{
		app.draw(sprite);
	}
};

class Enemy 
{

public:

	sf::Sprite sprite;
	bool inuse;

	int velX;
	int velY;

	Enemy () :velX(2), velY(0)
	{
		sprite.setTexture(beaverTexture);
	}

	void Think(float dt)
	{
		static float inc = 0.0f;

		inc += 1*dt;

		if( inc >= 2*PI) inc = 0.0f;

		sprite.move( std::sin(inc*velX), 0);
	}

	void Draw()
	{
		app.draw(sprite);
	}
};


/*==================================

				UI

  ================================*/

/*==================================

		Checking Collisions

  ================================*/

inline bool Collision_Horz_Down(int x, int y, int width, int & tileY)
{
	int tilePixels = x - (x % TILE_SIZE);
	int testEnd = x + width;

	tileY = y/TILE_SIZE;

	int tileX = tilePixels/TILE_SIZE;

	while( tilePixels <= testEnd) {
		if( map[tileY][tileX] < 16 )
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
		if( map[tileY][tileX] > 16 )
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
		if(map[tileY][tileX] < 16)
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

void PhysicsDos( float dt, Entity* ply )
{
	int tileCoord;

	//X AXIS
	
	if( ply->velX > 0 ) { //moving right

		if( Collision_Ver(ply->sprite.getPosition().x + ply->sprite.getTextureRect().width + ply->velX*dt, ply->sprite.getPosition().y, ply->sprite.getTextureRect().height, tileCoord ) ) {
			ply->sprite.setPosition( sf::Vector2f(tileCoord*TILE_SIZE - ply->sprite.getTextureRect().width, ply->sprite.getPosition().y) );
		}
		else
			ply->sprite.move( ply->velX*dt, 0 );
	}

	else if( ply->velX < 0) { //moving left

		if( Collision_Ver( ply->sprite.getPosition().x + ply->velX*dt, ply->sprite.getPosition().y, ply->sprite.getTextureRect().height, tileCoord ) ) { //collision on the left side
			ply->sprite.setPosition( sf::Vector2f((tileCoord + 1)*TILE_SIZE, ply->sprite.getPosition().y) );	//move to the edge of the tile
		}

		else  ply->sprite.move( ply->velX*dt, 0 ); 
	}

	//Y AXIS

	if( ply->velY < 0) { //moving up

		ply->grounded = false;

		if( Collision_Horz_Up(ply->sprite.getPosition().x, ply->sprite.getPosition().y + ply->velY*dt, ply->sprite.getTextureRect().width, tileCoord ) ) {
			ply->sprite.setPosition( sf::Vector2f(( tileCoord + 1 ) * TILE_SIZE, ply->sprite.getPosition().y) );
			ply->velY = 0;
		}
		
		else {
			ply->sprite.move( 0, ply->velY );
			Gravity(dt, ply->velY);
		}
	}

	else { //moving down / on the ground

		if( Collision_Horz_Down( ply->sprite.getPosition().x, ply->sprite.getPosition().y + ply->sprite.getTextureRect().height + ply->velY*dt, ply->sprite.getTextureRect().width, tileCoord ) ) { //on the ground
			ply->sprite.setPosition( sf::Vector2f(tileCoord * TILE_SIZE - ply->sprite.getTextureRect().height, ply->sprite.getPosition().y) );
			ply->velY = 0;
			ply->grounded = true;
		}
		else { //falling (in air)
			ply->sprite.move( 0, ply->velY );
			Gravity(dt, ply->velY);
			ply->grounded = false;

			if(ply->velY >= TILE_SIZE)
				ply->velY = TILE_SIZE;

		}
	}

}

/*=================================
	
	World specific game code

==================================*/

inline void OnBacon(ItemBacon & bacon)
{
	PickUpSound.play();
	bacon.inuse = false;
}

inline void CenterCameraView(const sf::Sprite & sprite)
{
	sf::View view = app.getView();

	view.setCenter( sprite.getPosition().x, view.getCenter().y );

	app.setView( view );
}


inline void Parallax(sf::Sprite & p, int iposx, const sf::View & view, float distance)
{
	sf::Vector2f center = view.getCenter();

	float viewX = center.x - WINDOW_WIDTH/2;

	float _x = viewX - ( 1 / distance ) * viewX + iposx;

	p.setPosition(sf::Vector2f(_x, p.getPosition().y));
}

/*==================================

		Some map editing stuff

  ================================*/

void LocaltoWorld(int &lx, int &ly) {

	sf::Vector2f final = app.convertCoords(sf::Vector2i(lx, ly));

	lx = final.x;
	ly = final.y;

	lx /= TILE_SIZE;
	ly /= TILE_SIZE;
}

void SetTile(int x, int y, int value, int (&_map)[MAP_HEIGHT][MAP_WIDTH]) 
{
	if(x*y < 0 || x*y > MAP_WIDTH*MAP_HEIGHT) return;

	_map[y][x] = value;
}


void LoadPallete()
{
	//load the tileset
	tileset.loadFromFile("Data/Tileset1.png");

	int width = tileset.getSize().x/TILE_SIZE; 
	int height = tileset.getSize().y/TILE_SIZE; 

	for(int y = 0; y < height; y++)
		for(int x = 0; x < width; x++) {
			pallete[x+y*width].setTexture(tileset);
			pallete[x+y*width].setTextureRect(sf::IntRect(TILE_SIZE*x, TILE_SIZE*y, TILE_SIZE, TILE_SIZE));
		}
	numTiles = width*height;
}

/*==================================

   Initializing the system and game

  ================================*/

bool quit;

inline void Init()
{
	//init sfml window
	app.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "Platformer test", sf::Style::Close);
	app.setFramerateLimit(60);

	//for the game loop
	quit = false; 
	
	//set up images
	icon.loadFromFile("Data/nope.bmp");
	app.setIcon(32, 32, icon.getPixelsPtr());

	LoadPallete();
	playerTexture.loadFromFile("Data/abe.png");
	logTexture.loadFromFile("Data/log.png");
	beaverTexture.loadFromFile("Data/beaver.png");
	backgroundTexture.loadFromFile("Data/blueroundflowers_tile.jpg");
	treesTexture.loadFromFile("Data/trees.png");

	//set up background
	background.setTexture(backgroundTexture);
	trees.setTexture(treesTexture);

	//set up sounds
	soundJump.loadFromFile("data/sounds/jump.wav");
	soundPickUp.loadFromFile("data/sounds/pickup.wav");
	soundShoot.loadFromFile("data/sounds/shoot.wav");

	PickUpSound.setBuffer(soundPickUp);
}

/*==================================

	  Simple Drawing functions

  ================================*/

inline void DrawMap()
{
	for(int y = 0; y < MAP_HEIGHT; ++y) {

		for(int x = 0; x < MAP_WIDTH; ++x)
		{
			pallete[map[y][x]].setPosition(sf::Vector2f(x*TILE_SIZE, y*TILE_SIZE)); 
			app.draw(pallete[map[y][x]]);
		}
	}
}

inline void Close()
{
	app.close(); 
}

int main()
{
	Init();

  sf::Clock clock; 

	Entity ply; 
	Enemy enemy;
	enemy.sprite.setPosition(sf::Vector2f(8*TILE_SIZE, 18*TILE_SIZE));
	ItemBacon bacon[MAX_BACON];
	bacon[0].inuse = true;
	bacon[0].sprite.setPosition(sf::Vector2f(16*TILE_SIZE, 16*TILE_SIZE));
	int numBacons = 0; 

	int currentTile = 0;

  float current_time  = 0;  
  float previous_time = 0;  
  float dt            = 0;  

	sf::Event event; 
	while(!quit) {
    //Handle time
    current_time = clock.getElapsedTime().asSeconds(); 
    dt = current_time - previous_time; 
    previous_time = clock.getElapsedTime().asSeconds(); 

		while(app.pollEvent(event)) {
      if( event.type == sf::Event::Closed )
        quit = true; 
      if( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape )
				quit = true; 

			if( event.type == sf::Event::MouseButtonPressed ) {

				if( event.mouseButton.button == sf::Mouse::Left ) {
          sf::Vector2i mousepos = sf::Mouse::getPosition();
					int _x = mousepos.x;
					int _y = mousepos.y;

					LocaltoWorld(_x, _y);

					SetTile(_x, _y, currentTile, map);
				}

				if( event.mouseButton.button == sf::Mouse::Right ) {
          sf::Vector2i mousepos = sf::Mouse::getPosition();
					numBacons++;

					if(numBacons >= MAX_BACON) { 
						numBacons = MAX_BACON-1;
					}

					bacon[numBacons].inuse = true;

					int bx = mousepos.x;
					int by = mousepos.y;

					LocaltoWorld( bx, by );
					bacon[numBacons].sprite.setPosition( sf::Vector2f(bx*TILE_SIZE, by*TILE_SIZE) );
				}
			}
			if(event.type == sf::Event::MouseWheelMoved) {
				currentTile += event.mouseWheel.delta;
        if(currentTile < 0) currentTile = 0;
				if(currentTile > 31) currentTile = 31;
			}
		}

		CenterCameraView( ply.sprite );

		Parallax(background, 0, app.getView(), 10.0f);
		Parallax(trees, 0, app.getView(), 2.0f);

		ply.Think(dt);

		enemy.Think(dt);

		for(int i = 0; i < MAX_BACON; i++) {

			if(bacon[i].inuse) {
				bacon[i].Think(dt);

				if( RectCollision( ply.sprite, bacon[i].sprite ) ) {
					OnBacon(bacon[i]);
					numBacons--;
				}
			}
		}

		PhysicsDos(dt, &ply);

		app.clear(sf::Color(255,255,255));

		app.draw( background );
		app.draw( trees );

		DrawMap();

		for(int i = 0; i < MAX_BACON; i++) {

			if(bacon[i].inuse) 
				bacon[i].Draw();
		}

		enemy.Draw();

		ply.Draw();

		sf::Vector2i mouse_pos = sf::Mouse::getPosition(app);
		pallete[currentTile].setPosition( sf::Vector2f(mouse_pos.x, mouse_pos.y) );
		app.draw( pallete[currentTile] );
		app.display();
	}

	Close();

	return EXIT_SUCCESS;
}
