// Third Party Library includes 
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// Standard Libary
#include <cmath>
#include <iostream>
#include <cassert>
#include <vector>

// My includes
#include "../include/Constants.hpp"
#include "../include/AnimationManager.hpp"
#include "../include/Entity.hpp"
#include "../include/Player.hpp"
#include "../include/Items.hpp"
#include "../include/Enemy.hpp"
#include "../include/Collision.hpp"

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

//pallete of tiles available
sf::Sprite pallete[PALLETE_MAX]; 
int numTiles; 

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

	sf::Vector2f final = app.mapPixelToCoords(sf::Vector2i(lx, ly));

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

	Player ply(tileset, soundJump, soundPickUp); 
	Enemy enemy(tileset);
	enemy.sprite.setPosition(sf::Vector2f(8*TILE_SIZE, 13*TILE_SIZE));

	std::vector<ItemBacon> bacon(MAX_BACON, ItemBacon(tileset));

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
		pallete[currentTile].setPosition( app.mapPixelToCoords(mouse_pos) );
		app.draw( pallete[currentTile] );
		app.display();
	}

	Close();

	return EXIT_SUCCESS;
}
