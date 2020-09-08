#ifndef __MATERIAL_H__
#define __MATERIAL_H__ 

#include "Attribute.h"
#include "Element.h"


class Material : public Attribute{

	public:
		Material();
		Material(int nbElem);
		~Material();	
	
		void print();

	private:
		std::vector<std::shared_ptr<Element>> m_elements;
}; 


#endif

