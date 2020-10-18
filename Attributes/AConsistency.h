#ifndef ACONSISTENCY_H
#define ACONSISTENCY_H

#include "AMatModifier.h"

class AConsistency : AMatModifier
{
public:
	enum Consistency{SOLID, SUPPLE, VISCOUS, LIMP, LIQUID, ETHEREAL};
	AConsistency();
	AConsistency(Consistency consist);
	virtual ~AConsistency();

	Consistency getConsist();
	void setConsist(Consistency consist);


private:
	Consistency m_consist;	
};

#endif /* ACONSISTENCY_H */
