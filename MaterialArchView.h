#ifndef MATERIALARCHVIEW_H
#define MATERIALARCHVIEW_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <vector>
#include "MaterialArch.h"
#include "MatArchNodeView.h"

class MaterialArchView
{
private:
	std::shared_ptr<MaterialArch> m_arch;

	std::vector<std::shared_ptr<MatArchNodeView>> g_nodes;
	std::vector<std::shared_ptr<MatArchNode>> m_nodes;
	

	static void nodeBuild(std::shared_ptr<MatArchNode> cnode, std::vector<std::shared_ptr<MatArchNodeView>>& nodesCollec, std::vector<std::shared_ptr<MatArchNode>>& nodeList);

	static void linkBuild(std::shared_ptr<MatArchNode> cnode,  std::vector<std::shared_ptr<MatArchNodeView>>& nodesCollec, std::vector<std::shared_ptr<MatArchNode>>& nodeList);



	static void getLinks(std::shared_ptr<MatArchNode> cnode, std::vector<std::vector<int>>& linksCollec);
public:


	MaterialArchView(std::shared_ptr<MaterialArch> arch);
	virtual ~MaterialArchView();

	void initNodes();
	void drawLinks(std::shared_ptr<sf::RenderWindow> win);
	void update();
	void draw(std::shared_ptr<sf::RenderWindow> win);
};

#endif /* MATERIALARCHVIEW_H */
