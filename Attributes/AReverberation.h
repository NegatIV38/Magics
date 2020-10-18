#ifndef AREVERBERATION_H
#define AREVERBERATION_H

#include "Attribute.h"

class AReverberation : Attribute
{
public:
	enum Reverberation{MATT, SHINY, REFLECTIVE};
	AReverberation();
	AReverberation(Reverberation reverb);
	virtual ~AReverberation();

	Reverberation getReverb();
	void setReverb(Reverberation reverb);
private:
	Reverberation m_reverb;	

};

#endif /* AREVERBERATION_H */
