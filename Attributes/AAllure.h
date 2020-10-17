#ifndef AALLURE_H
#define AALLURE_H

#include "Attribute.h"

class AAllure : Attribute 
{
private:
	

public:
	/*! \enum Allure
	 *
	 *  Detailed description
	 */
	enum Allure { RUGGED, SLEEK, BUMPY, RIBBED };

	AAllure();
	virtual ~AAllure();
};

#endif /* AALLURE_H */
