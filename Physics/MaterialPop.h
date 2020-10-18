#ifndef MATERIALPOP_H
#define MATERIALPOP_H

#include "MaterialArch.h"
#include "MaterialArchView.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <string>

class MaterialPop
{
private:
	
	std::map<std::string, std::shared_ptr<MaterialArch>> m_varMaterials;
	std::map<std::string, std::shared_ptr<MaterialArchView>> m_views;

public:
	MaterialPop();
	virtual ~MaterialPop();

	void add(std::string name, std::shared_ptr<MaterialArch> mat);
	void update();
	void draw(std::shared_ptr<sf::RenderWindow> win);
	std::shared_ptr<MaterialArch> get(std::string name);
	std::shared_ptr<MaterialArchView> getView(std::string name);
	bool find(std::string name);
		
};

#endif /* MATERIALPOP_H */
