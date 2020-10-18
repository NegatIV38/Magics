#include "AConsistency.h"

AConsistency::AConsistency(){

}

AConsistency::AConsistency(Consistency consist):m_consist(consist){

}
AConsistency::~AConsistency(){

}

AConsistency::Consistency AConsistency::getConsist(){
	return m_consist;
}

void AConsistency::setConsist(Consistency consist){
	m_consist = consist;
}
