#ifndef ASHATTERING_H
#define ASHATTERING_H

#include "Attribute.h"

class AShattering : Attribute
{
public:
	enum Shattering{TEAR, BREAK, DEFORMATION, BURST};
	AShattering();
	AShattering(Shattering shatter);
	virtual ~AShattering();

	Shattering getShattering();
	void setShattering(Shattering shatter);

private:
	Shattering m_shatter;
};

#endif /* ASHATTERING_H */
