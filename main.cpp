#include <SFML/Graphics.hpp>

int main()
{
  sf::RenderWindow window(sf::VideoMode(300, 200), "SFML works!"); 

  //Rectangle object 
  
  sf::RectangleShape rectangle;
  rectangle.setSize(sf::Vector2f(5,5));
  rectangle.setOutlineColor(sf::Color::Red);
  rectangle.setOutlineThickness(1);
  rectangle.setPosition(10, 20);

  sf::Clock clock; 
  sf::Time time; 
  
  int side = 1; 
  int updown = 0; 

  int left;
  int right;
  int up;    
  int down;   

  float speed = 5.0f; 

  while (window.isOpen())
  {  
    time = clock.restart(); 

    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    
    side = left  ? -1 : side; 
    side = right ?  1 : side; 

    updown = up   ? -1 : updown; 
    updown = down ?  1 : updown; 

    rectangle.move(speed*side*time.asSeconds(), speed*updown*time.asSeconds()); 

    window.clear();
    window.draw(rectangle);
    window.display();
  }

  return 0;
}


