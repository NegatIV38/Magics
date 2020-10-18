#include "GraphManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <string>
#include <vector>

double GraphManager::dt = 0;
sf::Clock GraphManager::clock;

GraphManager::GraphManager(){
//	m_visibilityCounter = 0;
}

GraphManager::~GraphManager(){

}

/*void GraphManager::addMaterial(std::string name,std::shared_ptr<MaterialArch> arch){
	m_matViews.emplace(name,std::make_shared<MaterialArchView>(arch));	
	m_visibility.emplace(name, false);


}*/

void GraphManager::update(){
	m_matPop->update();
/*	for (auto mat : m_matViews) {
		if(m_visibility.at(mat.first)){
			//ths.push_back(sf::Thread(&MaterialArchView::update,mat.second.get()));
			//ths.back().launch();
			//mat.second->update();
			mat.second->startUpdateTh();
		}
	}
*/
}

void GraphManager::draw(std::shared_ptr<sf::RenderWindow> win){
	m_matPop->draw(win);
	/*for (auto mat: m_matViews) {
		if(m_visibility.at(mat.first)){
			mat.second->waitUpdateTh(); //!
		
		}
	}
	for (auto mat: m_matViews) {
		if(m_visibility.at(mat.first)){
			mat.second->draw(win);
		
		}
	}*/

}

void GraphManager::hide(std::string name){
	/*bool b = m_visibility.at(name);
	m_visibility.at(name) = false;
	if(b != m_visibility.at(name)){
		m_visibilityCounter--;
	}*/
}

void GraphManager::hideAll(){
	/*m_visibilityCounter = 0;
	for (auto& i : m_visibility) {
		i.second = false;
	}*/
}

void GraphManager::show(std::string name){

	//m_matViews.at(name)->setPosition(sf::Vector2f(m_visibilityCounter*600+300,300));
	/*bool b = m_visibility.at(name);
	m_visibility.at(name) = true;
	if(b != m_visibility.at(name)){
		m_visibilityCounter++;
	}*/	
}

void GraphManager::setMaterialPop(std::shared_ptr<MaterialPop> pop){
	m_matPop = pop;
}
