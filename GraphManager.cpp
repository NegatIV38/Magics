#include "GraphManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

GraphManager::GraphManager(){

}

GraphManager::~GraphManager(){

}

void GraphManager::addMaterial(std::shared_ptr<MaterialArch> arch){
	m_matViews.push_back(std::make_shared<MaterialArchView>(arch));	
}

void GraphManager::update(){
	for (auto mat : m_matViews) {
		mat->update();
	}

}

void GraphManager::draw(std::shared_ptr<sf::RenderWindow> win){
	for (auto mat: m_matViews) {
		mat->draw(win);
		
	}
}
