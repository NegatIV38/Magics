#include "MatArchNode.h"
#include "MaterialArch.h"

#include <memory>

MatArchNode::MatArchNode(std::shared_ptr<Element> elem): m_element(elem){
	for(int i = 0; i < Element::REACTION::__COUNT; i++){
		linkedNodes.emplace(Element::REACTION(i),nullptr);
	}
}
MatArchNode::MatArchNode(){
	for(int i = 0; i < Element::REACTION::__COUNT; i++){
		linkedNodes.emplace(Element::REACTION(i),nullptr);
	}
}
MatArchNode::~MatArchNode(){
}
void MatArchNode::link(Element::REACTION r, std::shared_ptr<MatArchNode> node){
	linkedNodes.at(r) = node;
	node->linkedNodes.at(r) = shared_from_this();
}
void MatArchNode::unlink(Element::REACTION r){
	linkedNodes.at(r).reset();
}
std::map<Element::REACTION, float> MatArchNode::getResultReac(){
	std::map<Element::REACTION,float> ret;
	auto selfStats = m_element->getSum();
	for(int i = 0; i < Element::REACTION::__COUNT; i++){
		if(linkedNodes.at(Element::REACTION(i))!= nullptr){
			ret.emplace(Element::REACTION(i),selfStats.at(Element::REACTION(i))+linkedNodes.at(Element::REACTION(i))->getElementRawValue(Element::REACTION(i)));	
		}
		else{
			ret.emplace(Element::REACTION(i),selfStats.at(Element::REACTION(i)));
		}
	}
	return ret;
}
float MatArchNode::getElementRawValue(Element::REACTION r){
	return m_element->getSum().at(r);
}
std::shared_ptr<MatArchNode> MatArchNode::getNode(Element::REACTION r){
	if(linkedNodes.at(r) != nullptr && linkedNodes.at(r).get() != nullptr && r != Element::REACTION::__COUNT){
		return linkedNodes.at(r);
	}
	return nullptr;
}
std::map<Element::REACTION, std::shared_ptr<MatArchNode>> MatArchNode::getNextNodes(){
	return linkedNodes;
}
void MatArchNode::generate(){
	if(m_element == nullptr){
		m_element = std::make_shared<Element>();
	}
	m_element->randomize();
}
void MatArchNode::update(){
	for (int i = 0; i < Element::REACTION::__COUNT; ++i) {
		if(linkedNodes.at(Element::REACTION(i)) == nullptr){
			m_element->step(Element::REACTION(i));
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

