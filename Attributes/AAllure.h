#ifndef AALLURE_H
#define AALLURE_H

#include "Attribute.h"

class AAllure : Attribute 
{
public:
	/*! \enum Allure
	 *
	 *  Detailed description
	 */
	enum Allure { RUGGED, SLEEK, BUMPY, RIBBED };

	AAllure();
	AAllure(Allure all);
	virtual ~AAllure();

	Allure getAllure();
	void setAllure(Allure all);

private:
	Allure m_allure;	


};

#endif /* AALLURE_H */
