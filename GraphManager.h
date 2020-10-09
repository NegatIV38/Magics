#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include "MaterialArchView.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>
#include <vector>


class GraphManager
{
private:
	
	std::map<std::string ,std::shared_ptr<MaterialArchView>> m_matViews;
	std::map<std::string, bool> m_visibility;

public:
	static sf::Clock clock;
	static double dt;
	GraphManager();
	virtual ~GraphManager();

	void update();
	void draw(std::shared_ptr<sf::RenderWindow> win);

	void addMaterial(std::string name,std::shared_ptr<MaterialArch> arch);
	void show(std::string name);
	void hide(std::string name);
};

#endif /* GRAPHMANAGER_H */
