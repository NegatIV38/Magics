#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <memory>
#include <string>
#include "../Magics/Descripteur.h"
#include "../Magics/Function.h"
#include "../Physics/Element.h"
#include "GraphManager.h"
#include "../Physics/MaterialPop.h"


enum TYPE{
	DESCRIPTOR, FUNCTION, ELEMENT, MATERIAL
};

enum STATE{
	ERROR,
	INIT,CLEAR, VAR_LIST, EXIT, COLORS,HIDE_ALL,PAUSE,RESUME, 
	NEW_DESC, DESC_NAME, DESC_FUN,
	NEW_FUN, FUN_NAME,
	VAR_D, 
	D_ADDCHILD,D_ADDCHILD_NEWD,D_ADDCHILD_DNAME,D_ADDCHILD_FNAME, D_ADDCHILD_ARG, 
	D_GETCHILD, D_GETCHILD_ARG, D_PRINT, D_SETFUN,D_SETFUN_ARG, D_REMOVECHILD, D_REMOVECHILD_ARG,
	VAR_F,
	F_PRINT,

	NEW_ELEM, ELEM_NAME, ELEM_RANK,
	VAR_E, E_PRINT, E_RAND, 
	E_EXALT, E_EXALT_REAC, E_EXALT_WEIGHT,
	E_INHIB, E_INHIB_REAC, E_INHIB_WEIGHT,
	E_ABSORB, E_ABSORB_ELEM,

	NEW_MAT, MAT_NAME, MAT_RANK,
	VAR_M, M_SHOW, M_HIDE, M_PRINT,
	M_PLUS, M_PLUS_ARG, M_PLUS_EQUAL, M_PLUS_NEW_MAT


};

class Console: public std::enable_shared_from_this<Console>
{
	
	public:
		Console(const std::shared_ptr<sf::RenderWindow>& win, std::shared_ptr<GraphManager> gmgr);
		~Console();

		static std::vector<std::string> m_dictionary;

		void update();
		void draw();
		void toggle();
		bool isVisible();
		void newCmd(std::string cmd);
		void print(std::string txt,sf::Color col = sf::Color::White);
		void setCurrentCmd(char c);
		void execute(std::string cmd);
		void upArrow();
		void downArrow();
		void pageUp();
		void pageDown();
		void togglePause();
		bool getPaused();
		void addMatName(std::string name);	
	private:

		std::string autoComplete(std::string line, int id);
		bool is_num(const std::string& str);	
		void updateHistory();	
		int getHistMaxSize();
		std::vector<std::string> cmdCut(std::string cmd,std::vector<char> delimits);
		STATE nextState(STATE cState,std::string cWord);
		void printVar();
		void printColors();
		std::string typeToStr(TYPE t);
		std::string stateToStr(STATE s);

		void newDescriptor(std::string name, std::string fun);
		void newFunction(std::string name);
		void newElement(std::string name, int rank = 1);		
		void newMaterial(std::string name, int rank);

		void execDPrint(std::string name);
		void execDSetFunc(std::string desc, std::string fun);
		void execDAddChild(std::string desc, std::string child);
		void execDRemoveChild(std::string desc, std::string child);

		void execFPrint(std::string name);
		void execEPrint(std::string name);

		void execMShow(std::string name);
		void execMHide(std::string name);
		void execCombine(std::string a, std::string b, std::string c);
		void execMPrint(std::string name);
		
		STATE getInitErr(std::string msg);
		STATE getNewDescErr(std::string msg);
		STATE getDescNameErr(std::string msg);
		STATE getDescFunErr(std::string msg);
		STATE getNewFunErr(std::string msg);
		STATE getVarDErr(std::string msg);
		STATE getGetChildErr(std::string msg);
		STATE getSetFunErr(std::string msg);
		STATE getRemoveChildErr(std::string msg);
		STATE getAddChildErr(std::string msg);
		STATE getAddChildNewDErr(std::string msg);
		STATE getAddChildDNameErr(std::string msg);
		STATE getAddChildFNameErr(std::string msg);

		bool isVariable(std::string str);
		bool isDescriptor(std::string str);
		bool isFunction(std::string str);
		bool inFunctions(std::string str);
		bool inDescriptors(std::string str);

		bool isElement(std::string str);
		bool inElements(std::string str);

		bool isMaterial(std::string str);
		bool inMaterial(std::string str);
		
		void initMatPop();
		
		//------------------------------------
		bool m_visibility;
		bool m_pause;
		std::shared_ptr<sf::RectangleShape> m_background;
		std::shared_ptr<sf::RenderWindow> m_parent;
		std::vector<std::string> m_cmds;
		std::vector<std::shared_ptr<sf::Text>> m_history;
		std::string m_currstr;
		sf::Text m_current;
		sf::Font m_font;
		int m_charSize;
		int m_cmdID;
		int histoShift;
		int pageShift;
		std::map<std::string,TYPE> m_variables;
		std::map<std::string, std::shared_ptr<Descripteur>> m_varDescriptors;
		std::map<std::string, std::shared_ptr<Function>> m_varFunctions;
		std::map<std::string, std::shared_ptr<Element>> m_varElements;
	//	std::map<std::string, std::shared_ptr<MaterialArch>> m_varMaterials;
		std::shared_ptr<MaterialPop> m_matPop;
		std::shared_ptr<GraphManager> m_gMgr;
}; 

#endif
