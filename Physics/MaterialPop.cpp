#include "MaterialPop.h"
#include "MatArchNode.h"
#include <SFML/System/Clock.hpp>
#include <c++/7/bits/c++config.h>
#include <memory>
#include <string>
#include <vector>
#include "../System/Console.h"

std::vector<MaterialPop*> MaterialPop::family;

int MaterialPop::orphCount = 0;
std::map<std::shared_ptr<MatArchNode>,std::shared_ptr<MatArchNodeView>> MaterialPop::deletedNodes;

MaterialPop::MaterialPop(Console* console):m_console(console){
	updateSwitch = false;
	m_clock = std::make_shared<sf::Clock>();
	if(std::find(family.begin(), family.end(),this) == family.end()){
		family.push_back(this);
	}

}
MaterialPop::~MaterialPop(){
}
void MaterialPop::add(std::string name, std::shared_ptr<MaterialArch> mat){
	std::cout << "ADD : "<<name << std::endl;
	bool alreadyExist = false;
	for (auto var : m_varMaterials) {
		if(var.second == mat){
			alreadyExist = true;
		}
	}
	if(!alreadyExist){
		m_console->addMatName(name);
		m_varMaterials.emplace(name,std::move(mat));
		m_views.emplace(name, std::make_shared<MaterialArchView>(m_varMaterials.at(name)));
		m_visibility.emplace(name,true);
	}
}
void MaterialPop::update(){

	if(m_varMaterials.size()>=1){
		if(m_clock->getElapsedTime().asSeconds() > 1){
			if(updateSwitch){
				//	relink();
				if(rand()%2){
					relink();
				}
				else{
					breakLinks();
				}
			}else{
				for (auto mat : m_varMaterials) {
					if(m_visibility.at(mat.first)){
						mat.second->updateStep();

					}
				}
			}
			updateSwitch = !updateSwitch;
			m_clock->restart();
		}
		for (auto view : m_views) {
			if(m_visibility.at(view.first)){
				//view.second->startUpdateTh();
				view.second->update();
				//	view.second->clean();
			}
		}
		cleanPop();
		deletion();
	}

}
void MaterialPop::draw(std::shared_ptr<sf::RenderWindow> win){
	for (auto view : m_views) {
		if(m_visibility.at(view.first)){
			//view.second->waitUpdateTh();
			//view.second->clean();
		}
	}

	for (auto view : m_views) {
		if(m_visibility.at(view.first)){
			view.second->clean();
			view.second->draw(win);
		}
	}
}
std::shared_ptr<MaterialArch> MaterialPop::get(std::string name){
	return m_varMaterials.at(name);
}
bool MaterialPop::find(std::string name){
	return m_varMaterials.find(name) != m_varMaterials.end();
}
std::string MaterialPop::orphName(){
	return "ORPH"+std::to_string(orphCount++);
}
std::map<REAC, std::vector<std::shared_ptr<MatArchNode>>> MaterialPop::getFreeLinks(){
	std::map<REAC, std::vector<std::shared_ptr<MatArchNode>>> freeLinks;
	for(int i = 0; i < REAC::__COUNT; i++){
		freeLinks.emplace(REAC(i),std::vector<std::shared_ptr<MatArchNode>>());
	}
	for (auto matarch : m_varMaterials) {
		if(m_visibility.at(matarch.first)){
			auto matFlinks =  matarch.second->getFreeLinks();
			for(int i = 0; i < REAC::__COUNT; i++){
				if(matFlinks.find(REAC(i))!= matFlinks.end()){
					std::vector<std::shared_ptr<MatArchNode>>* reacFreeList = &freeLinks.at(REAC(i));
					for(std::size_t j = 0; j < matFlinks.at(REAC(i)).size(); j++){
						if(std::find(reacFreeList->begin(),reacFreeList->end(),matFlinks.at(REAC(i)).at(j))== reacFreeList->end()){
							reacFreeList->push_back(matFlinks.at(REAC(i)).at(j));
						}
					}
				}
			}
		}
	}
	return freeLinks;
}
std::map<std::string,std::shared_ptr<MatArchNode>> MaterialPop::getAllRoots(){
	std::map<std::string,std::shared_ptr<MatArchNode>> ret;
	for (auto arch : m_varMaterials) {
		if(arch.second != nullptr){
			ret.emplace(arch.first,arch.second->getRoot());
		}
	}
	return ret;
}
std::map<REAC,std::vector<std::shared_ptr<MatArchNode>>> MaterialPop::getAllLinks(){
	std::map<REAC,std::vector<std::shared_ptr<MatArchNode>>> links;
	for(int i = 0; i < REAC::__COUNT; i++){
		links.emplace(REAC(i),std::vector<std::shared_ptr<MatArchNode>>());
	}
	for (auto arch : m_varMaterials) {
		for(int i = 0; i < REAC::__COUNT; i++){
			if(arch.second != nullptr){
				auto archLinks = arch.second->getLinkedNodes().at(REAC(i));
				links.at(REAC(i)).insert(links.at(REAC(i)).end(),archLinks.begin(),archLinks.end());
			}
		}
	}
	return links;
}
void MaterialPop::breakLinks(){
	auto fLinks = getFreeLinks();
	auto links = getAllLinks();
	for(int i = 0; i < REAC::__COUNT; i++){
		for(std::size_t j = 0; j < fLinks.at(REAC(i)).size(); j++){
			std::shared_ptr<MatArchNode> toLink = fLinks.at(REAC(i)).at(j);
			if(links.at(REAC(i)).size() > 0){
				std::size_t best = 0;
				for(std::size_t k = 0; k < links.at(REAC(i)).size(); k++){
					bool canLink =  std::find(fLinks.at(REAC(i)).begin(),fLinks.at(REAC(i)).end(),links.at(REAC(i)).at(k)->getNode(REAC(i)))== fLinks.at(REAC(i)).end();
					if(links.at(REAC(i)).at(k)->getNode(REAC(i)) != nullptr&&links.at(REAC(i)).at(best)->getNode(REAC(i)) == nullptr  ){
						best = k;
					}
					else if(links.at(REAC(i)).at(k)->getNode(REAC(i)) != nullptr){
						float valLink = links.at(REAC(i)).at(k)->getNode(REAC(i))->getElementRawValue(REAC(i));
						float bestLink = links.at(REAC(i)).at(best)->getNode(REAC(i))->getElementRawValue(REAC(i));
						bool compatible = ((toLink->getElementRawValue(REAC(i)) > 1 &&
									links.at(REAC(i)).at(k)->getElementRawValue(REAC(i)) < -1) ||
								(toLink->getElementRawValue(REAC(i)) < -1 &&
								 links.at(REAC(i)).at(k)->getElementRawValue(REAC(i)) > 1));

						bool canBreak = (toLink->getElementRawValue(REAC(i))-valLink > toLink->getElementRawValue(REAC(i))-bestLink);
						if(compatible && canBreak){
							best = k;
						}
					}
				}
				auto other = links.at(REAC(i)).at(best)->getNode(REAC(i));
				if(other != nullptr && other != toLink){
					other->unlink(REAC(i));
					links.at(REAC(i)).at(best)->unlink(REAC(i));
					bool noRoot = true;
					auto roots = getAllRoots();
					for (auto root:roots){
						if(!other->orpheline(root.second)){
							noRoot = false;
							break;
						}
					}
					if(other->orpheline()&& noRoot){//other is orph
						std::cout << "ORPH" << std::endl;
						//std::shared_ptr<MaterialPop> thisPtr = this->shared_from_this();
						std::string oname = orphName();
						m_orphBuffer.emplace(oname,std::make_shared<MaterialArch>(this));
						m_orphBuffer.at(oname)->setRoot(other);
						add(oname,m_orphBuffer.at(oname));
					}
					toLink->link(REAC(i),links.at(REAC(i)).at(best));
				}
				else if(other->orpheline()){//other is root
					std::cout << "ORPH GROUP" << std::endl;
					std::string oname = orphName();
					m_orphBuffer.emplace(oname,std::make_shared<MaterialArch>(this));
					m_orphBuffer.at(oname)->setRoot(links.at(REAC(i)).at(best));
					add(oname,m_orphBuffer.at(oname));

				}
			}
		}
	}
}
void MaterialPop::relink(){
	auto fLinks = getFreeLinks();
	for(int i = 0; i < REAC::__COUNT; i++){
		for(std::size_t j = 0; j < fLinks.at(REAC(i)).size(); j++){
			if(fLinks.at(REAC(i)).at(j)->getNode(REAC(i)) == nullptr){
				std::shared_ptr<MatArchNode> toLink = fLinks.at(REAC(i)).at(j);
				std::size_t best = 0;
				for(std::size_t k = 0; k < fLinks.at(REAC(i)).size(); k++){
					if(k != j && fLinks.at(REAC(i)).at(k)->getNode(REAC(i)) == nullptr){
						float valK =(toLink->getElementRawValue(REAC(i))+fLinks.at(REAC(i)).at(k)->getElementRawValue(REAC(i)));
						float valBest =(toLink->getElementRawValue(REAC(i))+fLinks.at(REAC(i)).at(best)->getElementRawValue(REAC(i)));
						bool compatible = ((toLink->getElementRawValue(REAC(i)) > 1 &&
									fLinks.at(REAC(i)).at(k)->getElementRawValue(REAC(i)) < -1) ||
								(toLink->getElementRawValue(REAC(i)) < -1 &&
								 fLinks.at(REAC(i)).at(k)->getElementRawValue(REAC(i)) > 1));

						if(best == j){
							best = k;
						}
						else if(compatible && std::abs(valK)< std::abs(valBest)){
							best = k;
						}
					}
				}
				bool compatBest = ((toLink->getElementRawValue(REAC(i)) > 1 &&
							fLinks.at(REAC(i)).at(best)->getElementRawValue(REAC(i)) < -1) ||
						(toLink->getElementRawValue(REAC(i)) < -1 &&
						 fLinks.at(REAC(i)).at(best)->getElementRawValue(REAC(i)) > 1));

				if((fLinks.at(REAC(i)).at(best)->getNode(REAC(i))==nullptr)&& best != j && compatBest){
					toLink->link(REAC(i),fLinks.at(REAC(i)).at(best));
					//	std::cout << "RELINK" << std::endl;
				}
			}
		}
	}
}

