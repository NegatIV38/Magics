#include <SFML/Graphics.hpp>
#include "Magics/Descripteur.h"
#include "Magics/Function.h"
#include "System/Console.h"
#include "Physics/Element.h"
#include "Physics/Material.h"
#include <ctime>
#include <memory>
#include "Physics/MaterialArchView.h"
#include "System/GraphManager.h"

int main()
{
	srand(time(nullptr));
	std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(sf::VideoMode(1800, 600), "SFML works!");
	Descripteur d("test",std::make_shared<Function>("objet"));

	std::map<Element::REACTION, float> need;
	for(int i = 0; i < int(Element::REACTION::__COUNT); i++){
		need.emplace(Element::REACTION(i), i);
	}
	std::shared_ptr<GraphManager> gmgr = std::make_shared<GraphManager>();
	Console cwin(window, gmgr);

	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}
			if(event.type == sf::Event::KeyPressed){
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)){
					cwin.toggle();
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
					if(cwin.isVisible()){
						cwin.upArrow();
					}
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){
					if(cwin.isVisible()){
						cwin.downArrow();
					}
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::PageDown)){
					if(cwin.isVisible()){
						cwin.pageDown();
					}
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::PageUp)){
					if(cwin.isVisible()){
						cwin.pageUp();
					}
				}
			}
			if(event.type == sf::Event::TextEntered){
				if(cwin.isVisible()){
					cwin.setCurrentCmd(event.text.unicode);
				}
			}
		}
		GraphManager::dt = GraphManager::clock.getElapsedTime().asMilliseconds();
		GraphManager::clock.restart();
		gmgr->update();
		cwin.update();
		window->clear();
		gmgr->draw(window);
		if(cwin.isVisible()){
			cwin.draw();
		}
		window->display();
	}
	return 0;
}
