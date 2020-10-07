#include "Material.h"

Material::Material(){

}
Material::Material(int nbElem){
	for(int i = 0; i < nbElem; i++){
		m_elements.push_back(std::make_shared<Element>());
		m_elements.back()->randomize();
	}
	m_stableRank = nbElem;
}

Material::~Material(){

}
Material Material::result(Material reac){
	std::vector<std::shared_ptr<Element>> elem = m_elements;
	for(auto r: reac.m_elements){
		elem.push_back(r);
	}
	Material ret;
	ret.setStats(elem);
	return ret;
}

std::pair<Material,Material> Material::transmute(Material reac, std::map<Element::REACTION,float> result){
	Material retGood, retBad;
	retGood = *this;
	retBad = reac;

	Element g = retGood.getGlobal();
	auto reacSum = retBad.getGlobal().getSum();
	auto sum = g.getSum();
	auto dif = result;
	bool finished = false;
	while(!finished){
		finished = true;
		bool reaction = false;
		for(int i = 0; i < int(Element::REACTION::__COUNT); i++){
			dif.at(Element::REACTION(i))-=sum.at(Element::REACTION(i));
			if(dif.at(Element::REACTION(i)) <= 0){
				finished = false;
			}
			if(reacSum.at(Element::REACTION(i)) > 0 && dif.at(Element::REACTION(i)) > 0){
				reaction = true;
			}
		}	
		if(!reaction){
			finished = true;
		}
		
		
		std::cout << "You need : \t\t You have :  \t\t Target : \t\t Reac :  " << std::endl;
		for(int i = 0; i < int(Element::REACTION::__COUNT); i++){
			std::cout << Element::reacToStr(Element::REACTION(i)) << ": \t"<< dif.at(Element::REACTION(i)) << "\t\t"  << sum.at(Element::REACTION(i))  << "\t\t"<< result.at(Element::REACTION(i)) << "\t\t" << reacSum.at(Element::REACTION(i)) <<  std::endl;
			if(dif.at(Element::REACTION(i)) > 0){
				retGood.m_elements.at(getMaxElement(Element::REACTION(i)))->absorb(retBad.m_elements.at(reac.getMaxElement(Element::REACTION(i))));
			}
		}	
		std::cout << std::endl;
		dif = result;
		g = retGood.getGlobal();
		sum = g.getSum();
		reacSum = retBad.getGlobal().getSum();
		
		
	}

	retGood.print();

	return std::pair<Material,Material>(retGood,retBad);
}

void Material::setStats(std::vector<std::shared_ptr<Element>> elem){
	m_elements = elem;
}
int Material::getMaxElement(Element::REACTION r){
	int ret =0;
	for(int i = 1;i <m_elements.size(); i++){
		if(m_elements.at(i)->getSum().at(r) > m_elements.at(ret)->getSum().at(r)){
			ret = i;
		}
	} 
	return ret;
}

Element Material::getGlobal(){
	Element global;
	global.null();
	for(auto& elem : m_elements){
		global.absorb(*elem);
	}
	return global;
}

void Material::print(){
	Element global;
	global.null();
	for(auto& elem : m_elements){
		elem->print();
		std::cout << std::endl;
		global.absorb(*elem);
	}
	std::cout << "SUM : ----------------------------------" << std::endl;
	global.print();

}
