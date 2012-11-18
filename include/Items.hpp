#ifndef ITEMS_H_
#define ITEMS_H_

#include "Entity.hpp"

class ItemBacon : public DrawableEntity
{

private:

	float m_impulse;
	float m_limit;
	float m_rate;
	bool m_change;

public:

	bool inuse;

	ItemBacon(sf::Texture & tileset) :m_limit(0.5f), m_impulse(m_limit), m_rate(2.0f), m_change(false), inuse(false)
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

#endif
