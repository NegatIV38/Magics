#include "MatArchNode.h"
#include "MaterialArch.h"

#include <memory>

MatArchNode::MatArchNode(std::shared_ptr<Element> elem): m_element(elem){
	for(int i = 0; i < REAC::__COUNT; i++){
		linkedNodes.emplace(REAC(i),nullptr);
	}
}
MatArchNode::MatArchNode(){
	for(int i = 0; i < REAC::__COUNT; i++){
		linkedNodes.emplace(REAC(i),nullptr);
	}
}
MatArchNode::~MatArchNode(){
}
void MatArchNode::link(REAC r, std::shared_ptr<MatArchNode> node){
	linkedNodes.at(r) = node;
	node->linkedNodes.at(r) = shared_from_this();
}
void MatArchNode::unlink(REAC r){
	linkedNodes.at(r) = nullptr;
}
std::map<REAC, float> MatArchNode::getResultReac(){
	std::map<REAC,float> ret;
	auto selfStats = m_element->getSum();
	for(int i = 0; i < REAC::__COUNT; i++){
		if(linkedNodes.at(REAC(i))!= nullptr){
			ret.emplace(REAC(i),selfStats.at(REAC(i))+linkedNodes.at(REAC(i))->getElementRawValue(REAC(i)));
		}
		else{
			ret.emplace(REAC(i),selfStats.at(REAC(i)));
		}
	}
	return ret;
}
float MatArchNode::getElementRawValue(REAC r){
	return m_element->getSum().at(r);
}
std::shared_ptr<MatArchNode> MatArchNode::getNode(REAC r){
	if(linkedNodes.at(r) != nullptr && linkedNodes.at(r).get() != nullptr && r != REAC::__COUNT){
		return linkedNodes.at(r);
	}
	return nullptr;
}
std::map<REAC, std::shared_ptr<MatArchNode>> MatArchNode::getNextNodes(){
	return linkedNodes;
}
void MatArchNode::generate(){
	if(m_element == nullptr){
		m_element = std::make_shared<Element>();
	}
	m_element->randomize();
}
void MatArchNode::update(){
	for (int i = 0; i < REAC::__COUNT; ++i) {
		if(linkedNodes.at(REAC(i)) == nullptr){
			m_element->step(REAC(i));
		}
		else{
			linkedNodes.at(REAC(i))->m_element->step(REAC(i),m_element->invstep(REAC(i)));
		}
	}
}
bool MatArchNode::orpheline(std::shared_ptr<MatArchNode> parent){
	auto nodes = MaterialArch::getAllNodes(shared_from_this());
	if(std::find(nodes.begin(),nodes.end(),parent) == nodes.end()){
		return true;
	}
	return false;
}

bool MatArchNode::orpheline(){
	bool ret = true;
	for (auto node : linkedNodes) {
		if(node.second != nullptr){
			ret = false;
		}
	}
	return ret;
}

float MatArchNode::getLinkValue(REAC r){
	return m_element->getSum().at(r) + linkedNodes.at(r)->getElementRawValue(r);
}
