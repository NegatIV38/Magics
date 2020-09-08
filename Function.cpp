#include "Function.h"

Function::Function(){
	m_name = "?";
}

Function::Function(std::string name)
	: m_name(name)
{

}

Function::~Function(){

}


bool Function::isLeaf(){
	return m_attributes.size() == 0;
}

void Function::addAttribute(const std::shared_ptr<Function>& attr){
	m_attributes.push_back(attr);
}

void Function::print(int indent){
	for(int d = 0; d < indent; d++){	std::cout << "\t";	}
	std::cout << "Name:\t" << m_name << std::endl;
	
	//for(int d = 0; d < indent; d++){	std::cout << "\t";	}
	//std::cout << "Attribute:\t" << m_function->getName() << std::endl;
	std::cout <<std::endl;
	for(std::vector<std::shared_ptr<Function>>::iterator it = m_attributes.begin(); it != m_attributes.end(); it++){
		it->get()->print(indent+1);
	}
}


std::string Function::getName(){
	return m_name;
}


std::vector<std::shared_ptr<Function>> Function::getAttributes(){
	return m_attributes;
}
