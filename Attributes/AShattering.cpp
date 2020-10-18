#include "AShattering.h"

AShattering::AShattering(){

}

AShattering::AShattering(Shattering shatter):m_shatter(shatter){
}

AShattering::~AShattering(){

}

AShattering::Shattering AShattering::getShattering(){
	return m_shatter;
}

void AShattering::setShattering(Shattering shatter){
	m_shatter = shatter;
}
