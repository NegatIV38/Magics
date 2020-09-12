#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include <map>
#include <iostream>
#include <vector>
#include <memory>

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
		Element(int rank);
		~Element();
		
		void print();
		void randomize();
		void null();
		void initPower(int rank, bool random = true);
		void exalt(REACTION r, float weight);
		void inhib(REACTION r, float weight);

		static std::string reacToStr(REACTION r);

		std::map<REACTION, float> getResistance();
		std::map<REACTION, float> getActivation();
		std::map<REACTION, float> getSum();

		void absorb(Element elem);
		void absorb(std::shared_ptr<Element> elem);
	private:
		void initStable();
	
		std::map<REACTION, float> m_activation;
		std::map<REACTION, float> m_resistance;

};

#endif
