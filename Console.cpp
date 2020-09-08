#include "Console.h"

std::vector<std::string> Console::m_dictionary = {"D","Desc","Descriptor","F","Func","Function","print","addChild","getChild","removeChild", "rename","var","exit","clear"	};

Console::Console(){

	m_visibility = false;
	m_background = std::make_shared<sf::RectangleShape>(sf::Vector2f(800,200));
	m_background->setFillColor(sf::Color::Black);
	m_background->setOutlineColor(sf::Color::White);
	m_background->setOutlineThickness(5);
	m_font.loadFromFile("Minecraftia-Regular.ttf");
	m_charSize = 12;
}

Console::Console(const std::shared_ptr<sf::RenderWindow>& win)
	:m_parent(win)
{
	m_visibility = false;
	m_background = std::make_shared<sf::RectangleShape>(sf::Vector2f(win->getSize().x,2+win->getSize().y/2));
	m_background->setFillColor(sf::Color::Black);
	m_background->setOutlineColor(sf::Color::White);
	m_background->setOutlineThickness(5);
	m_font.loadFromFile("Minecraftia-Regular.ttf");
	m_charSize = 14;
	m_currstr = ">";
	m_current.setString(m_currstr);
	m_current.setFont(m_font);
	m_current.setFillColor(sf::Color::White);
	m_current.setCharacterSize(m_charSize);
	m_current.setPosition(0,(getHistMaxSize()+1)*m_charSize);	
	m_cmdID = 0;
}
Console::~Console(){

}

void Console::update(){

}

void Console::draw(){
	m_parent->draw(*m_background);
	for(int i = 0; i < m_history.size(); i++){
		m_parent->draw(*m_history.at(i));
	}
	m_parent->draw(m_current);
}

void Console::toggle(){
	m_visibility = !m_visibility;	


}
bool Console::isVisible(){
	return m_visibility;
}

void Console::newCmd(std::string cmd){
	m_cmds.push_back(cmd);
	print(m_cmds.back());	
	m_cmdID = m_cmds.size();
	execute(cmd);//

}
void Console::print(std::string txt){
	m_history.push_back(std::make_shared<sf::Text>());
	m_history.back()->setFont(m_font);
	m_history.back()->setString(txt);
	m_history.back()->setCharacterSize(m_charSize-2);
	m_history.back()->setFillColor(sf::Color::White);
	updateHistory();

}
std::vector<std::string> Console::cmdCut(std::string cmd,std::vector<char> delimits){
	std::vector<std::string> ret;
	std::string temp = "";
	for(int i = 0; i < cmd.size(); i++){
		bool isDelim = false;
		for(int j = 0; j < delimits.size(); j++){
			if(cmd.at(i) == delimits.at(j)){
				isDelim = true;
			}
		}
		if(isDelim){
			ret.push_back(temp);
			temp = "";
		}else{
			temp += cmd.at(i);
		}	
	}
	if(temp != ""){
		ret.push_back(temp);
	}
	return ret;
}
STATE Console::nextState(STATE cState,std::string cWord){
	switch(cState){
		case STATE::ERROR:
			return STATE::ERROR;
		case STATE::INIT:
			if(cWord == "clear"){
				return STATE::CLEAR;
			}
			else if(cWord == "var"){
				return STATE::VAR_LIST;	
			}
			else if(cWord == "exit"){
				return STATE::EXIT;
			}
			else if(cWord == "Desc" || cWord == "D" || cWord == "Descriptor"){
				return STATE::NEW_DESC;
			}
			else if(cWord == "Func" || cWord == "F" || cWord == "Function"){
				return STATE::NEW_FUN;
			}
			else if(inDescriptors(cWord)){
				return STATE::VAR_D;
			}
			else if(inFunctions(cWord)){
				return STATE::VAR_F;
			}
			else{
				return getInitErr(cWord);
			}
			break;
		case STATE::CLEAR:
			//SUCCESS
			break;
		case STATE::EXIT:
			//SUCCESS
			break;
		case STATE::NEW_DESC:
			if(inDescriptors(cWord) || inFunctions(cWord)){
				return getNewDescErr(cWord);
			}
			return STATE::DESC_NAME;
		case STATE::DESC_NAME:
			if(inDescriptors(cWord)){
				return getDescNameErr(cWord);
			}	
			return STATE::DESC_FUN;
		case STATE::DESC_FUN:
			if(cWord != "&"){
				return getDescFunErr(cWord);
			}
			return STATE::INIT;
		case STATE::NEW_FUN:
			if(inDescriptors(cWord)){
				return getNewFunErr(cWord);
			}
			return STATE::FUN_NAME;
		case STATE::FUN_NAME:
			//SUCCESS		
			break;
		case STATE::VAR_D:
			if(cWord == "addChild"){
				return STATE::ADDCHILD;
			}
			else if (cWord == "getChild"){
				return STATE::GETCHILD;
			}
			else if (cWord == "setFunction"){
				return STATE::SETFUN;
			}
			else if(cWord == "print"){
				return STATE::D_PRINT;
			}
			else if(cWord == "removeChild"){
				return STATE::REMOVECHILD;
			}
			else{
				return getVarDErr(cWord);
			}
			break;
		case STATE::ADDCHILD:
			if(cWord == "Desc" || cWord == "D" || cWord == "Descriptor"){
				return STATE::ADDCHILD_NEWD;
			}
			else if(!inDescriptors(cWord)){
				return getAddChildErr(cWord);
			}
			return STATE::ADDCHILD_ARG; 
		case STATE::ADDCHILD_NEWD:
			if(isVariable(cWord)){
				return getAddChildNewDErr(cWord);
			}
			return ADDCHILD_DNAME;
		case STATE::ADDCHILD_DNAME:
			if(inDescriptors(cWord)){
				return getAddChildDNameErr(cWord);
			}
			return STATE::ADDCHILD_FNAME;
		case STATE::ADDCHILD_FNAME:
			if(cWord != "&"){
				return getAddChildFNameErr(cWord);
			}
			return STATE::ADDCHILD;
		case STATE::ADDCHILD_ARG:
			if(cWord != "&"){
				return getAddChildFNameErr(cWord);
			}
			return STATE::ADDCHILD;
		case STATE::GETCHILD:
			if(!inDescriptors(cWord)){
				return getGetChildErr(cWord);
			}
			return STATE::VAR_D;
		case STATE::GETCHILD_ARG:

			break;
		case STATE::SETFUN:
			if(!inFunctions(cWord)){
				return getSetFunErr(cWord);
			}
			return STATE::SETFUN_ARG;
					//SUCCESS
		case STATE::SETFUN_ARG:
			break;
		case STATE::D_PRINT:
					//SUCCESS
			break;	
		case STATE::REMOVECHILD:
			if(!inDescriptors(cWord)){
				return getRemoveChildErr(cWord);
			}
			return STATE::REMOVECHILD_ARG;
		case STATE::REMOVECHILD_ARG:

			break;
		case STATE::VAR_F:
			if(cWord == "print"){
				return STATE::F_PRINT;
			}
			break;
		case STATE::F_PRINT:
			break;
			default:
			break;
	}
}

