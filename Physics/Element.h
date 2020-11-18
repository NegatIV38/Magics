#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include <map>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>


class Element{

	public:

enum REACTION{
	CHALEUR,
	MAGNETISME, PH,
	CONDUCTIVITE, RADIOACTIVITE,
	__COUNT//,
};
		Element();
		Element(int rank);
		~Element();

	//	void print();
		void randomize();
		void null();
		void initPower(int rank, bool random = true);
		void step(REACTION r);
		void step(REACTION r, float step);
		float invstep(REACTION r);

		static std::string reacToStr(REACTION r);
		static REACTION strToReac(std::string str);

		std::map<REACTION, float> getSum();

		//std::vector<std::string> getPrintableState();

	private:
		void initStable();

		float stepFactor;
		//std::string m_name;
		bool reduceStep;

		std::map<REACTION, float> m_reactions;

};

typedef Element::REACTION REAC;
typedef sf::Color rgb;
#endif
