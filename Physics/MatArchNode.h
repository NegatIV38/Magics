#ifndef MATARCHNODE_H
#define MATARCHNODE_H

#include <map>
#include <memory>
#include "Element.h"	

class MatArchNode : public std::enable_shared_from_this<MatArchNode>
{
private:
	std::shared_ptr<Element> m_element;
	std::map<Element::REACTION, std::shared_ptr<MatArchNode>> linkedNodes;


public:
	MatArchNode(std::shared_ptr<Element> elem);
	MatArchNode();	
	virtual ~MatArchNode();
	
	void update();

	void link(Element::REACTION r, std::shared_ptr<MatArchNode> node);
	void unlink(Element::REACTION r);
	std::map<Element::REACTION, float> getResultReac();
	float getElementRawValue(Element::REACTION r);
	std::shared_ptr<MatArchNode> getNode(Element::REACTION r);
	
	std::map<Element::REACTION, std::shared_ptr<MatArchNode>> getNextNodes();
	void generate();
	bool orpheline(std::shared_ptr<MatArchNode> parent);
};

#endif /* MATARCHNODE_H */
