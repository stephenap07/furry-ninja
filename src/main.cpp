// Third Party Library includes 
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// Standard Libary
#include <cmath>
#include <iostream>
#include <cassert>
#include <vector>

// My indludes
#include "../include/Constants.h"

//Textures
sf::Texture tileset;        
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

const int CLEAR_TILE = 16;
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

//pallete of tiles available
sf::Sprite pallete[PALLETE_MAX]; 
int numTiles; 

// Animation Typedefs 
typedef sf::IntRect frame_t; 
typedef std::vector<frame_t> animation_t; ; 

struct Animation { 
  Animation(const animation_t & a, float t = 0.05f, int cur_frame=0) 
  : _anim(a), _time_limit(t), _current_frame(cur_frame), _current_time(0.0f) {}; 

  void setAnimation(const animation_t & a) { 
    _anim = a; 
  }

  const frame_t & getNextFrame(float dt) { 
    _current_time += dt;  

    if(_current_time > _time_limit) { 
      _current_time = 0.0f; 
      if(++_current_frame > (_anim.size()-1)) {
        restart(); 
      }
    }
    
    return _anim[_current_frame]; 
  }

  const frame_t & getFrame(int index) { 
    if( index < _anim.size() ) { 
      return _anim[index]; 
    }
    else { 
      std::cout << "Programmer error\n"; 
    }
  }

  void restart() { 
    _current_frame = 0; 
    _current_time = 0.0f; 
  }

  private: 
    animation_t _anim;         
    int _current_frame; 
    float _current_time; 
    float _time_limit; 
};

class AnimationManager { 
  private: 

    // Variables
    std::vector<Animation> _animations; 
    int _current_anim; 

    const frame_t & getNextFrame(int anim, float dt) { 
      if(_current_anim <= (_animations.size()-1)) {
        return _animations[_current_anim].getNextFrame(dt); 
      }
      else { 
        std::cout << "programmer error\n"; 
      }
    }
 public: 
    AnimationManager() :_current_anim(0) { }
    int pushAnim(const Animation &anim) { 
      _animations.push_back(anim); 
      return (_animations.size()-1); 
    }
    void setAnim(int anim) { 
      if( _current_anim != anim ) { 
        if(_current_anim <= (_animations.size()-1)) {
          _animations[_current_anim].restart(); 
        }
        _current_anim = anim; 
      }
    }
    int getAnim() { 
      return _current_anim; 
    }
    const frame_t & getFrame(int index) { 
      if(_current_anim < _animations.size()) { 
        return _animations[_current_anim].getFrame(index); 
      }
    }
    const frame_t & play(int anim, float dt) { 
      setAnim(anim); 
      return getNextFrame(anim, dt);
    }

};

class Entity { 
  public:
    virtual void think(float dt) {}; 
    virtual void contact(const Entity &other) {}; 
    virtual void action(const Entity &other) {};
};

class DrawableEntity : public Entity{ 
  public:
    sf::Sprite sprite; 
};

class Player : public DrawableEntity
{

private:
	//animations indexes
	enum { ANIM_RIGHT=0, ANIM_LEFT,
         ANIM_IDLE
       };
  int _current_direction; 

  AnimationManager _anim_mgr; 

  // private attributes
	int m_jumpPosition;
	bool m_jumpKey;
	int jumpSpeed;;
	bool m_lockJump; 

	//sounds
	sf::Sound m_soundJump;
	sf::Sound m_soundPickUp;

private:

	void UpdateAnimations(float dt)
	{
		if(_current_direction == ANIM_RIGHT) {
			sprite.setTextureRect(_anim_mgr.play(ANIM_RIGHT,dt));
		}
		else if(_current_direction == ANIM_LEFT) {
			sprite.setTextureRect(_anim_mgr.play(ANIM_LEFT,dt));
		}
    else if(_current_direction == ANIM_IDLE) { 
      if(_anim_mgr.getAnim() == ANIM_RIGHT) { 
        sprite.setTextureRect(_anim_mgr.getFrame(0)); 
      }
      else {
        sprite.setTextureRect(_anim_mgr.getFrame(2)); 
      }
    }
	}

