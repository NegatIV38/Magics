#include "MaterialArch.h"	
#include <algorithm>
#include <deque>
#include <memory>
#include <vector>

MaterialArch::MaterialArch(){
}
MaterialArch::~MaterialArch(){
}
void MaterialArch::setRoot(std::shared_ptr<MatArchNode> r){
	//if r is linked to root ->
	m_root = r;
}
std::shared_ptr<MatArchNode> MaterialArch::getRoot(){
	return m_root;
}
void MaterialArch::routeAllNodes(void(*fun)(std::shared_ptr<MatArchNode> currNode)){
	std::vector<std::shared_ptr<MatArchNode>> visited;
	std::deque<std::shared_ptr<MatArchNode>> toVisit;
	if(m_root != nullptr){
		toVisit.push_back(m_root);
	}
	while(toVisit.size() > 0){
		auto current = toVisit.front();
		toVisit.pop_front();
		fun(current);
		visited.push_back(current);
		for(int i = 0; i < Element::REACTION::__COUNT; i++){
			if(current->getNode(Element::REACTION(i))!= nullptr){
				bool hasBeenVisited = false;
				for (int j = 0; j < visited.size() ; j++) {
					if(current->getNode(Element::REACTION(i))== visited.at(j)){
						hasBeenVisited = true;
						break;
					}
				}
				if(!hasBeenVisited){
					bool isToVisit = false;
					for (int j = 0; j < toVisit.size() ; j++) {
						if(current->getNode(Element::REACTION(i))== toVisit.at(j)){
							isToVisit = true;
							break;
						}
					}
					if(!isToVisit){
						toVisit.push_back(current->getNode(Element::REACTION(i)));
					}	
				}
			}
		}
	}
}
std::map<Element::REACTION, float> MaterialArch::getResultReac(){
	std::map<Element::REACTION,float> ret;
	for(int i = 0; i < Element::REACTION::__COUNT; i++){
		ret.emplace(Element::REACTION(i),0.f);
	}
	routeAllNodes<std::map<Element::REACTION,float>>(sumStatsNodes,ret);
	for(int i = 0; i < Element::REACTION::__COUNT; i++){
		ret.at(Element::REACTION(i)) /=2.f;
	}
	return ret;
}
std::map<Element::REACTION,float> MaterialArch::getReacOfSign(bool positive){
		std::map<Element::REACTION,float> ret;
	for(int i = 0; i < Element::REACTION::__COUNT; i++){
		ret.emplace(Element::REACTION(i),0.f);
	}
	routeAllNodes<std::map<Element::REACTION,float>,bool>(sumStatsSign,ret,positive);

	for(int i = 0; i < Element::REACTION::__COUNT; i++){
		ret.at(Element::REACTION(i)) /=2.f;
	}
	return ret;
}
void MaterialArch::generate(int nbElem){
	std::vector<std::shared_ptr<MatArchNode>> nodeList;
	if(m_root == nullptr){
		m_root = std::make_shared<MatArchNode>();
		m_root->generate();
		nodeList.push_back(m_root);
		for (int i = 1; i < nbElem; ++i) {
			std::shared_ptr<MatArchNode> nextNode = std::make_shared<MatArchNode>();
			nextNode->generate();
			nodeList.push_back(nextNode);
		}
		if(nodeList.size() < 2){
			return;
		}
		for (auto node : nodeList) {
			bool canLink = false;
			auto linkList = node->getNextNodes();
			for(int i = 0; i < Element::REACTION::__COUNT; i++){
				if(linkList.at(Element::REACTION(i)) == nullptr){
					canLink = true;
					break;
				}
			}
			if(canLink){
				for(std::size_t j = 0; j < Element::REACTION::__COUNT; j++){
					if(linkList.at(Element::REACTION(j)) == nullptr){
						std::vector<int> visited;
						int idLinkNode;
						float rawValue = node->getElementRawValue(Element::REACTION(j));
						bool sameSign;
						do{
							idLinkNode = rand()%nodeList.size();
							float candidatRawVal = nodeList.at(idLinkNode)->getElementRawValue(Element::REACTION(j));
							sameSign = (rawValue > 0 && candidatRawVal > 0) || (rawValue < 0 && candidatRawVal < 0);
							if(std::find(visited.begin(),visited.end(),idLinkNode) == visited.end()){
								visited.push_back(idLinkNode);
							}
						}while((nodeList.at(idLinkNode) == node || nodeList.at(idLinkNode)->getNode(Element::REACTION(j))!=nullptr || sameSign)&&visited.size() < nodeList.size());
						if(rand()%nbElem>-1 && (nodeList.at(idLinkNode) != node && nodeList.at(idLinkNode)->getNode(Element::REACTION(j))==nullptr)){
							node->link(Element::REACTION(j),nodeList.at(idLinkNode));	
						}	
					}
				}
			}			
		}
	}
}
void MaterialArch::sumStatsNodes(std::shared_ptr<MatArchNode> cnode, std::map<Element::REACTION,float>& sum){
	auto stats = cnode->getResultReac();
	for(std::size_t j = 0; j < Element::REACTION::__COUNT; j++){
		if(cnode->getNextNodes().at(Element::REACTION(j))!= nullptr){
			sum.at(Element::REACTION(j)) += stats.at(Element::REACTION(j));
		}
	}	
}
void MaterialArch::sumStatsSign(std::shared_ptr<MatArchNode> cnode, std::map<Element::REACTION,float>& sum,bool& positive){
	auto stats = cnode->getResultReac();
	for(std::size_t j = 0; j < Element::REACTION::__COUNT; j++){
		if(cnode->getNextNodes().at(Element::REACTION(j))!= nullptr){
			if((positive && stats.at(Element::REACTION(j)) > 0)||(!positive && stats.at(Element::REACTION(j))<0)){
				sum.at(Element::REACTION(j)) += stats.at(Element::REACTION(j));
			}
		}
	}	
}
void MaterialArch::setFreeLinks(std::shared_ptr<MatArchNode> cnode, std::map<Element::REACTION,std::vector<std::shared_ptr<MatArchNode>>>& ret){
	auto nxt = cnode->getNextNodes();
	for(int i = 0; i < Element::REACTION::__COUNT; i++){
		if(nxt.at(Element::REACTION(i))==nullptr){
			ret.at(Element::REACTION(i)).push_back(cnode);
		}
	}
}
std::map<Element::REACTION, std::vector<std::shared_ptr<MatArchNode>>> MaterialArch::getFreeLinks(){
	std::map<Element::REACTION,std::vector<std::shared_ptr<MatArchNode>>> ret;
	for(int i = 0; i < Element::REACTION::__COUNT; i++){
		ret.emplace(Element::REACTION(i),std::vector<std::shared_ptr<MatArchNode>>());
	}
	routeAllNodes<std::map<Element::REACTION,std::vector<std::shared_ptr<MatArchNode>>>>(setFreeLinks,ret);
	return ret;
}
MaterialArch MaterialArch::operator+(const MaterialArch& other){
	MaterialArch ret = *this;
	MaterialArch o = other;
	auto retFree = ret.getFreeLinks();
	auto oFree = o.getFreeLinks();
	for(int i = 0; i < Element::REACTION::__COUNT; i++){
		for(int j = 0; j < retFree.at(Element::REACTION(i)).size(); j++){
			float val = retFree.at(Element::REACTION(i)).at(j)->getElementRawValue(Element::REACTION(i));
			int best = 0;
			float cval;	
			for(int k = 1; k < oFree.at(Element::REACTION(i)).size(); k++){	
				if(oFree.at(Element::REACTION(i)).at(k)->getNode(Element::REACTION(i))==nullptr){
					bool sameSign;
					float oval = 	oFree.at(Element::REACTION(i)).at(k)->getElementRawValue(Element::REACTION(i));
					float bval = 	oFree.at(Element::REACTION(i)).at(best)->getElementRawValue(Element::REACTION(i));
					sameSign = (val > 0 && oval > 0) || (val < 0 && oval < 0);
					if(std::abs(val+oval) < std::abs(val+bval) && !sameSign){
						best = k;
					}
				}
			}
			if(oFree.at(Element::REACTION(i)).size() > 0 && retFree.at(Element::REACTION(i)).at(j)->getNode(Element::REACTION(i)) == nullptr && oFree.at(Element::REACTION(i)).at(best)->getNode(Element::REACTION(i))==nullptr){
				cval = oFree.at(Element::REACTION(i)).at(best)->getElementRawValue(Element::REACTION(i));
				if( !((val > 0 && cval > 0) || (val < 0 && cval < 0))){
					retFree.at(Element::REACTION(i)).at(j)->link(Element::REACTION(i),oFree.at(Element::REACTION(i)).at(best));
				}
			}
		}
	}
	return ret;
}
std::shared_ptr<MaterialArch> MaterialArch::combine(std::shared_ptr<MaterialArch> other){
	MaterialArch sum = *this+*other;
	return std::make_shared<MaterialArch>(std::move(sum));
}
void MaterialArch::update(){
	if(m_clock.getElapsedTime().asSeconds() >= 1){
		m_clock.restart();
		updateStep();
	}
}
void MaterialArch::updateStep(){
	routeAllNodes(stepNodes);
} 
void MaterialArch::stepNodes(std::shared_ptr<MatArchNode> cnode){
	cnode->update();
}