STATE Console::getInitErr(std::string msg){
	std::cout  << "ERROR : Unknown command : "<< msg << std::endl;
	return STATE::ERROR;
}
STATE Console::getNewDescErr(std::string msg){
	std::cout  << "ERROR : "<<msg<<  " cannot be used for a new D" << std::endl;
	return STATE::ERROR;
}
STATE Console::getDescNameErr(std::string msg){
	std::cout  << "ERROR : "<<msg<< " cannot be used for a new F" << std::endl;
	return STATE::ERROR;
}
STATE Console::getDescFunErr(std::string msg){
	std::cout  << "ERROR : "<<msg<< " is not allowed, try & instead" << std::endl;
	return STATE::ERROR;
}
STATE Console::getNewFunErr(std::string msg){
	std::cout  << "ERROR : "<<msg<< " cannot be used for a new F" << std::endl;
	return STATE::ERROR;
}
STATE Console::getVarDErr(std::string msg){
	std::cout  << "ERROR : "<<msg<< " is not a member of this D" << std::endl;
	return STATE::ERROR;
}
STATE Console::getGetChildErr(std::string msg){
	std::cout  << "ERROR : "<<msg<< " is not a D" << std::endl;
	return STATE::ERROR;
}
STATE Console::getSetFunErr(std::string msg){
	std::cout  << "ERROR : "<<msg<< " is not a F" << std::endl;
	return STATE::ERROR;

}
STATE Console::getRemoveChildErr(std::string msg){
	std::cout  << "ERROR : "<<msg<< " is not a D" << std::endl;
	return STATE::ERROR;

}
STATE Console::getAddChildErr(std::string msg){
	std::cout  << "ERROR : "<<msg<< " is not a D" << std::endl;
	return STATE::ERROR;
}
STATE Console::getAddChildNewDErr(std::string msg){
	std::cout  << "ERROR : "<<msg<<  " cannot be used for a new D" << std::endl;
	return STATE::ERROR;
}
STATE Console::getAddChildDNameErr(std::string msg){
	std::cout  << "ERROR : "<<msg<< " cannot be used for a new F" << std::endl;
	return STATE::ERROR;
}
STATE Console::getAddChildFNameErr(std::string msg){
	std::cout  << "ERROR : "<<msg<< " is not allowed, try & instead" << std::endl;
	return STATE::ERROR;
}


