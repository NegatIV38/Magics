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

	std::cout << "STATS : --------------------------------"<< std::endl;
	float meanA=0, meanR=0, meanS = 0;
	int max1=0, max2=0, max3=0;
	std::vector<float> s;
	for(int i = 0; i < int(REACTION::__COUNT); i++){
		meanA += m_activation.at(REACTION(i));
		meanR += m_resistance.at(REACTION(i)); 
		meanS += m_activation.at(REACTION(i))- m_resistance.at(REACTION(i));
		s.push_back(m_activation.at(REACTION(i))-m_resistance.at(REACTION(i)));
		
	}
	for(int j = 0; j < s.size(); j++){
		if(s.at(j) > s.at(max1)){
			max1=j;
		}
	}
	for(int j = 0; j < s.size(); j++){
		if((s.at(j) > s.at(max2) || max2 == max1)&& j != max1){
			max2=j;
		}
	}
	for(int j = 0; j < s.size(); j++){
		if((s.at(j) > s.at(max3) || max3==max2 || max3==max1)&& j!=max1 && j!=max2){
		max3=j;
	}
	}
	if(s.at(max1) < 1){
		max1= REACTION::__COUNT;
	}
	if(s.at(max2) < 1){
		max2= REACTION::__COUNT;
	}

	if(s.at(max3) < 1){
		max3= REACTION::__COUNT;
	}
	meanA /= REACTION::__COUNT;
	meanR /= REACTION::__COUNT;
	meanS /= REACTION::__COUNT;
	std::cout << "MEAN :\t" << meanA << "\t\t" << meanR << "\t\t" << meanS << std::endl;
	std::cout << "MAX :\tMAX1 : " << reacToStr(REACTION(max1)) << "\t\tMAX2: " << reacToStr(REACTION(max2))<< "\t\tMAX3 : " << reacToStr(REACTION(max3)) << std::endl;
}

void Element::randomize(){
	for(int i = 0; i < int(REACTION::__COUNT); i++){
		m_activation.at(REACTION(i)) = (rand()%1000)/100.f;
		m_resistance.at(REACTION(i)) = (rand()%1000)/100.f;
	}	
}
void Element::initPower(int rank , bool random){
	for(int i = 0; i < int(REACTION::__COUNT); i++){
		if(random){
			m_activation.at(REACTION(i)) = (rand()%(rank*1000))/1000.f;
			m_resistance.at(REACTION(i)) = (rand()%(rank*1000))/1000.f;
		}
		else{
			m_activation.at(REACTION(i)) = rank;
			m_resistance.at(REACTION(i)) = rank;
		
		}
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
			return "";
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
std::map<Element::REACTION, float> Element::getSum(){
	std::map<Element::REACTION, float> sum;
	for(int i = 0; i < int(REACTION::__COUNT); i++){
		sum.emplace(REACTION(i), m_activation.at(REACTION(i))-m_resistance.at(REACTION(i)));
	}
	return sum;
}

void Element::absorb(Element elem){
	for(int i = 0; i < int(REACTION::__COUNT); i++){
		m_activation.at(REACTION(i)) += elem.getActivation().at(REACTION(i));
		m_resistance.at(REACTION(i)) += elem.getResistance().at(REACTION(i));
	}
}
void Element::absorb(std::shared_ptr<Element> elem){
	for(int i = 0; i < int(REACTION::__COUNT); i++){
		m_activation.at(REACTION(i)) += elem->getActivation().at(REACTION(i));
		m_resistance.at(REACTION(i)) += elem->getResistance().at(REACTION(i));
		elem->m_activation.at(REACTION(i)) =0;
		elem->m_resistance.at(REACTION(i)) =0;
		
	}
}

void Element::exalt(REACTION r , float weight){
	m_activation.at(r) *= weight;

}

void Element::inhib(REACTION r , float weight){
	m_resistance.at(r) *= weight;
}
