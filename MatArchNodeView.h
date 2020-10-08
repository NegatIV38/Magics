#ifndef MATARCHNODEVIEW_H
#define MATARCHNODEVIEW_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <vector>
#include "Element.h"
#include <cmath>

#define PI_V 3.14159265359

class MatArchNodeView
{
protected:

	sf::CircleShape g_shape;	
	std::vector<sf::RectangleShape> g_inputs;		
private:

	float m_speedX,m_speedY;
	std::vector<std::shared_ptr<MatArchNodeView>> linkedViews;
	std::vector<Element::REACTION> linkedReac;

public:
	static std::map<Element::REACTION,sf::Color> elemColors;
	MatArchNodeView();
	virtual ~MatArchNodeView();

	void printLinks();

	void initShape();
	void initInputs();
	void update();
	void draw(std::shared_ptr<sf::RenderWindow> win);
	void addLink(std::shared_ptr<MatArchNodeView> nodeV,Element::REACTION r);
};

#endif /* MATARCHNODEVIEW_H */
