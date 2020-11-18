#include "MaterialArchView.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iterator>
#include <memory>
#include <vector>
#include "MaterialArch.h"
#include "MaterialPop.h"

int NodeCounter::c = 0;

void NodeCounter::reset(){
	NodeCounter::c =0;
}
MaterialArchView::MaterialArchView(std::shared_ptr<MaterialArch> arch):m_arch(arch),m_updateTh(&MaterialArchView::update,this){
	threadStatus = false;
	initNodes();
}
MaterialArchView::~MaterialArchView(){
}
void MaterialArchView::nodeBuild(std::shared_ptr<MatArchNode> cnode, std::map<std::shared_ptr<MatArchNode>,std::shared_ptr<MatArchNodeView>>& nodes,std::vector<std::shared_ptr<MatArchNode>>& nodeList){
	if(cnode == nullptr){
		std::cout << "nodeNull" << std::endl;
	}
	nodeList.push_back(cnode);
	if(nodes.find(cnode) == nodes.end()){
	/*	if(MaterialPop::deletedNodes.find(cnode) != MaterialPop::deletedNodes.end()){
			std::cout << "_RECUP_" << std::endl;
			nodes.emplace(cnode,MaterialPop::deletedNodes.at(cnode));
		}else{*/

			nodes.emplace(cnode,std::make_shared<MatArchNodeView>(cnode));
	//	}
	}

}
void MaterialArchView::linkBuild(std::shared_ptr<MatArchNode> cnode,  std::map<std::shared_ptr<MatArchNode>,std::shared_ptr<MatArchNodeView>>& nodes){
	auto nxtNodes = cnode->getNextNodes();
	for(int i =0; i < REAC::__COUNT; i++){
		for (auto node : nodes) {
			if(node.first == nxtNodes.at(REAC(i)) && node.first != cnode){
				nodes.find(cnode)->second->addLink(node.second,REAC(i));
			}
		}
	}
}
void MaterialArchView::draw(std::shared_ptr<sf::RenderWindow> win){
	for (auto node: m_nodes) {
		node.second->draw(win);
	}
}
void MaterialArchView::update(){
	//if(m_arch->getTicked()){
	for (auto node : m_nodes) {
		node.second->clearLinks();
	}
	initNodes();
	m_arch->closeTicked();
	//}
	//clean();
	for (auto node : m_nodes) {
		node.second->update();
	}
	threadStatus = false;
}
void print(std::shared_ptr<MatArchNode> cnode){
	static int c = 0;
	std::cout << c<<" node\t " << cnode <<"\n" ;
	c++;
}
void MaterialArchView::initNodes(){
	std::vector<std::shared_ptr<MatArchNode>> nodesList;
	std::vector<std::shared_ptr<MatArchNode>> toErase;
	auto prevNodes = m_nodes;
	m_arch->routeAllNodes(nodeBuild,m_nodes,nodesList);
	for (auto node : m_nodes) {
		//if(prevNodes.find(node.first) != prevNodes.end()){

		//}
		//if(node.second == nullptr){
		//std::cout << "NULL" << std::endl;
		//}

		if(std::find(nodesList.begin(),nodesList.end(),node.first) == nodesList.end()){
			//	toErase.push_back(node.first);
			delNodes.push_back(node.first);
			//	std::cout << "ERASING" << std::endl;
		}

	}


	m_arch->routeAllNodes(linkBuild,m_nodes);
}
void MaterialArchView::clean(){
	for(std::size_t i = 0; i < delNodes.size(); i++){

		m_nodes.erase(delNodes.at(i));
	}

}
void MaterialArchView::printLinks(){
	for (auto node : m_nodes) {
		node.second->printLinks();
	}
}
void MaterialArchView::setPosition(sf::Vector2f pos, sf::Vector2f dim){
	for (auto node : m_nodes) {
		node.second->setPosition(pos,dim);
	}
}
void MaterialArchView::startUpdateTh(){
	threadStatus = true;
	m_updateTh.launch();
	//m_updateTh.wait();
}
void MaterialArchView::waitUpdateTh(){
	//if(!threadStatus){
	m_updateTh.wait();
	//}
}
std::map<std::shared_ptr<MatArchNode>,std::shared_ptr<MatArchNodeView>> MaterialArchView::getNodes(){
	return m_nodes;
}
