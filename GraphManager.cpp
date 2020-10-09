#include "GraphManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>
#include <string>

double GraphManager::dt = 0;
sf::Clock GraphManager::clock;

GraphManager::GraphManager(){

}

GraphManager::~GraphManager(){

}

void GraphManager::addMaterial(std::string name,std::shared_ptr<MaterialArch> arch){
	m_matViews.emplace(name,std::make_shared<MaterialArchView>(arch));	
	m_visibility.emplace(name, false);
}

void GraphManager::update(){
	for (auto mat : m_matViews) {
		if(m_visibility.at(mat.first)){
			mat.second->update(GraphManager::dt);
		}
	}

}

void GraphManager::draw(std::shared_ptr<sf::RenderWindow> win){
	for (auto mat: m_matViews) {
		if(m_visibility.at(mat.first)){
			mat.second->draw(win);
		
		}
	}

}

void GraphManager::hide(std::string name){
	m_visibility.at(name) = false;
}


void GraphManager::show(std::string name){
	m_visibility.at(name) = true;
}
