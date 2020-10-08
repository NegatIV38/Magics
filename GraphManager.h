#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include "MaterialArchView.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <vector>

class GraphManager
{
private:
	
	std::vector<std::shared_ptr<MaterialArchView>> m_matViews;

public:
	GraphManager();
	virtual ~GraphManager();

	void update();
	void draw(std::shared_ptr<sf::RenderWindow> win);

	void addMaterial(std::shared_ptr<MaterialArch> arch);
};

#endif /* GRAPHMANAGER_H */
