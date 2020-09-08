#include "Material.h"

Material::Material(){

}
Material::Material(int nbElem){
	for(int i = 0; i < nbElem; i++){
		m_elements.push_back(std::make_shared<Element>());
		m_elements.back()->randomize();
	}
}

Material::~Material(){

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
