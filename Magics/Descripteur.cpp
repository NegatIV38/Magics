#include "Descripteur.h"

Descripteur::Descripteur(){
	m_name = "?";
	m_function = std::make_shared<Function>(nullptr);
	//m_parent = std::make_shared<Descripteur>(nullptr);
}

Descripteur::Descripteur(Descripteur& copy){
	m_name = copy.m_name;
	m_function = copy.m_function;
	m_childrens = copy.m_childrens;
}

Descripteur::Descripteur(std::string name, const std::shared_ptr<Function>& fun)
	: m_name(name),m_function(fun)
{

}

Descripteur::~Descripteur(){

}

void Descripteur::print(int indent){
	if(m_name != "?"){
		for(int d = 0; d < indent; d++){	std::cout << "\t";	}
		std::cout << "Name:\t" << m_name << std::endl;
	}
	for(int d = 0; d < indent; d++){	std::cout << "\t";	}
	std::cout << "Function:\t" << m_function->getName() << std::endl;
	std::cout <<std::endl;
	for(std::vector<std::shared_ptr<Descripteur>>::iterator it = m_childrens.begin(); it != m_childrens.end(); it++){
		it->get()->print(indent+1);
	}
}

std::vector<std::string> Descripteur::getState(int indent){
	std::vector<std::string> ret;
	std::string temp = "";
	if(m_name != "?"){
		for(int d = 0; d < indent; d++){	temp += "\t";	}
		temp += "Name:\t" + m_name;
		ret.push_back(temp);
	}
	temp = "";
	for(int d = 0; d < indent; d++){	temp += "\t";	}
	temp += "Function:\t" + m_function->getName();
	ret.push_back(temp);
	for(std::vector<std::shared_ptr<Descripteur>>::iterator it = m_childrens.begin(); it != m_childrens.end(); it++){
		std::vector<std::string> tret =it->get()->getState(indent+1);
		ret.insert(ret.end(),tret.begin(),tret.end());
	}
	return ret;
}

void Descripteur::setFunction(std::shared_ptr<Function> fun){
	m_function = fun;
}

void Descripteur::addChild(const std::shared_ptr<Descripteur>& child){
	m_childrens.push_back(child);
}
void Descripteur::removeChild(const std::shared_ptr<Descripteur>& child){
	m_childrens.erase(std::find(m_childrens.begin(), m_childrens.end(),child));
}

void Descripteur::setParent(const std::shared_ptr<Descripteur>& parent){
	//m_parent = parent;
	parent->addChild(std::make_shared<Descripteur>(*this));
}

void Descripteur::printFunction(){
	m_function->print();
}

std::vector<std::shared_ptr<Descripteur>> Descripteur::getChildrens(){
	return m_childrens;
}
