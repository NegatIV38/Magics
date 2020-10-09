#ifndef MATERIALARCH_H
#define MATERIALARCH_H

#include "MatArchNode.h"
#include <memory>
#include <queue>
#include <vector>

class MaterialArch
{
private:
	std::shared_ptr<MatArchNode> m_root;	

	static void sumStatsNodes(std::shared_ptr<MatArchNode> cnode, std::map<Element::REACTION,float>& sum);
	static void setFreeLinks(std::shared_ptr<MatArchNode> cnode, std::map<Element::REACTION,std::vector<std::shared_ptr<MatArchNode>>>& ret);

public:
	MaterialArch();
	virtual ~MaterialArch();

	void setRoot(std::shared_ptr<MatArchNode> r);
	std::shared_ptr<MatArchNode> getRoot();
	std::map<Element::REACTION, float> getResultReac();
	std::map<Element::REACTION, std::vector<std::shared_ptr<MatArchNode>>> getFreeLinks();
	void generate(int nbElem);
	void routeAllNodes(void(*fun)(std::shared_ptr<MatArchNode> currNode));
	
	std::shared_ptr<MaterialArch> combine(std::shared_ptr<MaterialArch> other);
	MaterialArch operator+(const MaterialArch& other);	

	template <typename T>
	void routeAllNodes(void(*fun)(std::shared_ptr<MatArchNode> currNode, T& extObject), T& extObject);
	template <typename T, typename U>
	void routeAllNodes(void(*fun)(std::shared_ptr<MatArchNode> currNode, T& extObjectA, U& extObjectB), T& extObjectA, U& extObjectB);
};

template <typename T>
void MaterialArch::routeAllNodes(void(*fun)(std::shared_ptr<MatArchNode> currNode,T& extObject),T& extObject){
	std::vector<std::shared_ptr<MatArchNode>> visited;
	std::deque<std::shared_ptr<MatArchNode>> toVisit;
	if(m_root != nullptr){
		toVisit.push_back(m_root);
	}
	while(toVisit.size() > 0){
		auto current = toVisit.front();
		toVisit.pop_front();

		fun(current,extObject);

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


template <typename T,typename U>
void MaterialArch::routeAllNodes(void(*fun)(std::shared_ptr<MatArchNode> currNode,T& extObjectA, U& extObjectB),T& extObjectA, U& extObjectB){
	std::vector<std::shared_ptr<MatArchNode>> visited;
	std::deque<std::shared_ptr<MatArchNode>> toVisit;
	if(m_root != nullptr){
		toVisit.push_back(m_root);
	}
	while(toVisit.size() > 0){
		auto current = toVisit.front();
		toVisit.pop_front();

		fun(current,extObjectA,extObjectB);

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
#endif /* MATERIALARCH_H */

