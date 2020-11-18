#ifndef MATERIALPOP_H
#define MATERIALPOP_H

#include "MaterialArch.h"
#include "MaterialArchView.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>

class Console;

class MaterialPop : public std::enable_shared_from_this<MaterialPop>
{
private:

	std::map<std::string, std::shared_ptr<MaterialArch>> m_varMaterials;
	std::map<std::string, std::shared_ptr<MaterialArch>> m_orphBuffer;
	std::map<std::string, std::shared_ptr<MaterialArchView>> m_views;
	std::map<std::string, bool> m_visibility;

	std::vector<std::string> toDelete;
	std::shared_ptr<sf::Clock> m_clock;
	bool updateSwitch;
	Console* m_console;
	static int orphCount;

public:
	static std::vector<MaterialPop*> family;
	static std::map<std::shared_ptr<MatArchNode>,std::shared_ptr<MatArchNodeView>> deletedNodes;
	//MaterialPop(std::shared_ptr<Console> console);
	MaterialPop(Console* console);
	virtual ~MaterialPop();

	void add(std::string name, std::shared_ptr<MaterialArch> mat);
	void update();
	void draw(std::shared_ptr<sf::RenderWindow> win);
	std::shared_ptr<MaterialArch> get(std::string name);
	std::shared_ptr<MaterialArchView> getView(std::string name);
	void getDeletedViews(std::vector<std::string> toDelete);
	bool find(std::string name);
	void show(std::string name);
	void hide(std::string name);
	void hideall();

	std::map<REAC, std::vector<std::shared_ptr<MatArchNode>>> getFreeLinks();
	std::map<std::string,std::shared_ptr<MatArchNode>> getAllRoots();
	std::map<REAC,std::vector<std::shared_ptr<MatArchNode>>> getAllLinks();
	void breakLinks();
	void relink();
	void cleanPop();
	void deletion();
	std::pair<int,int> getCount(std::string name);

	std::string orphName();

};

#endif /* MATERIALPOP_H */
