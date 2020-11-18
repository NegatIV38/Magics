#ifndef MATARCHNODEVIEW_H
#define MATARCHNODEVIEW_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <vector>
#include "Element.h"
#include <cmath>

#include "MatArchNode.h"

#define PI_V 3.14159265359

class MatArchNodeView
{
protected:
	sf::CircleShape g_shape;	
	int m_id;
	std::vector<sf::RectangleShape> g_inputs;		
	std::vector<sf::Text> g_values;
private:
	sf::Font m_font;
	std::shared_ptr<MatArchNode> m_node;
	float m_speedX,m_speedY;
	std::vector<std::shared_ptr<MatArchNodeView>> linkedViews;
	std::vector<REAC> linkedReac;
	sf::Vector2f m_pos,m_bounds;
	std::shared_ptr<sf::VertexArray> g_lines;
public:
	static std::map<REAC,sf::Color> elemColors;
	MatArchNodeView(std::shared_ptr<MatArchNode> node);
	MatArchNodeView(std::shared_ptr<MatArchNode> node,sf::Vector2f pos);
	virtual ~MatArchNodeView();

	void printLinks();
	void setPosition(sf::Vector2f pos, sf::Vector2f dim = sf::Vector2f(600,600));
	sf::Vector2f getPosition();
	void initShape();
	void initInputs();
	void initLines();
	void setID(int id);
	void update();
	void draw(std::shared_ptr<sf::RenderWindow> win);
	void addLink(std::shared_ptr<MatArchNodeView> nodeV,REAC r);
	void removeLink();
	void clearLinks();
};

#endif /* MATARCHNODEVIEW_H */
