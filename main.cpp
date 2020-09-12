#include <SFML/Graphics.hpp>
#include "Descripteur.h"
#include "Function.h"
#include "Console.h"
#include "Element.h"
#include "Material.h"
#include <ctime>

int main()
{
	srand(time(nullptr));
	std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "SFML works!");
	Descripteur d("test",std::make_shared<Function>("objet"));

	std::shared_ptr<Function> f1 = std::make_shared<Function>("rond");
	std::shared_ptr<Function> f2 = std::make_shared<Function>("acier");
	std::shared_ptr<Function> f3 = std::make_shared<Function>("carre");
	std::shared_ptr<Function> f4 = std::make_shared<Function>("mou");
	std::shared_ptr<Function> f5 = std::make_shared<Function>("vert");

	
	f2->addAttribute(std::make_shared<Function>("metal"));
	f2->addAttribute(std::make_shared<Function>("dur"));
	f2->addAttribute(std::make_shared<Function>("alliage"));
	f2->getAttributes().back()->addAttribute(std::make_shared<Function>("fer"));
	f2->getAttributes().back()->addAttribute(std::make_shared<Function>("charbon"));

	std::shared_ptr<Descripteur> ch1=std::make_shared<Descripteur>("fils1",f2);
	std::shared_ptr<Descripteur> ch2=std::make_shared<Descripteur>("fils2",f4);

	std::shared_ptr<Descripteur> ch3=std::make_shared<Descripteur>("subfils1",f3);
	std::shared_ptr<Descripteur> ch4=std::make_shared<Descripteur>("subfils2",f5);
	std::shared_ptr<Descripteur> ch5=std::make_shared<Descripteur>("subfils3",f1);
	d.addChild(ch1);
	d.addChild(ch2);
	ch1->addChild(ch3);
	ch2->addChild(ch4);
	ch5->setParent(ch2);
	ch1->addChild(std::make_shared<Descripteur>("subfils4",f4));
	d.print();

	d.getChildrens().at(0)->printFunction();

	Material mat(3);
	Material mat2(8);
	Material mat4(40);
//	mat.print();
	std::cout << "------" <<std::endl;
//	mat2.print();
	std::cout << "0000000000000000000000000000000000000000000" <<std::endl;
	Material mat3 = mat.result(mat2);
//	mat3.print();
	std::map<Element::REACTION, float> need;
	for(int i = 0; i < int(Element::REACTION::__COUNT); i++){
		need.emplace(Element::REACTION(i), i);
	}
	auto ret = mat.transmute(mat4,mat3.getGlobal().getSum()); 
	Console cwin(window);
	
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
		if(cwin.isVisible()){
			cwin.draw();
		}
		window->display();
	}

	return 0;
}
