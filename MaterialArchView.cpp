#include "MaterialArchView.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

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

void MaterialArchView::update(){

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

}


void MaterialArchView::drawLinks(std::shared_ptr<sf::RenderWindow> win){
	std::vector<std::vector<int>> links;

}

void MaterialArchView::getLinks(std::shared_ptr<MatArchNode> cnode, std::vector<std::vector<int>>& linksCollec){
		
}

void MaterialArchView::linkBuild(std::shared_ptr<MatArchNode> cnode,  std::vector<std::shared_ptr<MatArchNodeView>>& nodesCollec, std::vector<std::shared_ptr<MatArchNode>>& nodeList){
	static int c = 0;
	auto nxtNodes = cnode->getNextNodes();
	for(int i =0; i < Element::REACTION::__COUNT; i++){
		for(std::size_t j = 0; j < nodeList.size(); j++){
			if(nxtNodes.at(Element::REACTION(i))== nodeList.at(j)){
				nodesCollec.at(c)->addLink(nodesCollec.at(j),Element::REACTION(i));
			}		
		}
	}
	c++;
}

void MaterialArchView::nodeBuild(std::shared_ptr<MatArchNode> cnode, std::vector<std::shared_ptr<MatArchNodeView>>& nodesCollec,std::vector<std::shared_ptr<MatArchNode>>& nodesList){
	
	nodesCollec.push_back(std::make_shared<MatArchNodeView>());
	nodesList.push_back(cnode);
}

