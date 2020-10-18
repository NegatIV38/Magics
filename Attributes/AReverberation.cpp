#include "AReverberation.h"

AReverberation::AReverberation(){

}

AReverberation::AReverberation(Reverberation reverb) : m_reverb(reverb){

}

AReverberation::~AReverberation(){

}

AReverberation::Reverberation AReverberation::getReverb(){
	return m_reverb;
}

void AReverberation::setReverb(Reverberation reverb){
	m_reverb = reverb;
}
