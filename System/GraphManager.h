#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

//#include "../Physics/MaterialArchView.h"
#include "../Physics/MaterialPop.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>
#include <vector>


class GraphManager
{
private:
	
//	std::map<std::string ,std::shared_ptr<MaterialArchView>> m_matViews;
//	std::map<std::string, bool> m_visibility;
//	int m_visibilityCounter;
	std::shared_ptr<MaterialPop> m_matPop;

public:
	static sf::Clock clock;
	static double dt;
	GraphManager();
	virtual ~GraphManager();

	void update();
	void draw(std::shared_ptr<sf::RenderWindow> win);

	//void addMaterial(std::string name,std::shared_ptr<MaterialArch> arch);
	void show(std::string name);
	void hide(std::string name);
	void hideAll();

	void setMaterialPop(std::shared_ptr<MaterialPop> pop);
};

#endif /* GRAPHMANAGER_H */
