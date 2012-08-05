#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <functional>

#define BLOCK_WIDTH_SIZE 10 
#define BLOCK_HEIGHT_SIZE 2.5 

typedef int tag; 

struct CollisionMesage { 
  sf::FloatRect collision_bounds;  
  tag id; 

  CollisionMesage(sf::FloatRect bounds, tag id=-1) : id(id) {}
  CollisionMesage() : id(-1) {}
}; 


class BlockMap { 
  private:
    std::vector<sf::RectangleShape> _renderObjects;

    float _width; 
    float _height; 
    int   _id;  

    /* This function returns the width of the map 
     */ 
    bool calcWidth(const sf::RectangleShape &r, const sf::RectangleShape & b) { 
      // Get the resultant right coordinate and left coordinate
      float big_x_pos = r.getPosition().x + _renderObjects[0].getGlobalBounds().width; 
      float small_x_pos = r.getPosition().x; 

      // Get the compared right and left coordinates 
      float bx_pos = block.getGlobalBounds().left + b.getGlobalBounds().width; 
      float sx_pos = b.getPosition().x; 

      return big_x_pos - small_x_pos; 
    }

    const sf::RectangleShape getBounds(const sf::RectangleShape & initial_b,std::function<bool(const sf::RectangleShape &r, const sf::RectangleShape
     & b)> f) const { 
     sf::RectangleShape result = initial_b; 

      for( auto & block : _renderObjects ) { 
        if( f(result, block) ) { 
          result = block;
        }
      }

      return result; 
    }

  public:
    BlockMap(float offset_x, float offset_y, int count, std::function<sf::Vector2f(int)> f) 
    {
      _id = 0; 
      for(int i=0; i<count; ++i) { 
        _renderObjects.push_back(sf::RectangleShape(sf::Vector2f(BLOCK_WIDTH_SIZE, BLOCK_HEIGHT_SIZE)));
        _renderObjects[i].setPosition(f(i)); 
        _renderObjects[i].move(offset_x, offset_y); 
      }
    }

    const std::vector<sf::RectangleShape> & getRenderObjects() {
      return _renderObjects;
    }

    // Return object ids and collision bounds for whatever 
    const std::vector<CollisionMesage> getCollisionMessages(const sf::FloatRect collbox) { 
      std::vector<CollisionMesage> messages; 
      for( auto & block : _renderObjects ) { 
       if( block.getGlobalBounds().intersects(collbox) ) { 
          CollisionMesage message; 
          messages.push_back(message); 
          message.collision_bounds = block.getGlobalBounds();  
          message.id = _id; 
       }
      }
      return messages; 
    }
};

class Ball 
{
  private: 
    sf::RectangleShape _ball; 
    float _speedX; 
    float _speedY; 

  public: 
    Ball(float x, float y) :_speedX(0), _speedY(15.0f) 
    {
      _ball = sf::RectangleShape(sf::Vector2f(2.0f, 2.0f));    
      _ball.setPosition(x,y); 
    }

    const sf::RectangleShape getRenderObject() {
      return _ball; 
    }

    void think(float dt) { 
      _ball.move(_speedX*dt, _speedY*dt);  
    }

    const sf::FloatRect getCollisionBounds() { 
      return _ball.getGlobalBounds(); 
    }

    CollisionMesage getCollisionMessages(const sf::FloatRect & collbox) { 
      //check if the two boxes collide 
      CollisionMesage message; 
      if(_ball.getGlobalBounds().intersects(collbox)) { 
        message.collision_bounds = _ball.getGlobalBounds(); 
        message.id = 1; 
      }

      return message; 
    }
}; 

int main()
{
  sf::RenderWindow window(sf::VideoMode(300, 200), "breakout fool!"); 
  sf::Clock Clock; 

  //Rectangle object 
  
  float  width          =  10.0f;
  float  height         =  2.5f;
  float  offset_vert    =  -20.0f;
  float  offset_horiz   =  -width/2.0f;
  float  middle_screen  =  window.getView().getSize().x/2;
  float  end_screen     =  window.getView().getSize().y;
  float  initial_y      =  end_screen + offset_vert;
  float  initial_x      =  middle_screen + offset_horiz;

  sf::RectangleShape rectangle;
  rectangle.setSize(sf::Vector2f(width,height));
  rectangle.setOutlineColor(sf::Color::Red);
  rectangle.setOutlineThickness(1);
  rectangle.setPosition(initial_x, initial_y);

  sf::RectangleShape collided(sf::Vector2f(10,10));
  collided.setFillColor(sf::Color::Blue); 

  BlockMap map(100, 30, 10, [](int i) -> sf::Vector2f{ 
          return sf::Vector2f(i*BLOCK_WIDTH_SIZE, (i % 2)*BLOCK_HEIGHT_SIZE ); 
        }); 
  Ball ball(90, 50); 

  while (window.isOpen())
  {  

    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    float mouse_x = sf::Mouse::getPosition(window).x;
    float dt = Clock.getElapsedTime().asSeconds(); 
    Clock.restart(); 

    rectangle.setPosition(sf::Vector2f(mouse_x, initial_y)); 
    ball.think(dt); 
    std::vector<sf::RectangleShape> blocks = map.getRenderObjects(); 

    sf::FloatRect rect = ball.getCollisionBounds(); 
    if( map.getCollisionMessages(rect).size() ) { 
    }
    else { 
    }
    if( ball.getCollisionMessages(rectangle.getGlobalBounds()).id >= 0 ) { 
    }

    window.clear();
    window.draw(rectangle);
    window.draw(ball.getRenderObject()); 
    for( auto block : map.getRenderObjects() ) { 
      window.draw(block); 
    }
    window.display();
  }

  return 0;
}


