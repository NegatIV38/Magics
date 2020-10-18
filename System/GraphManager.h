#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include "../Physics/MaterialPop.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>
#include <vector>

class GraphManager
{
private:
	std::shared_ptr<MaterialPop> m_matPop;

public:
	GraphManager();
	virtual ~GraphManager();

	void update();
	void draw(std::shared_ptr<sf::RenderWindow> win);

	void setMaterialPop(std::shared_ptr<MaterialPop> pop);
};

#endif /* GRAPHMANAGER_H */
