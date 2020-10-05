#ifndef MATARCHNODE_H
#define MATARCHNODE_H

#include <map>
#include <memory>
#include "Element.h"	

class MatArchNode
{
private:
	Element m_element;
	std::map<Element::REACTION, std::shared_ptr<MatArchNode>> linkedNodes;


public:
	MatArchNode();
	virtual ~MatArchNode();
	
	void link(Element::REACTION r, std::shared_ptr<MatArchNode> node);
	void unlink(Element::REACTION r);
};

#endif /* MATARCHNODE_H */
