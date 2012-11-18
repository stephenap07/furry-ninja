#ifndef PLAYER_H_
#define PLAYER_H_

#include "Entity.hpp"

class Player : public DrawableEntity
{

private:

	//animations indexes
  enum { ANIM_RIGHT=0, ANIM_LEFT, ANIM_IDLE };

  int _current_direction; 

  AnimationManager _anim_mgr; 

  // private attributes
	int _jumpPosition;
	bool _jumpKey;
	int jumpSpeed;;
	bool _lockJump; 

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
	
	Player(sf::Texture & tileset, sf::SoundBuffer & jump, sf::SoundBuffer &pickup) :
  _current_direction(ANIM_RIGHT), 
  _lockJump(false), 
  velX(0), 
  velY(0),
  nextthink(0.01f), 
  jumpSpeed(5), 
  _jumpPosition(0), 
  _jumpKey(false),
  physicsObject(true), 
  inuse(false)	
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
		m_soundJump.setBuffer(jump);
		m_soundPickUp.setBuffer(pickup);	
	}

	virtual void think(float dt)
	{

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			velX = 220;
		} 
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			velX = -220;
		}else velX = 0;

		if( sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && _lockJump == false && grounded ) {
			_lockJump = true;
			m_soundJump.play();
		}

		if(_lockJump) { //if we pressed the jump button
			Jump();
			_lockJump = false;
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


#endif
