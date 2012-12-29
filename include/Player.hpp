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
  int current_direction; 

  // private attributes
	int jumpSpeed;;
	bool lock_jump; 

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
	float velX;
	float velY;
	bool grounded;

	bool inuse;

	//functions
	
	Player(sf::Texture & tileset, sf::SoundBuffer & jump, sf::SoundBuffer &pickup) :
        current_direction(PlayerModel::ANIM_RIGHT), 
        lock_jump(false), 
        velX(0), 
        velY(0),
        jumpSpeed(5), 
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

		if( sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && lock_jump == false && grounded ) {
			lock_jump = true;
			m_soundJump.play();
		}

		if(lock_jump) { //if we pressed the jump button
			Jump();
			lock_jump = false;
		}

    if(velX > 0) {
      current_direction = PlayerModel::ANIM_RIGHT; 
    }
    else if(velX < 0) {
      current_direction = PlayerModel::ANIM_LEFT; 
		}
    else { 
      current_direction = PlayerModel::ANIM_IDLE; 
    }

    player_model.UpdateAnimations(current_direction, dt);
	}

};


#endif
