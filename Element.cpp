#include "Element.h"

Element::Element(){
	initStable();	
}

Element::~Element(){

}

void Element::initStable(){
	m_activation.clear();
	m_resistance.clear();
	for(int i = 0; i < int(REACTION::__COUNT); i++){
		m_activation.emplace(REACTION(i),1.0f);
		m_resistance.emplace(REACTION(i),1.0f);
	}
}

void Element::print(){
	for(int i = 0; i < int(REACTION::__COUNT); i++){
		std::cout << reacToStr(REACTION(i)) << " :\tActiv: " <<m_activation.at(REACTION(i)) << " \tResist: "<< m_resistance.at(REACTION(i))<< " \tSum: "<< m_activation.at(REACTION(i))- m_resistance.at(REACTION(i))<<std::endl;
	}
}

void Element::randomize(){
	for(int i = 0; i < int(REACTION::__COUNT); i++){
		m_activation.at(REACTION(i)) = (rand()%1000)/100.f;
		m_resistance.at(REACTION(i)) = (rand()%1000)/100.f;
	}	
}
void Element::null(){
	for(int i = 0; i < int(REACTION::__COUNT); i++){
		m_activation.at(REACTION(i)) = 0;
		m_resistance.at(REACTION(i)) = 0;
	}	
}



std::string Element::reacToStr(REACTION r){
	switch(r){
		case REACTION::SOUFFLE:
			return "SOUFFLE";
		case REACTION::SUBLIMATION:
			return "SUBLIMATION";
		case REACTION::INFLAMMATION:
			return "INFLAMMATION";
		case REACTION::ATTRACTION:
			return "ATTRACTION";
		case REACTION::REPULSION:
			return "REPULSION";
		case REACTION::EXPLOSION:
			return "EXPLOSION";
		case REACTION::GEL:
			return "GEL";
		case REACTION::LIQUEFACTION:
			return "LIQUEFACTION";
		case REACTION::RADIOACTIVITE:
			return "RADIOACTIVITE";
		case REACTION::DECHARGE:
			return "DECHARGE";
		case REACTION::CORROSION:
			return "CORROSION";
		case REACTION::TOXICITE:
			return "TOXICITE";
		case REACTION::ELEC_STATIC:
			return "ELEC_STATIC";
		case REACTION::SOLIDIFICATION:
			return "SOLIDIFICATION";
		default:
			break;

	}
	return "ERR";
}

std::map<Element::REACTION, float> Element::getResistance(){
	return m_resistance;
}
std::map<Element::REACTION, float> Element::getActivation(){
	return m_activation;
}

void Element::absorb(Element elem){
	for(int i = 0; i < int(REACTION::__COUNT); i++){
		m_activation.at(REACTION(i)) += elem.getActivation().at(REACTION(i));
		m_resistance.at(REACTION(i)) += elem.getResistance().at(REACTION(i));
	}
}
