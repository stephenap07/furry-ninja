#ifndef ENTITY_H_
#define ENTITY_H_

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

#endif
