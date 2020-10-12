#include "MatArchNodeView.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <memory>
#include <string>

std::map<Element::REACTION,sf::Color> MatArchNodeView::elemColors = {
	{Element::REACTION::ATTRACTION, sf::Color(102,153,153)},
	{Element::REACTION::CORROSION, sf::Color(100,100,50)},
	{Element::REACTION::DECHARGE, sf::Color(255,255,255)},
	{Element::REACTION::ELEC_STATIC, sf::Color(255,255,100)},
	{Element::REACTION::EXPLOSION, sf::Color(255,150,0)},
	{Element::REACTION::GEL, sf::Color(127,255,255)},
	{Element::REACTION::INFLAMMATION, sf::Color(255,0,0)},
	{Element::REACTION::LIQUEFACTION, sf::Color(0,100,255)},
	{Element::REACTION::RADIOACTIVITE, sf::Color(200,200,0)},
	{Element::REACTION::REPULSION, sf::Color(115,102,153)},
	{Element::REACTION::SOLIDIFICATION, sf::Color(102,20,0)},
	{Element::REACTION::SOUFFLE, sf::Color(102,255,50)},
	{Element::REACTION::SUBLIMATION, sf::Color(204,50,255)},
	{Element::REACTION::TOXICITE, sf::Color(0,127,0)},
	{Element::REACTION::__COUNT, sf::Color(0,0,0)},
};



MatArchNodeView::MatArchNodeView(std::shared_ptr<MatArchNode> node):m_node(node){
	m_font.loadFromFile("Minecraftia-Regular.ttf");

	g_lines = std::make_shared<sf::VertexArray>(sf::Lines);	
	m_speedX = -250+rand()%500;
	m_speedY = -250+rand()%500;
	m_speedX /= 10000;
	m_speedY /= 10000;
	initShape();
	initInputs();
	m_id = -1;
}

MatArchNodeView::~MatArchNodeView(){

}

void MatArchNodeView::initShape(){
	g_shape.setRadius(10);
	g_shape.setFillColor(sf::Color::White);
	g_shape.setPosition(rand()%500,rand()%500);
	g_shape.setOrigin(g_shape.getRadius(),g_shape.getRadius());
}
void MatArchNodeView::initInputs(){
	for(int i =0; i < Element::REACTION::__COUNT; i++){
		g_inputs.push_back(sf::RectangleShape(sf::Vector2f(4,4)));
		g_inputs.back().setOrigin(2,2);
		g_inputs.back().setFillColor(elemColors.at(Element::REACTION(i)));
	}
}


void MatArchNodeView::printLinks(){
	std::cout << std::endl <<"Reaction LIST begin : "<< m_id << std::endl;
	for(std::size_t i = 0; i < linkedReac.size(); i++){
		std::cout <<i<<": " << Element::reacToStr(linkedReac.at(i)) << "\t" << linkedViews.at(i)->m_id <<std::endl;
	}
	std::cout <<std::endl;


}

void MatArchNodeView::setID(int id){
	m_id = id;
}

void MatArchNodeView::draw(std::shared_ptr<sf::RenderWindow> win){
	win->draw(*g_lines);
	win->draw(g_shape);	
	for (auto input : g_inputs) {
		win->draw(input);
	}
	for (auto v : g_values) {
		win->draw(v);
	}
}

void MatArchNodeView::update(){

	float delta = (2*PI_V)/Element::REACTION::__COUNT;
	g_shape.setPosition(g_shape.getPosition() + sf::Vector2f(m_speedX,m_speedY));	
	if(g_shape.getPosition().x < m_pos.x-m_bounds.x/2 || g_shape.getPosition().x>m_pos.x+m_bounds.x/2){
		m_speedX *=-1;
	}
	if(g_shape.getPosition().y < m_pos.y-m_bounds.y/2 || g_shape.getPosition().y>m_pos.y+m_bounds.y/2){
		m_speedY *= -1;
	}
	g_lines->clear();
	g_values.clear();
	for(std::size_t i = 0; i < linkedViews.size(); i++){
		g_lines->append(sf::Vertex(g_inputs.at(int(linkedReac.at(i))).getPosition(),elemColors.at(linkedReac.at(i))));

		g_lines->append(sf::Vertex(linkedViews.at(i)->g_inputs.at(int(linkedReac.at(i))).getPosition(),elemColors.at(linkedReac.at(i))));

	}
	for(int i =0; i < Element::REACTION::__COUNT; i++){
		bool alreadyDrawn = false;
		for(std::size_t j = 0; j < linkedReac.size(); j++){
			if(i == linkedReac.at(j)){
				alreadyDrawn = true;
			}
		}
		if(!alreadyDrawn){
			g_lines->append(sf::Vertex(g_inputs.at(i).getPosition(),elemColors.at(Element::REACTION(i))));
			float	px = 50*std::cos(i*delta);
			float 	py = 50*std::sin(i*delta);
			g_lines->append(sf::Vertex(g_shape.getPosition() + sf::Vector2f(px,py),elemColors.at(Element::REACTION(i))));
			g_values.push_back(sf::Text());
			g_values.back().setFont(m_font);
			g_values.back().setCharacterSize(18);
			g_values.back().setPosition(g_shape.getPosition() + sf::Vector2f(px,py));
			g_values.back().setFillColor(elemColors.at(Element::REACTION(i)));
			int val = int(std::nearbyint(m_node->getElementRawValue(Element::REACTION(i))));
			g_values.back().setString(std::to_string(val));
			if(val < 0){
				g_values.back().setOrigin(g_values.back().getCharacterSize()/2,0);
			}

		}
	}

	for(std::size_t i = 0; i < g_inputs.size(); i++){
		g_inputs.at(i).setPosition(g_shape.getPosition() + sf::Vector2f(g_shape.getRadius()*std::cos(i*delta),g_shape.getRadius()*std::sin(i*delta)));
	}
}


void MatArchNodeView::addLink(std::shared_ptr<MatArchNodeView> nodeV,Element::REACTION r){
	linkedViews.push_back(nodeV);
	linkedReac.push_back(r);

}

void MatArchNodeView::setPosition(sf::Vector2f pos, sf::Vector2f dim ){
	m_pos = pos;
	m_bounds = dim;
	g_shape.setPosition(pos);
}
