#include "MatArchNodeView.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
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

	m_speedX = rand()%100;
	m_speedY = rand()%100;
	m_speedX /= 10000;
	m_speedY /= 10000;
	initShape();
	initInputs();
}

MatArchNodeView::~MatArchNodeView(){

}

void MatArchNodeView::initShape(){
	g_shape.setRadius(20);
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
	std::cout << std::endl <<"Reaction LIST begin" << std::endl;
	for(std::size_t i = 0; i < linkedReac.size(); i++){
		std::cout <<i<<": " << Element::reacToStr(linkedReac.at(i)) <<std::endl;
	}
	std::cout <<std::endl;


}

void MatArchNodeView::draw(std::shared_ptr<sf::RenderWindow> win){

	float delta = (2*PI_V)/Element::REACTION::__COUNT;
	float radius = g_shape.getRadius()-1;


	sf::VertexArray lines(sf::Lines);
	for(std::size_t i = 0; i < linkedViews.size(); i++){
		lines.append(sf::Vertex(g_inputs.at(int(linkedReac.at(i))).getPosition(),elemColors.at(linkedReac.at(i))));

		lines.append(sf::Vertex(linkedViews.at(i)->g_inputs.at(int(linkedReac.at(i))).getPosition(),elemColors.at(linkedReac.at(i))));

	}
	for(int i =0; i < Element::REACTION::__COUNT; i++){
		bool alreadyDrawn = false;
		for(std::size_t j = 0; j < linkedReac.size(); j++){
			if(i == linkedReac.at(j)){
				alreadyDrawn = true;
			}
		}
		if(!alreadyDrawn){
			lines.append(sf::Vertex(g_inputs.at(i).getPosition(),elemColors.at(Element::REACTION(i))));
			float	px = 50*std::cos(i*delta);
			float 	py = 50*std::sin(i*delta);
			lines.append(sf::Vertex(g_shape.getPosition() + sf::Vector2f(px,py),elemColors.at(Element::REACTION(i))));

		}
	}
	win->draw(lines);
	win->draw(g_shape);	
	for (auto input : g_inputs) {
		win->draw(input);
	}
}

void MatArchNodeView::update(){

	float delta = (2*PI_V)/Element::REACTION::__COUNT;
	g_shape.setPosition(g_shape.getPosition() + sf::Vector2f(m_speedX,m_speedY));	
	if(g_shape.getPosition().x < 0 || g_shape.getPosition().x>500){
		m_speedX *=-1;
	}
	if(g_shape.getPosition().y < 0 || g_shape.getPosition().y>500){
		m_speedY *= -1;
	}

	for(std::size_t i = 0; i < g_inputs.size(); i++){

		g_inputs.at(i).setPosition(g_shape.getPosition() + sf::Vector2f(g_shape.getRadius()*std::cos(i*delta),g_shape.getRadius()*std::sin(i*delta)));
	}
}


void MatArchNodeView::addLink(std::shared_ptr<MatArchNodeView> nodeV,Element::REACTION r){
	linkedViews.push_back(nodeV);
	linkedReac.push_back(r);

}
