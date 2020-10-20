#include "MaterialPop.h"
#include <memory>
#include <string>
#include "../System/Console.h"

int MaterialPop::orphCount = 0;

MaterialPop::MaterialPop(Console* console):m_console(console){
}
MaterialPop::~MaterialPop(){
}
void MaterialPop::add(std::string name, std::shared_ptr<MaterialArch> mat){
	bool alreadyExist = false;
	for (auto var : m_varMaterials) {
		if(var.second == mat){
			alreadyExist = true;
		}
	}
	if(!alreadyExist){
		m_console->addMatName(name);
		m_varMaterials.emplace(name,mat);
		m_views.emplace(name, std::make_shared<MaterialArchView>(mat));
	}
}
void MaterialPop::update(){
	for (auto mat : m_varMaterials) {
		mat.second->update();
	}
	for (auto view : m_views) {
		view.second->startUpdateTh();
		//view.second->update();
	}
}
void MaterialPop::draw(std::shared_ptr<sf::RenderWindow> win){
	for (auto view : m_views) {
		view.second->waitUpdateTh();	
	}
	for (auto view : m_views) {
		view.second->draw(win);
	}
}
std::shared_ptr<MaterialArch> MaterialPop::get(std::string name){
	return m_varMaterials.at(name);
}
bool MaterialPop::find(std::string name){
	return m_varMaterials.find(name) != m_varMaterials.end();
}
std::string MaterialPop::orphName(){
	return "@"+std::to_string(orphCount++);
}
