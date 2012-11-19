#ifndef PLAYER_H_
#define PLAYER_H_

#include "Entity.hpp"

class PlayerModel { 

  private: 
    AnimationManager _anim_mgr; 

   void setFrame(const frame_t frame) { 
      sprite->setTextureRect(frame);
    } 
   
   void setAnimation(const int anim, float dt) { 
      setFrame(_anim_mgr.play(anim, dt)); 
    }

  public: 
    sf::Sprite *sprite; 
    
    enum { ANIM_RIGHT=0, ANIM_LEFT, ANIM_IDLE };

    PlayerModel() { 
      //set up animation frames
      _anim_mgr.pushAnim( Animation( 
            {  
              frame_t(0, 65, PLAYER_WIDTH, PLAYER_HEIGHT), 
              frame_t(PLAYER_WIDTH, 65, PLAYER_WIDTH, PLAYER_HEIGHT),
              frame_t(PLAYER_WIDTH*2, 65, PLAYER_WIDTH, PLAYER_HEIGHT)
            }, 0.075f )
          );

      _anim_mgr.pushAnim( Animation( 
            {  
              frame_t(0, 91, PLAYER_WIDTH, PLAYER_HEIGHT),
              frame_t(PLAYER_WIDTH, 91, PLAYER_WIDTH, PLAYER_HEIGHT),
              frame_t(PLAYER_WIDTH*2, 91, PLAYER_WIDTH, PLAYER_HEIGHT)
            }, 0.075 )
          );
    }
    
    void UpdateAnimations(const int anim, float dt)
    {
      switch(anim) { 
        case ANIM_RIGHT: 
         setAnimation(ANIM_RIGHT, dt);
          break; 

        case ANIM_LEFT: 
          setAnimation(ANIM_LEFT,dt);
          break; 

        case ANIM_IDLE: 
          if(_anim_mgr.getAnim() == ANIM_RIGHT) { 
            setFrame(_anim_mgr.getFrame(0)); 
          }
          else {
            setFrame(_anim_mgr.getFrame(2)); 
          }
          break; 
      } 
    }
};

class Player : public DrawableEntity
{

private:

  PlayerModel player_model; 
  int _current_direction; 

  // private attributes
	int _jumpPosition;
	bool _jumpKey;
	int jumpSpeed;;
	bool _lockJump; 

	//sounds
	sf::Sound m_soundJump;
	sf::Sound m_soundPickUp;

private:

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
  _current_direction(PlayerModel::ANIM_RIGHT), 
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

    player_model.sprite = &sprite; 
    player_model.UpdateAnimations(PlayerModel::ANIM_RIGHT, 0.0f);

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
      _current_direction = PlayerModel::ANIM_RIGHT; 
    }
    else if(velX < 0) {
      _current_direction = PlayerModel::ANIM_LEFT; 
		}
    else { 
      _current_direction = PlayerModel::ANIM_IDLE; 
    }

    player_model.UpdateAnimations(_current_direction, dt);
	}

};


#endif
