#ifndef MATERIALARCHVIEW_H
#define MATERIALARCHVIEW_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <vector>
#include "MaterialArch.h"
#include "MatArchNodeView.h"

struct NodeCounter{
	static int c;
	static void reset();
};

class MaterialArchView
{
private:
	std::shared_ptr<MaterialArch> m_arch;

	std::vector<std::shared_ptr<MatArchNodeView>> g_nodes;
	std::vector<std::shared_ptr<MatArchNode>> m_nodes;
	
	sf::Thread m_updateTh;
	bool threadStatus;

	static void nodeBuild(std::shared_ptr<MatArchNode> cnode, std::vector<std::shared_ptr<MatArchNodeView>>& nodesCollec, std::vector<std::shared_ptr<MatArchNode>>& nodeList);

	static void linkBuild(std::shared_ptr<MatArchNode> cnode,  std::vector<std::shared_ptr<MatArchNodeView>>& nodesCollec, std::vector<std::shared_ptr<MatArchNode>>& nodeList);

	static NodeCounter nc;

public:


	MaterialArchView(std::shared_ptr<MaterialArch> arch);
	virtual ~MaterialArchView();

	void setPosition(sf::Vector2f pos, sf::Vector2f dim = sf::Vector2f(600,600));

	void startUpdateTh();
	void waitUpdateTh();

	void initNodes();
	void update();
	void draw(std::shared_ptr<sf::RenderWindow> win);

	void printLinks();
};

#endif /* MATERIALARCHVIEW_H */
