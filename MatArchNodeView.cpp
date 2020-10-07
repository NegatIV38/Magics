#include "MatArchNodeView.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <memory>

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



MatArchNodeView::MatArchNodeView(){

	initShape();
}

MatArchNodeView::~MatArchNodeView(){

}

void MatArchNodeView::initShape(){
	g_shape.setRadius(10);
	g_shape.setFillColor(sf::Color::White);
	g_shape.setPosition(rand()%500,rand()%500);
	g_shape.setOrigin(10,10);
}

void MatArchNodeView::draw(std::shared_ptr<sf::RenderWindow> win){
	sf::VertexArray lines(sf::Lines);
	for(std::size_t i = 0; i < linkedViews.size(); i++){
		lines.append(sf::Vertex(g_shape.getPosition(),elemColors.at(linkedReac.at(i))) );
		lines.append(sf::Vertex(linkedViews.at(i)->g_shape.getPosition(),elemColors.at(linkedReac.at(i))));
	
	}
	win->draw(lines);
	win->draw(g_shape);	
}

void MatArchNodeView::update(){

}

void MatArchNodeView::addLink(std::shared_ptr<MatArchNodeView> nodeV,Element::REACTION r){
	linkedViews.push_back(nodeV);
	linkedReac.push_back(r);

}
