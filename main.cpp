#include <SFML/Graphics.hpp>
#include "Descripteur.h"
#include "Function.h"
#include "Console.h"
#include "Element.h"
#include "Material.h"
#include <ctime>
#include <memory>
#include "MaterialArchView.h"

int main()
{
	srand(time(nullptr));
	std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(sf::VideoMode(1600, 600), "SFML works!");
	Descripteur d("test",std::make_shared<Function>("objet"));

	std::map<Element::REACTION, float> need;
	for(int i = 0; i < int(Element::REACTION::__COUNT); i++){
		need.emplace(Element::REACTION(i), i);
	}
	Console cwin(window);

	std::shared_ptr<MaterialArch> arch = std::make_shared<MaterialArch>();
	arch->generate(9);
	arch->getResultReac();
	MaterialArchView mvTest(arch);

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
			}
			if(event.type == sf::Event::TextEntered){
				if(cwin.isVisible()){
					cwin.setCurrentCmd(event.text.unicode);
				}
			}
			
		}

		cwin.update();
		window->clear();
		
		mvTest.draw(window);
		if(cwin.isVisible()){
			cwin.draw();
		}
		window->display();
	}

	return 0;
}
