#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

class Element{

	public:

enum REACTION{
	ATTRACTION, CORROSION,  DECHARGE, ELEC_STATIC,EXPLOSION, 
	GEL,INFLAMMATION,
	LIQUEFACTION,	
 	RADIOACTIVITE, REPULSION,
	SOLIDIFICATION, SOUFFLE,SUBLIMATION,TOXICITE,
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
		static REACTION strToReac(std::string str);

		std::map<REACTION, float> getResistance();
		std::map<REACTION, float> getActivation();
		std::map<REACTION, float> getSum();

		std::vector<std::string> getPrintableState();

		void absorb(Element elem);
		void absorb(std::shared_ptr<Element> elem);
	private:
		void initStable();
	
		//std::string m_name;
	
		std::map<REACTION, float> m_activation;
		std::map<REACTION, float> m_resistance;

};

#endif
