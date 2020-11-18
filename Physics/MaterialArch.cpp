#include "MaterialArch.h"
#include <algorithm>
#include <deque>
#include <memory>
#include <vector>
#include "MaterialPop.h"

MaterialArch::MaterialArch(MaterialPop* pop){
	m_pop.reset(pop);
	ticked = false;
}
MaterialArch::~MaterialArch(){
}
void MaterialArch::setRoot(std::shared_ptr<MatArchNode> r){
		m_root = r;
}
std::shared_ptr<MatArchNode> MaterialArch::getRoot(){
	return m_root;
}
std::vector<std::shared_ptr<MatArchNode>> MaterialArch::getAllNodes(std::shared_ptr<MatArchNode> root){
	std::vector<std::shared_ptr<MatArchNode>> visited;
	std::deque<std::shared_ptr<MatArchNode>> toVisit;
	if(root != nullptr){
		toVisit.push_back(root);
	}
	while(toVisit.size() > 0){
		auto current = toVisit.front();
		toVisit.pop_front();

		visited.push_back(current);
		for(int i = 0; i < REAC::__COUNT; i++){
			if(current->getNode(REAC(i))!= nullptr){
				bool hasBeenVisited = false;
				for (int j = 0; j < visited.size() ; j++) {
					if(current->getNode(REAC(i))== visited.at(j)){
						hasBeenVisited = true;
						break;
					}
				}
				if(!hasBeenVisited){
					bool isToVisit = false;
					for (int j = 0; j < toVisit.size() ; j++) {
						if(current->getNode(REAC(i))== toVisit.at(j)){
							isToVisit = true;
							break;
						}
					}
					if(!isToVisit){
						toVisit.push_back(current->getNode(REAC(i)));
					}
				}
			}
		}
	}
	return visited;
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
		for(int i = 0; i < REAC::__COUNT; i++){
			if(current->getNode(REAC(i))!= nullptr){
				bool hasBeenVisited = false;
				for (int j = 0; j < visited.size() ; j++) {
					if(current->getNode(REAC(i))== visited.at(j)){
						hasBeenVisited = true;
						break;
					}
				}
				if(!hasBeenVisited){
					bool isToVisit = false;
					for (int j = 0; j < toVisit.size() ; j++) {
						if(current->getNode(REAC(i))== toVisit.at(j)){
							isToVisit = true;
							break;
						}
					}
					if(!isToVisit){
						toVisit.push_back(current->getNode(REAC(i)));
					}
				}
			}
		}
	}
}
std::map<REAC, float> MaterialArch::getResultReac(){
	std::map<REAC,float> ret;
	for(int i = 0; i < REAC::__COUNT; i++){
		ret.emplace(REAC(i),0.f);
	}
	routeAllNodes<std::map<REAC,float>>(sumStatsNodes,ret);
	for(int i = 0; i < REAC::__COUNT; i++){
		ret.at(REAC(i)) /=2.f;
	}
	return ret;
}
std::map<REAC,float> MaterialArch::getReacOfSign(bool positive){
	std::map<REAC,float> ret;
	for(int i = 0; i < REAC::__COUNT; i++){
		ret.emplace(REAC(i),0.f);
	}
	routeAllNodes<std::map<REAC,float>,bool>(sumStatsSign,ret,positive);

	for(int i = 0; i < REAC::__COUNT; i++){
		ret.at(REAC(i)) /=2.f;
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
			for(int i = 0; i < REAC::__COUNT; i++){
/*				bool compatible = ((node->getElementRawValue(REAC(i)) > 1 &&
							fLinks.at(REAC(i)).at(best)->getElementRawValue(REAC(i)) < -1) ||
						(node->getElementRawValue(REAC(i)) < -1 &&
						 fLinks.at(REAC(i)).at(best)->getElementRawValue(REAC(i)) > 1));
*/
				if(linkList.at(REAC(i)) == nullptr){
					canLink = true;
					break;
				}
			}
			if(canLink){
				for(std::size_t j = 0; j < REAC::__COUNT; j++){
					if(linkList.at(REAC(j)) == nullptr){
						std::vector<int> visited;
						int idLinkNode;
						float rawValue = node->getElementRawValue(REAC(j));
						bool sameSign;
						do{
							idLinkNode = rand()%nodeList.size();
							float candidatRawVal = nodeList.at(idLinkNode)->getElementRawValue(REAC(j));
							sameSign = (rawValue > 0 && candidatRawVal > 0) || (rawValue < 0 && candidatRawVal < 0);
							if(std::find(visited.begin(),visited.end(),idLinkNode) == visited.end()){
								visited.push_back(idLinkNode);
							}
						}while((nodeList.at(idLinkNode) == node || nodeList.at(idLinkNode)->getNode(REAC(j))!=nullptr || sameSign)&&visited.size() < nodeList.size());
						if(rand()%nbElem>-1 && (nodeList.at(idLinkNode) != node && nodeList.at(idLinkNode)->getNode(REAC(j))==nullptr)){
							node->link(REAC(j),nodeList.at(idLinkNode));
						}
					}
				}
			}
		}
	}
	std::vector<std::shared_ptr<MatArchNode>> newRoots;
	for (auto node : nodeList) {
		bool unreachable = true;
		if(!node->orpheline(m_root)){
				unreachable = false;
		}
		for (auto root : newRoots) {
			if(!node->orpheline(root)){
				unreachable = false;
			}
		}
		if(unreachable){
			std::cout << "@ORPH" << std::endl;
			std::shared_ptr<MaterialArch> orph(new MaterialArch(m_pop.get()));
			orph->setRoot(node);
			m_pop->add(m_pop->orphName(),orph);
			newRoots.push_back(node);
		}
	}
}
void MaterialArch::sumStatsNodes(std::shared_ptr<MatArchNode> cnode, std::map<REAC,float>& sum){
	auto stats = cnode->getResultReac();
	for(std::size_t j = 0; j < REAC::__COUNT; j++){
		if(cnode->getNextNodes().at(REAC(j))!= nullptr){
			sum.at(REAC(j)) += stats.at(REAC(j));
		}
	}
}
void MaterialArch::sumStatsSign(std::shared_ptr<MatArchNode> cnode, std::map<REAC,float>& sum,bool& positive){
	auto stats = cnode->getResultReac();
	for(std::size_t j = 0; j < REAC::__COUNT; j++){
		if(cnode->getNextNodes().at(REAC(j))!= nullptr){
			if((positive && stats.at(REAC(j)) > 0)||(!positive && stats.at(REAC(j))<0)){
				sum.at(REAC(j)) += stats.at(REAC(j));
			}
		}
	}
}
////
void MaterialArch::setFreeLinks(std::shared_ptr<MatArchNode> cnode, std::map<REAC,std::vector<std::shared_ptr<MatArchNode>>>& ret){
	auto nxt = cnode->getNextNodes();
	for(int i = 0; i < REAC::__COUNT; i++){
		if(nxt.at(REAC(i))==nullptr){
			ret.at(REAC(i)).push_back(cnode);
		}
	}
}
std::map<REAC, std::vector<std::shared_ptr<MatArchNode>>> MaterialArch::getFreeLinks(){
	std::map<REAC,std::vector<std::shared_ptr<MatArchNode>>> ret;
	for(int i = 0; i < REAC::__COUNT; i++){
		ret.emplace(REAC(i),std::vector<std::shared_ptr<MatArchNode>>());
	}
	routeAllNodes<std::map<REAC,std::vector<std::shared_ptr<MatArchNode>>>>(setFreeLinks,ret);
	return ret;
}
MaterialArch MaterialArch::operator+(const MaterialArch& other){
	MaterialArch ret = *this;
	MaterialArch o = other;
	auto retFree = ret.getFreeLinks();
	auto oFree = o.getFreeLinks();
	for(int i = 0; i < REAC::__COUNT; i++){
		for(int j = 0; j < retFree.at(REAC(i)).size(); j++){
			float val = retFree.at(REAC(i)).at(j)->getElementRawValue(REAC(i));
			int best = 0;
			float cval;
			for(int k = 1; k < oFree.at(REAC(i)).size(); k++){
				if(oFree.at(REAC(i)).at(k)->getNode(REAC(i))==nullptr){
					bool sameSign;
					float oval = 	oFree.at(REAC(i)).at(k)->getElementRawValue(REAC(i));
					float bval = 	oFree.at(REAC(i)).at(best)->getElementRawValue(REAC(i));
					sameSign = (val > 0 && oval > 0) || (val < 0 && oval < 0);
					if(std::abs(val+oval) < std::abs(val+bval) && !sameSign){
						best = k;
					}
				}
			}
			if(oFree.at(REAC(i)).size() > 0 && retFree.at(REAC(i)).at(j)->getNode(REAC(i)) == nullptr && oFree.at(REAC(i)).at(best)->getNode(REAC(i))==nullptr){
				cval = oFree.at(REAC(i)).at(best)->getElementRawValue(REAC(i));
				if( !((val > 0 && cval > 0) || (val < 0 && cval < 0))){
					retFree.at(REAC(i)).at(j)->link(REAC(i),oFree.at(REAC(i)).at(best));
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
	std::map<std::shared_ptr<MatArchNode>,std::vector<REAC>> marks;
	routeAllNodes(unlinkNodes,marks);
	for (auto node : marks) {
		auto linkList = node.second;
		std::cout << "_UNLINK_ (" <<linkList.size() << ")"<< std::endl;
		std::vector<std::shared_ptr<MatArchNode>> lost;
		for(std::size_t i = 0; i < linkList.size(); i++){
			if(node.first->getNode(linkList.at(i)) != nullptr){
				if(std::find(lost.begin(),lost.end(),node.first->getNode(linkList.at(i)))==lost.end()){
					lost.push_back(node.first->getNode(linkList.at(i)));
				}
				lost.back()->unlink(linkList.at(i));
				node.first->unlink(linkList.at(i));
			}
		}
		for(auto l:lost){
			if(l == nullptr){
				std::cout << "NULL__" << std::endl;
				break;
			}
			if(l->orpheline(m_root) && l->orpheline(node.first)){
				std::cout << "_ORPH_L" << std::endl;
				std::shared_ptr<MaterialArch> orph = std::make_shared<MaterialArch>(m_pop.get());
				orph->setRoot(l);
				m_pop->add(m_pop->orphName(),std::move(orph));
			}
		}
		if(node.first->orpheline(m_root) ){
			std::cout << "_ORPH_N" << std::endl;
			std::shared_ptr<MaterialArch> orph = std::make_shared<MaterialArch>(m_pop.get());
			orph->setRoot(node.first);
			m_pop->add(m_pop->orphName(),std::move(orph));
		}
	}
	ticked = true;
}
void MaterialArch::stepNodes(std::shared_ptr<MatArchNode> cnode){
	cnode->update();
}
void MaterialArch::unlinkNodes(std::shared_ptr<MatArchNode> cnode,std::map<std::shared_ptr<MatArchNode>,std::vector<REAC>>& marks){
	for(int i = 0; i < REAC::__COUNT; i++){
		if(std::abs(cnode->getResultReac().at(REAC(i)))<1&& cnode->getNode(REAC(i))!=nullptr){
			auto onode = cnode->getNode(REAC(i));
			if(marks.find(cnode) != marks.end()){
				if(std::find(marks.at(cnode).begin(), marks.at(cnode).end(),REAC(i)) == marks.at(cnode).end()){
					marks.at(cnode).push_back(REAC(i));
				}
			}
			else{
				marks.emplace(cnode,std::vector<REAC>(1,REAC(i)));
			}
			if(marks.find(onode) != marks.end()){
				if(std::find(marks.at(onode).begin(),marks.at(onode).end(),REAC(i)) == marks.at(onode).end()){
					marks.at(onode).push_back(REAC(i));
				}
			}
			else{
				marks.emplace(onode,std::vector<REAC>(1,REAC(i)));
			}
		}
	}
}

bool MaterialArch::getTicked(){
	return ticked;
}
void MaterialArch::closeTicked(){
	ticked = false;
}
float MaterialArch::getLinkValue(REAC r){
	return getResultReac().at(r);
}
std::map<REAC, std::vector<std::shared_ptr<MatArchNode>>> MaterialArch::getLinkedNodes(){
	std::map<REAC, std::vector<std::shared_ptr<MatArchNode>>> ret;
	for(int i = 0; i < REAC::__COUNT; i++){
		ret.emplace(REAC(i),std::vector<std::shared_ptr<MatArchNode>>());
	}
	routeAllNodes(getLinks,ret);
	return ret;
}
void MaterialArch::getLinks(std::shared_ptr<MatArchNode> cnode,std::map<REAC,std::vector<std::shared_ptr<MatArchNode>>>& marks){
	if(cnode != nullptr){
		auto links = cnode->getNextNodes();
		for (auto link : links) {
			if(link.second != nullptr){
				marks.at(link.first).push_back(link.second);
			}
		}
	}
}

void MaterialArch::counter(std::shared_ptr<MatArchNode> cnode,int& count){
	count++;
}
int MaterialArch::getCount(){
	int ret = 0;
	routeAllNodes(counter,ret);
	return ret;
}