void MaterialPop::cleanPop(){
	m_orphBuffer.clear();
	auto roots = getAllRoots();
	//	std::vector<std::string> toDelete;
	toDelete.clear();
	for (auto arch1 : roots) {
		if(std::find(toDelete.begin(),toDelete.end(),arch1.first) == toDelete.end()){

			for (auto arch2 : roots) {
				if(arch2.second != arch1.second ){
					if((arch1.second == nullptr || m_varMaterials.at(arch1.first)->getCount()== 0) && (std::find(toDelete.begin(),toDelete.end(),arch1.first)== toDelete.end())){

						std::cout << "NULL_DELETE" << std::endl;
						toDelete.push_back(arch1.first);
					}
					else if((arch2.second == nullptr|| m_varMaterials.at(arch1.first)->getCount() == 0)&& (std::find(toDelete.begin(),toDelete.end(),arch2.first)== toDelete.end())){
						std::cout << "NULL_DELETE" << std::endl;
						toDelete.push_back(arch2.first);
					}
					else if(!arch1.second->orpheline(arch2.second)){
						if(arch2.second->orpheline(arch1.second)){
							std::cout << "BIZARRE : "<< arch2.first << " x " << arch1.first << std::endl;
						}
						if(std::find(toDelete.begin(),toDelete.end(),arch2.first)== toDelete.end()){
							toDelete.push_back(arch2.first);
						}
					}
				}
			}
		}
		//std::reverse(toDelete.begin(),toDelete.end());
	}

	getDeletedViews(toDelete);

}
void MaterialPop::deletion(){
	for (auto name : toDelete) {
		m_varMaterials.erase(name);
		//m_views.erase(name);
		//m_visibility.erase(name);
		hide(name);
		m_console->deleteMatName(name);
	}
}
void MaterialPop::getDeletedViews(std::vector<std::string> toDelete){
	deletedNodes.clear();
	for (auto name : toDelete) {
		auto listNodes =m_views.at(name)->getNodes();
		deletedNodes.insert(listNodes.begin(),listNodes.end());
	}

}
void MaterialPop::show(std::string name){
	m_visibility.at(name) = true;
}
void MaterialPop::hide(std::string name){
	m_visibility.at(name) = false;
}
void MaterialPop::hideall(){
	for (auto v : m_visibility) {
		v.second = false;
	}
}

std::pair<int,int> MaterialPop::getCount(std::string name){
	std::pair<int,int> ret;
	ret.first = m_varMaterials.at(name)->getCount();
	ret.second = m_views.at(name)->getNodes().size();
	return ret;
}