	void Jump()
	{
		velY = -jumpSpeed;
	}

public:
	//public variables
	sf::Sprite sprite;
	float nextthink;
	float velX;
	float velY;
	bool grounded;
	bool physicsObject;

	bool inuse;

	//functions
	
	Player() :_current_direction(ANIM_RIGHT), m_lockJump(false), velX(0), velY(0),
            nextthink(0.01f), jumpSpeed(5), m_jumpPosition(0), m_jumpKey(false),
            physicsObject(true), inuse(false)	
  {
		//set the texture
		sprite.setTexture(tileset);

		//set up animation frames
		_anim_mgr.pushAnim( Animation( {  frame_t(0, 65, PLAYER_WIDTH, PLAYER_HEIGHT), 
                                      frame_t(PLAYER_WIDTH, 65, PLAYER_WIDTH, PLAYER_HEIGHT),
                                      frame_t(PLAYER_WIDTH*2, 65, PLAYER_WIDTH, PLAYER_HEIGHT)
                                   }, 0.075f )
                      );

		_anim_mgr.pushAnim( Animation( {  frame_t(0, 91, PLAYER_WIDTH, PLAYER_HEIGHT),
                                      frame_t(PLAYER_WIDTH, 91, PLAYER_WIDTH, PLAYER_HEIGHT),
                                      frame_t(PLAYER_WIDTH*2, 91, PLAYER_WIDTH, PLAYER_HEIGHT)
                                   }, 0.075 )
                      );

		//set the current animations and current frame
    sprite.setTextureRect(_anim_mgr.play(ANIM_RIGHT,0.0f)); 

		//set up sounds
		m_soundJump.setBuffer(soundJump);
		m_soundPickUp.setBuffer(soundPickUp);	
	}

	virtual void think(float dt)
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

		if(m_lockJump) { //if we pressed the jump button
			Jump();
			m_lockJump = false;
		}

    if(velX > 0) {
      _current_direction = ANIM_RIGHT; 
    }
    else if(velX < 0) {
      _current_direction = ANIM_LEFT; 
		}
    else { 
      _current_direction = ANIM_IDLE; 
    }

		UpdateAnimations(dt);

	}

};

class ItemBacon : public DrawableEntity
{

private:

	float m_impulse;
	float m_limit;
	float m_rate;
	bool m_change;

public:

	bool inuse;

	ItemBacon() :m_limit(0.5f), m_impulse(m_limit), m_rate(2.0f), m_change(false), inuse(false)
	{
		sprite.setTexture(tileset);
    sprite.setTextureRect(sf::IntRect(140,58,11,10));
	}

	void think(float dt)
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
	tileset.loadFromFile("data/platformer.png");

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

	LoadPallete();

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

	Player ply; 
	Enemy enemy;
	enemy.sprite.setPosition(sf::Vector2f(8*TILE_SIZE, 13*TILE_SIZE));
	ItemBacon bacon[MAX_BACON];
	bacon[0].inuse = true;
	bacon[0].sprite.setPosition(sf::Vector2f(16*TILE_SIZE, 13*TILE_SIZE));
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
          sf::Vector2i mousepos = sf::Mouse::getPosition(app);
					int _x = mousepos.x;
					int _y = mousepos.y;

					LocaltoWorld(_x, _y);

					SetTile(_x, _y, currentTile, map);
				}

				if( event.mouseButton.button == sf::Mouse::Right ) {
          sf::Vector2i mousepos = sf::Mouse::getPosition(app);
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

		ply.think(dt);

		enemy.think(dt);

		for(int i = 0; i < MAX_BACON; i++) {

			if(bacon[i].inuse) {
				bacon[i].think(dt);

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
				app.draw(bacon[i].sprite);
		}

		app.draw(enemy.sprite);

		app.draw(ply.sprite);

		sf::Vector2i mouse_pos = sf::Mouse::getPosition(app);
		pallete[currentTile].setPosition( app.convertCoords(mouse_pos) );
		app.draw( pallete[currentTile] );
		app.display();
	}

	Close();

	return EXIT_SUCCESS;
}
