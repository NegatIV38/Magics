#include "AColor.h"
#include <SFML/Graphics/Color.hpp>

AColor::AColor(){

	m_color = sf::Color::Black;
}

AColor::AColor(sf::Color c) : m_color(c){

}

AColor::~AColor(){

}

void AColor::setColor(sf::Color c){
	m_color = c;
}

sf::Color AColor::getColor(){
	return m_color;
}