std::string Console::stateToStr(STATE s){
	switch(s){
		case STATE::ERROR:
			return "ERROR";
		case STATE::INIT:
			return "INIT";
		case STATE::CLEAR:
			return "CLEAR";
		case STATE::VAR_LIST:
			return "VAR_LIST";
		case STATE::EXIT:
			return "EXIT";
		case STATE::NEW_DESC:
			return "NEW_DESC";
		case STATE::DESC_NAME:
			return "DESC_NAME";
		case STATE::DESC_FUN:
			return "DESC_FUN";
		case STATE::NEW_FUN:
			return "NEW_FUN";
		case STATE::FUN_NAME:
			return "FUN_NAME";		
		case STATE::VAR_D:
			return "VAR_D";
		case STATE::ADDCHILD:
			return "ADDCHILD";
		case STATE::ADDCHILD_NEWD:
			return "ADDCHILD_NEWD";
		case STATE::ADDCHILD_DNAME:
			return "ADDCHILD_DNAME";
		case STATE::ADDCHILD_FNAME:
			return "ADDCHILD_FNAME";
		case STATE::ADDCHILD_ARG:
			return "ADDCHILD_ARG";
		case STATE::GETCHILD:
			return "GETCHILD";
		case::STATE::GETCHILD_ARG:
			return "GETCHILD_ARG";
		case STATE::SETFUN:
			return "SETFUN";
		case STATE::D_PRINT:
			return "D_PRINT";
		case STATE::REMOVECHILD:
			return "REMOVECHILD";
		default:
			break;
	}
}

void Console::execute(std::string cmd){
	//
	STATE cState = STATE::INIT;
	std::vector<std::string> words = cmdCut(cmd,std::vector<char>({' ','(',')',',','.','\0'}));
	int idWord =0;
	std::string currentVar = "";
	while(idWord != words.size()){
		std::cout << stateToStr(cState) << std::endl;
		cState = nextState(cState,words.at(idWord));
		if(cState == STATE::DESC_FUN){
			newDescriptor(words.at(idWord-1), words.at(idWord));
		}
		if(cState == STATE::VAR_LIST){
			printVar();
		}
		if(cState == STATE::FUN_NAME){
			newFunction(words.at(idWord));
		}
		if(cState == STATE::D_PRINT){
			execPrint(words.at(idWord-1));
		}
		if(cState == STATE::SETFUN_ARG){
			execSetFunc(words.at(idWord-2), words.at(idWord));
		}
		if(cState == STATE::ADDCHILD_ARG){
			execAddChild(currentVar, words.at(idWord));
		}
		if(cState == STATE::ADDCHILD_FNAME){
			newDescriptor(words.at(idWord-1), words.at(idWord));
			execAddChild(currentVar,words.at(idWord-1));
		}
		if(cState == STATE::REMOVECHILD_ARG){
			execRemoveChild(words.at(idWord-2), words.at(idWord));
		}
		if(cState == STATE::VAR_D){
			currentVar = words.at(idWord);
		}
		if(cState == STATE::CLEAR){
			m_history.clear();
		}
		if(cState == STATE::EXIT){
			m_parent->close();
		}
		idWord++;
	} 
	std::cout << stateToStr(cState) << std::endl << std::endl;
}

void Console::printVar(){
	for(std::map<std::string,TYPE>::iterator it = m_variables.begin(); it != m_variables.end(); it++){
		std::string txt = "";
		txt += it->first;
		txt += "\t";
		txt += typeToStr(it->second); 
		print(txt);
	}
}

void Console::execPrint(std::string name){
	std::vector<std::string> tree = m_varDescriptors.at(name)->getState();	
	for(auto& line:tree){
		print(line);
	}
}

void Console::execAddChild(std::string desc, std::string child){
	m_varDescriptors.at(desc)->addChild(m_varDescriptors.at(child));
}

void Console::execRemoveChild(std::string desc, std::string child){
	m_varDescriptors.at(desc)->removeChild(m_varDescriptors.at(child));
}

void Console::execSetFunc(std::string desc, std::string fun){
	if(inDescriptors(desc) && inFunctions(fun)){
		m_varDescriptors.at(desc)->setFunction(m_varFunctions.at(fun));
	}
	else{
		std::cout << "Invalid parameters" << std::endl;
	}
} 

