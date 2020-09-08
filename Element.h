#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include <map>
#include <iostream>

class Element{

	public:

enum REACTION{
	SOUFFLE, ATTRACTION, REPULSION, 
	INFLAMMATION, EXPLOSION,
	GEL, LIQUEFACTION,
	CORROSION, TOXICITE,
 	DECHARGE, RADIOACTIVITE, ELEC_STATIC,
	SOLIDIFICATION, SUBLIMATION,
	__COUNT
};
		Element();
		~Element();
		
		void print();
		void randomize();
		void null();

		std::map<REACTION, float> getResistance();
		std::map<REACTION, float> getActivation();

		void absorb(Element elem);
	private:
		void initStable();
		std::string reacToStr(REACTION r);
	
		std::map<REACTION, float> m_activation;
		std::map<REACTION, float> m_resistance;

};

#endif
