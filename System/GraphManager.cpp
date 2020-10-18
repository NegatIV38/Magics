#include "GraphManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <string>
#include <vector>

GraphManager::GraphManager(){
}

GraphManager::~GraphManager(){
}
void GraphManager::update(){
	m_matPop->update();
}
void GraphManager::draw(std::shared_ptr<sf::RenderWindow> win){
	m_matPop->draw(win);
}
void GraphManager::setMaterialPop(std::shared_ptr<MaterialPop> pop){
	m_matPop = pop;
}
