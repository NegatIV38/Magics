#ifndef MATARCHNODE_H
#define MATARCHNODE_H

#include <map>
#include <memory>
#include "Element.h"	

class MatArchNode : public std::enable_shared_from_this<MatArchNode>
{
private:
	std::shared_ptr<Element> m_element;
	std::map<REAC, std::shared_ptr<MatArchNode>> linkedNodes;


public:
	MatArchNode(std::shared_ptr<Element> elem);
	MatArchNode();	
	virtual ~MatArchNode();
	
	void update();

	void link(REAC r, std::shared_ptr<MatArchNode> node);
	void unlink(REAC r);
	std::map<REAC, float> getResultReac();
	float getElementRawValue(REAC r);
	std::shared_ptr<MatArchNode> getNode(REAC r);

	float getLinkValue(REAC r);
	
	std::map<REAC, std::shared_ptr<MatArchNode>> getNextNodes();
	void generate();
	bool orpheline(std::shared_ptr<MatArchNode> parent);
	bool orpheline();
};

#endif /* MATARCHNODE_H */
