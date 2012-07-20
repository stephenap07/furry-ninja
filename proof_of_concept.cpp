/* Notes for proof of concept
 */

#include <map>
#include <vector>
#include <SFML/Graphics.hpp>

// Aplication code 

enum {
  PHYSICS = 0x1,
  RENDER  = 0x2,
  X       = 0x4
};  

class Controller { 
  public: 
  virtual int id() = 0; 
}; 

class Universe { 
}; 

namespace sys 
{ 
  namespace physics  
  { 
    // Time
    inline const float & get_time() { 
    }

    // Interpolation
    inline float linear_int(float dt, float x, float target) { 
      if( x < target ) return x + x*dt; 
      return 0; 
    }

    enum { 
      NONE  = 0x0; 
      LEFT  = 0x1, 
      RIGHT = 0x2,
      DOWN  = 0x4,
      UP    = 0x8 
    };

    // Collision
    int getCollisionSide(const sf::FloatRect & a, const sf::FloatRect & b) { 
      if(!a.intersects(b)) return 0;   

      // Check for intersection on the left side
      // Check for intersection on the right side
      // Check for intersection on the bottom side
      // Check for intersection on the top side
    }
  } 

  namespace Render 
  { 
  } 

}

namespace controls { 
  inline sf::Vector2f move_to_vector(float dt, float x, float y, const sf::Vector2f & target) { 
    sf::Vector2f vec( sys::physics::linear_int(dt,x,target.x), 
                      sys::physics::linear_int(dt,y,target.y) ); 
    return vec; 
  }
}

class Paddle :public Controller { 
  private: 
    // Should be a shared pointer
    sf::Sprite _sprite; 
  public: 
  virtual int id() { return 0; }
  const sf::Sprite & getSprite() { return _sprite; }
  void setSprite(const sf::Sprite & sprite) { _sprite = sprite; }
  virtual void inputHandle() { 
    _sprite.move(controls::paddle_move(dt, _sprite.getGlobalBounds().x, 0, sf::mouse::)); 
  }
}

int think()
{
  if( !Universe.think() || 
      !Universe.draw() ) 
  { 
    return 0; 
  }

  return 1; 
}
