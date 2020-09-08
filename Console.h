#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <SFML/Graphics.hpp>
#include <memory>
#include "Descripteur.h"
#include "Function.h"


enum TYPE{
	DESCRIPTOR, FUNCTION
};

enum STATE{
	ERROR,
	INIT,CLEAR, VAR_LIST, EXIT, 
	NEW_DESC, DESC_NAME, DESC_FUN,
	NEW_FUN, FUN_NAME,
	VAR_D, 
	ADDCHILD,ADDCHILD_NEWD,ADDCHILD_DNAME,ADDCHILD_FNAME, ADDCHILD_ARG, 
	GETCHILD, GETCHILD_ARG, D_PRINT, SETFUN,SETFUN_ARG, REMOVECHILD, REMOVECHILD_ARG,
	VAR_F,
	F_PRINT
};

class Console{
	
	public:
		Console();
		Console(const std::shared_ptr<sf::RenderWindow>& win);
		~Console();

		static std::vector<std::string> m_dictionary;

		void update();
		void draw();
		void toggle();
		bool isVisible();
		void newCmd(std::string cmd);
		void print(std::string txt);
		void setCurrentCmd(char c);
		void execute(std::string cmd);
		void upArrow();
		void downArrow();

	private:

		std::string autoComplete(std::string line, int id);
		void updateHistory();	
		int getHistMaxSize();
		std::vector<std::string> cmdCut(std::string cmd,std::vector<char> delimits);
		STATE nextState(STATE cState,std::string cWord);
		void printVar();
		std::string typeToStr(TYPE t);
		std::string stateToStr(STATE s);

		void newDescriptor(std::string name, std::string fun);
		void newFunction(std::string name);
		void execPrint(std::string name);
		void execSetFunc(std::string desc, std::string fun);
		void execAddChild(std::string desc, std::string child);
		void execRemoveChild(std::string desc, std::string child);
		
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
		//------------------------------------
		bool m_visibility;
		std::shared_ptr<sf::RectangleShape> m_background;
		std::shared_ptr<sf::RenderWindow> m_parent;
		std::vector<std::string> m_cmds;
		std::vector<std::shared_ptr<sf::Text>> m_history;
		std::string m_currstr;
		sf::Text m_current;
		sf::Font m_font;
		int m_charSize;
		int m_cmdID;
		std::map<std::string,TYPE> m_variables;
		std::map<std::string, std::shared_ptr<Descripteur>> m_varDescriptors;
		std::map<std::string, std::shared_ptr<Function>> m_varFunctions;
}; 

#endif
