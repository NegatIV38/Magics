#include "MaterialArchView.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

int NodeCounter::c = 0;

void NodeCounter::reset(){
	NodeCounter::c =0;
}

MaterialArchView::MaterialArchView(std::shared_ptr<MaterialArch> arch):m_arch(arch){
	initNodes();
}

MaterialArchView::~MaterialArchView(){


}

void MaterialArchView::draw(std::shared_ptr<sf::RenderWindow> win){
	for (auto node: g_nodes) {
		node->draw(win);
	}
}

void MaterialArchView::update(double dt){
	for (auto node : g_nodes) {
		node->update(dt);
	}
}

void print(std::shared_ptr<MatArchNode> cnode){
	static int c = 0;
	std::cout << c<<" node\t " << cnode <<"\n" ; 
	c++;
}
void MaterialArchView::initNodes(){

//	m_arch->routeAllNodes(print);
	m_arch->routeAllNodes<std::vector<std::shared_ptr<MatArchNodeView>>,std::vector<std::shared_ptr<MatArchNode>>>(nodeBuild,g_nodes,m_nodes);
	m_arch->routeAllNodes<std::vector<std::shared_ptr<MatArchNodeView>>,std::vector<std::shared_ptr<MatArchNode>>>(linkBuild,g_nodes,m_nodes);
	nc.reset();	
	printLinks();
}

void MaterialArchView::printLinks(){
	for (auto node : g_nodes) {
		node->printLinks();
	}
}

void MaterialArchView::linkBuild(std::shared_ptr<MatArchNode> cnode,  std::vector<std::shared_ptr<MatArchNodeView>>& nodesCollec, std::vector<std::shared_ptr<MatArchNode>>& nodeList){
	auto nxtNodes = cnode->getNextNodes();
	for(int i =0; i < Element::REACTION::__COUNT; i++){
		for(std::size_t j = 0; j < nodeList.size(); j++){
			if(nxtNodes.at(Element::REACTION(i))== nodeList.at(j)){
				nodesCollec.at(nc.c)->setID(nc.c);
				nodesCollec.at(nc.c)->addLink(nodesCollec.at(j),Element::REACTION(i));
			}		
		}
	}
	nc.c++;
}

void MaterialArchView::nodeBuild(std::shared_ptr<MatArchNode> cnode, std::vector<std::shared_ptr<MatArchNodeView>>& nodesCollec,std::vector<std::shared_ptr<MatArchNode>>& nodesList){
	
	nodesCollec.push_back(std::make_shared<MatArchNodeView>(cnode));
	nodesList.push_back(cnode);
}

