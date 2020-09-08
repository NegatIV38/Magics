#ifndef __DESCRIPTEUR_H__
#define __DESCRIPTEUR_H__

#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include "Function.h"


class Descripteur{

	public:
		Descripteur();
		Descripteur(std::string name, const std::shared_ptr<Function>& fun);
		Descripteur(Descripteur& copy);
		~Descripteur();

		void addChild(const std::shared_ptr<Descripteur>& child);
		void removeChild(const std::shared_ptr<Descripteur>& child);
		void setParent(const std::shared_ptr<Descripteur>& parent);
		void print(int indent = 0);
		void rename(std::string name);
		std::vector<std::string> getState(int indent = 0);
		std::vector<std::shared_ptr<Descripteur>> getChildrens();
		void printFunction();
		void setFunction(std::shared_ptr<Function> fun);

	protected:
		std::string m_name;
		std::shared_ptr<Function> m_function;
		std::vector<std::shared_ptr<Descripteur>> m_childrens;
		//std::shared_ptr<Descripteur> m_parent;
};
#endif
