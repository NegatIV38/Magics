#ifndef MATERIALARCH_H
#define MATERIALARCH_H

#include "MatArchNode.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <queue>
#include <vector>

class MaterialPop;

class MaterialArch
{
private:
	std::shared_ptr<MatArchNode> m_root;	
	std::shared_ptr<MaterialPop> m_pop;
	sf::Clock m_clock;
	bool ticked;

	static void sumStatsNodes(std::shared_ptr<MatArchNode> cnode, std::map<REAC,float>& sum);
	static void sumStatsSign(std::shared_ptr<MatArchNode> cnode,std::map<REAC,float>& sum, bool& positive);
	static void setFreeLinks(std::shared_ptr<MatArchNode> cnode, std::map<REAC,std::vector<std::shared_ptr<MatArchNode>>>& ret);
	static void stepNodes(std::shared_ptr<MatArchNode> cnode);
	static void unlinkNodes(std::shared_ptr<MatArchNode> cnode,std::map<std::shared_ptr<MatArchNode>,std::vector<REAC>>& marks);
	static void getLinks(std::shared_ptr<MatArchNode> cnode,std::map<REAC,std::vector<std::shared_ptr<MatArchNode>>>& marks);
	static void counter(std::shared_ptr<MatArchNode> cnode,int& count);


public:
	MaterialArch(MaterialPop* pop);
	virtual ~MaterialArch();

	static std::vector<std::shared_ptr<MatArchNode>> getAllNodes(std::shared_ptr<MatArchNode> root);

	void update();
	void updateStep();

	void setRoot(std::shared_ptr<MatArchNode> r);
	std::shared_ptr<MatArchNode> getRoot();
	std::map<REAC, float> getResultReac();
	float getLinkValue(REAC r);
	std::map<REAC,float> getReacOfSign(bool positive = true);
	std::map<REAC, std::vector<std::shared_ptr<MatArchNode>>> getFreeLinks();
	std::map<REAC, std::vector<std::shared_ptr<MatArchNode>>> getLinkedNodes();
	void generate(int nbElem);
	void routeAllNodes(void(*fun)(std::shared_ptr<MatArchNode> currNode));
	bool getTicked();
	void closeTicked();
	
	int getCount();
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
#endif /* MATERIALARCH_H */

