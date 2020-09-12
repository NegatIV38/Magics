#include "Gaz.h"

Gaz::Gaz() : Element(){

}

Gaz::Gaz(int rank) : Element(rank){

	exalt(REACTION::SOUFFLE,10);
	exalt(REACTION::INFLAMMATION, 4);
	exalt(REACTION::EXPLOSION,2);
	exalt(REACTION::TOXICITE, 7);
	exalt(REACTION::GEL , 5);
	
	inhib(REACTION::SUBLIMATION, 8);
	

}

Gaz::~Gaz(){

}
