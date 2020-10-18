#include "AAllure.h"

AAllure::AAllure(){
	m_allure = Allure::SLEEK;
}

AAllure::AAllure(Allure all) : m_allure(all){

}

AAllure::~AAllure(){

}

AAllure::Allure AAllure::getAllure(){
	return m_allure;
}

void AAllure::setAllure(Allure all){
	m_allure =all;
}
