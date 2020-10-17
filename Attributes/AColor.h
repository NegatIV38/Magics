#ifndef ACOLOR_H
#define ACOLOR_H

#include "Attribute.h"
#include <SFML/Graphics.hpp>

class AColor : Attribute 
{
private:
	sf::Color m_color;

public:
	AColor();
	AColor(sf::Color c);
	virtual ~AColor();

	void setColor(sf::Color c);
	sf::Color getColor();
};

#endif /* ACOLOR_H */
