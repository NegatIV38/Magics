#ifndef __MATERIAL_H__
#define __MATERIAL_H__ 

#include "Element.h"


class Material {

	public:
		Material();
		Material(int nbElem);
		~Material();	
	
		void print();
		Material result(Material reac);
		std::pair<Material,Material> transmute(Material reac, std::map<Element::REACTION,float> result);
		void setStats(std::vector<std::shared_ptr<Element>> elem);
		Element getGlobal();
		int getMaxElement(Element::REACTION r);
	private:
		std::vector<std::shared_ptr<Element>> m_elements;
		int m_stableRank;
}; 


#endif