void Console::newDescriptor(std::string name, std::string fun){
	if(!isVariable(name) && !isDescriptor(fun)){
		m_variables.emplace(name , TYPE::DESCRIPTOR);
		m_variables.emplace(fun , TYPE::FUNCTION);
		if(!inFunctions(fun)){
			m_varFunctions.emplace(fun, std::make_shared<Function>(fun));
		}
		m_varDescriptors.emplace(name,std::make_shared<Descripteur>(name,m_varFunctions.at(fun)));
	}else{
		if(isVariable(name)){
			print("Error : Descriptor already exists");
		}
		if(isDescriptor(fun)){
			print("Error : function name is a Descriptor");
		}

	}

}

void Console::newFunction(std::string name){
	if(!isDescriptor(name)){
		m_variables.emplace(name , TYPE::FUNCTION);
		if(!inFunctions(name)){
			m_varFunctions.emplace(name,std::make_shared<Function>(name));
		}
	}
	else{
		print("Error : function name is a Descriptor");
	}

}

std::string Console::typeToStr(TYPE t){
	switch(t){
		case TYPE::DESCRIPTOR:
			return "Descriptor";
			break;
		case TYPE::FUNCTION:
			return "Function";
			break;
		default:
			break;
	}
	return "";
}
void Console::updateHistory(){
	for(int i = m_history.size()-1; i >= std::max(0,int(m_history.size()-1-getHistMaxSize())); i--){
		m_history.at(i)->setPosition(2,(getHistMaxSize()-(m_history.size()-1-i))*m_charSize);
	}
}
int Console::getHistMaxSize(){
	return  (int(m_background->getSize().y/m_charSize)-1);
}
bool Console::inDescriptors(std::string str){
	if(m_varDescriptors.find(str) != m_varDescriptors.end()){
		return true;
	}
	return false;

}
bool Console::inFunctions(std::string str){
	if(m_varFunctions.find(str) != m_varFunctions.end()){
		return true;
	}
	return false;

}

bool Console::isVariable(std::string str){
	if(m_variables.find(str) != m_variables.end()){
		return true;
	}
	return false;
}
bool Console::isFunction(std::string str){
	if(!isVariable(str)){
		return false;
	}
	if(m_variables.at(str) == TYPE::FUNCTION){
		return true;
	}
	return false;
}

bool Console::isDescriptor(std::string str){
	if(!isVariable(str)){
		return false;
	}
	if(m_variables.at(str) == TYPE::DESCRIPTOR){
		return true;
	}
	return false;
}

void Console::setCurrentCmd(char c){
	int id = 0;
	if(c != 127 && c != 8 && c != 13 && c != 9 && c != 24 && c != 25 && c != 27){
		m_currstr += c;
	}
	else if (c == 13){
		newCmd(m_currstr.substr(1,m_currstr.size()));
		m_currstr = (">");
		id = 0;
	}
	else if(c == 9){
		if(m_currstr != ">"){
		m_currstr = autoComplete(m_currstr.substr(1,m_currstr.size()),id);
		id++;	
		}
	}
	else if(c == 24){
	}
	else if(c == 25){

	}
	else if(m_currstr.size() > 1){
		m_currstr.pop_back();
	}
	m_current.setString(m_currstr);
}

std::string Console::autoComplete(std::string line,int id){

	std::vector<std::string> words = cmdCut(line,std::vector<char>({' ','(',')',',','.','\0'}));
	std::string word = words.back(); 
	std::vector<std::string> candidats;
	for(std::map<std::string,TYPE>::iterator it = m_variables.begin(); it != m_variables.end(); it++){
		std::size_t findid = it->first.find(word);
		if(findid != std::string::npos){
			candidats.push_back(it->first);
		}
	}
	for(std::vector<std::string>::iterator it = m_dictionary.begin(); it != m_dictionary.end(); it++){
		std::size_t findid = it->find(word);
		if(findid != std::string::npos){
			candidats.push_back(*it);
		}
	}
	std::string ret = ">";
	for(int i = 0; i < words.size()-1; i++){
		ret += words.at(i);	
		ret += " ";
	}	
	if(candidats.size() > 0){
		ret += candidats.at(id%candidats.size());
	}else{
		ret +=word;
	}
	return ret;
}

void Console::upArrow(){
	if(m_cmdID > 0){
		m_cmdID--;
	}
	m_currstr = ">"+m_cmds.at(m_cmdID);
	m_current.setString(m_currstr);

}
void Console::downArrow(){
	if(m_cmdID < m_cmds.size()){
		m_cmdID++;
	}
	if(m_cmdID == m_cmds.size()){
		m_currstr = ">";
	}else{
		m_currstr = ">"+ m_cmds.at(m_cmdID);
	}
	m_current.setString( m_currstr);
}
