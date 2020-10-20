#include "Element.h"

Element::Element(){
	initStable();	
}
Element::Element(int rank){
	initPower(rank);
}
Element::~Element(){
}
void Element::initStable(){
	for (int i = 0; i < REACTION::__COUNT; ++i) {
		m_reactions.emplace(REACTION(i),1.f);
	}
	stepFactor=1;
}
void Element::randomize(){
	for(int i = 0; i < int(REACTION::__COUNT); i++){
		m_reactions.at(REACTION(i)) =( -500+(rand()%1000))/100.f;
	}	
	//stepFactor = (-500+rand()%1000)/100.f;
	stepFactor = rand()%1000/100.f;
}
void Element::initPower(int rank , bool random){
	initStable();
	m_reactions.clear();
	for(int i = 0; i < int(REACTION::__COUNT); i++){
		if(random){
			m_reactions.emplace(REACTION(i) , (rand()%(rank*1000))/1000.f);
		}
		else{
			m_reactions.emplace(REACTION(i) , rank);
		}
	}
} 
void Element::null(){
	for (int i = 0; i < REACTION::__COUNT; ++i) {
		m_reactions.at(REACTION(i)) = 0;
	}
}
Element::REACTION Element::strToReac(std::string str){
	std::string s = str;
	for(int i = 0; i < s.size(); i++){
		s[i] = std::toupper(s[i]);
	}
	if(s == "CHALEUR"){
		return CHALEUR;
	}
	if(s == "MAGNETISME"){
		return MAGNETISME;
	}
	if(s == "PH"){
		return PH;
	}
	if(s == "CONDUCTIVITE"){
		return CONDUCTIVITE;
	}
	if(s == "RADIOACTIVITE"){
		return RADIOACTIVITE;
	}
	return __COUNT;
}
std::string Element::reacToStr(REACTION r){
	switch(r){
		case REACTION::CHALEUR:
			return "CHALEUR";
		case REACTION::MAGNETISME:
			return "MAGNETISME";
		case REACTION::CONDUCTIVITE:
			return "CONDUCTIVITE";
		case REACTION::PH:
			return "PH";
		case REACTION::RADIOACTIVITE:
			return "RADIOACTIVITE";
		default:
			return "";
	}
	return "ERR";
}
std::map<Element::REACTION, float> Element::getSum(){
	return m_reactions;
}
void Element::step(REACTION r, float step){
	m_reactions.at(r) += step;
}
void Element::step(REACTION r){
	m_reactions.at(r) += stepFactor*(m_reactions.at(r)>=1?1:(m_reactions.at(r)<=-1?-1:0));
}
