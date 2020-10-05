#include "Console.h"

std::vector<std::string> Console::m_dictionary = {"D","Desc","Descriptor","F","Func","Function","print","addChild","getChild","removeChild", "rename","var","exit","clear"	};

//Constructors

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

//Automata ---------------------------------------------------------

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
			execDPrint(words.at(idWord-1));
		}
		if(cState == STATE::D_SETFUN_ARG){
			execDSetFunc(words.at(idWord-2), words.at(idWord));
		}
		if(cState == STATE::D_ADDCHILD_ARG){
			execDAddChild(currentVar, words.at(idWord));
		}
		if(cState == STATE::D_ADDCHILD_FNAME){
			newDescriptor(words.at(idWord-1), words.at(idWord));
			execDAddChild(currentVar,words.at(idWord-1));
		}
		if(cState == STATE::D_REMOVECHILD_ARG){
			execDRemoveChild(words.at(idWord-2), words.at(idWord));
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
		if(cState == STATE::ELEM_NAME){
			if( idWord +1 == words.size() || !is_num(words.at(idWord+1))){

				//std::cout << words.at(idWord)<< std::endl;
				newElement(words.at(idWord));
			}
		}
		if(cState == STATE::ELEM_RANK){
			newElement(words.at(idWord-1),std::stoi(words.at(idWord)));
		}
		if(cState == STATE::E_PRINT){
			execEPrint(words.at(idWord-1));
			m_varElements.at(words.at(idWord-1))->print();
		}	
		if(cState == STATE::E_RAND){
			m_varElements.at(words.at(idWord-1))->randomize();
		}
		if(cState == STATE::E_EXALT_WEIGHT){
			m_varElements.at(words.at(idWord-3))->exalt(Element::strToReac(words.at(idWord-1)),std::stof(words.at(idWord)));	
		}
		if(cState == STATE::E_INHIB_WEIGHT){
			m_varElements.at(words.at(idWord-3))->inhib(Element::strToReac(words.at(idWord-1)),std::stof(words.at(idWord)));	
		}
		idWord++;
	} 
	std::cout << stateToStr(cState) << std::endl << std::endl;
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
			else if(cWord == "Elem" || cWord == "E" || cWord == "Element"){
				return STATE::NEW_ELEM;
			}
			else if(inDescriptors(cWord)){
				return STATE::VAR_D;
			}
			else if(inFunctions(cWord)){
				return STATE::VAR_F;
			}
			else if(inElements(cWord)){
				return STATE::VAR_E;
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
				return STATE::D_ADDCHILD;
			}
			else if (cWord == "getChild"){
				return STATE::D_GETCHILD;
			}
			else if (cWord == "setFunction"){
				return STATE::D_SETFUN;
			}
			else if(cWord == "print"){
				return STATE::D_PRINT;
			}
			else if(cWord == "removeChild"){
				return STATE::D_REMOVECHILD;
			}
			else{
				return getVarDErr(cWord);
			}
			break;
		case STATE::D_ADDCHILD:
			if(cWord == "Desc" || cWord == "D" || cWord == "Descriptor"){
				return STATE::D_ADDCHILD_NEWD;
			}
			else if(!inDescriptors(cWord)){
				return getAddChildErr(cWord);
			}
			return STATE::D_ADDCHILD_ARG; 
		case STATE::D_ADDCHILD_NEWD:
			if(isVariable(cWord)){
				return getAddChildNewDErr(cWord);
			}
			return D_ADDCHILD_DNAME;
		case STATE::D_ADDCHILD_DNAME:
			if(inDescriptors(cWord)){
				return getAddChildDNameErr(cWord);
			}
			return STATE::D_ADDCHILD_FNAME;
		case STATE::D_ADDCHILD_FNAME:
			if(cWord != "&"){
				return getAddChildFNameErr(cWord);
			}
			return STATE::D_ADDCHILD;
		case STATE::D_ADDCHILD_ARG:
			if(cWord != "&"){
				return getAddChildFNameErr(cWord);
			}
			return STATE::D_ADDCHILD;
		case STATE::D_GETCHILD:
			if(!inDescriptors(cWord)){
				return getGetChildErr(cWord);
			}
			return STATE::VAR_D;
		case STATE::D_GETCHILD_ARG:

			break;
		case STATE::D_SETFUN:
			if(!inFunctions(cWord)){
				return getSetFunErr(cWord);
			}
			return STATE::D_SETFUN_ARG;
					//SUCCESS
		case STATE::D_SETFUN_ARG:
			break;
		case STATE::D_PRINT:
					//SUCCESS
			break;	
		case STATE::D_REMOVECHILD:
			if(!inDescriptors(cWord)){
				return getRemoveChildErr(cWord);
			}
			return STATE::D_REMOVECHILD_ARG;
		case STATE::D_REMOVECHILD_ARG:

			break;
		case STATE::VAR_F:
			if(cWord == "print"){
				return STATE::F_PRINT;
			}
			break;
		case STATE::F_PRINT:
			break;
		case STATE::NEW_ELEM:
			return STATE::ELEM_NAME;
			break;
		case STATE::ELEM_NAME:
			return STATE::ELEM_RANK;
			break;
		case STATE::ELEM_RANK:
			break;
		case STATE::VAR_E:
			if(cWord == "print"){
				return STATE::E_PRINT;
			}
			if(cWord == "randomize" || cWord == "rand"){
				return STATE::E_RAND;
			}
			if(cWord == "exalt"){
				return STATE::E_EXALT;
			}
			if(cWord == "inhib"){
				return STATE::E_INHIB;
			}
			if(cWord == "absorb"){
				return STATE::E_ABSORB;
			}
			break;
		case STATE::E_PRINT:
	
			break;
		case STATE::E_RAND:
			break;
		case STATE::E_EXALT:
			if(Element::strToReac(cWord) == Element::REACTION::__COUNT){
				return STATE::ERROR;
			}
			return STATE::E_EXALT_REAC;
			break;
		case STATE::E_INHIB:
			if(Element::strToReac(cWord) == Element::REACTION::__COUNT){
				return STATE::ERROR;
			}
			return STATE::E_INHIB_REAC;
			break;
		case STATE::E_EXALT_REAC:
			if(!is_num(cWord)){
				return STATE::ERROR;
			}
			return STATE::E_EXALT_WEIGHT;
			break;
		case STATE::E_EXALT_WEIGHT:
			break;
		case STATE::E_INHIB_REAC:
			if(!is_num(cWord)){
				return STATE::ERROR;
			}
			return STATE::E_INHIB_WEIGHT;
			break;
		case STATE::E_INHIB_WEIGHT:
			break;
		case STATE::E_ABSORB:
			if(!inElements(cWord)){
				return STATE::ERROR;
			}
			return STATE::E_ABSORB_ELEM;
			break;
		default:
			break;
	}
}

//Graphical -----------------------------------------------------------

void Console::update(){

}
void Console::draw(){
	m_parent->draw(*m_background);
	for(int i = 0; i < m_history.size(); i++){
		m_parent->draw(*m_history.at(i));
	}
	m_parent->draw(m_current);
}
void Console::print(std::string txt){
	m_history.push_back(std::make_shared<sf::Text>());
	m_history.back()->setFont(m_font);
	m_history.back()->setString(txt);
	m_history.back()->setCharacterSize(m_charSize-2);
	m_history.back()->setFillColor(sf::Color::White);
	updateHistory();

}
void Console::toggle(){
	m_visibility = !m_visibility;	


}
bool Console::isVisible(){
	return m_visibility;
}
void Console::updateHistory(){
	for(int i = m_history.size()-1; i >= std::max(0,int(m_history.size()-1-getHistMaxSize())); i--){
		m_history.at(i)->setPosition(2,(getHistMaxSize()-(m_history.size()-1-i))*m_charSize);
	}
}
int Console::getHistMaxSize(){
	return  (int(m_background->getSize().y/m_charSize)-1);
}

//CMD execution ---------------------------------------------------

void Console::printVar(){
	for(std::map<std::string,TYPE>::iterator it = m_variables.begin(); it != m_variables.end(); it++){
		std::string txt = "";
		txt += it->first;
		txt += "\t";
		txt += typeToStr(it->second); 
		print(txt);
	}
}
void Console::execDPrint(std::string name){
	std::vector<std::string> tree = m_varDescriptors.at(name)->getState();	
	for(auto& line:tree){
		print(line);
	}
}
void Console::execFPrint(std::string name){
	
}

void Console::execEPrint(std::string name){
	std::vector<std::string> table = m_varElements.at(name)->getPrintableState();
	for(auto& line:table){
		print(line);
	}
}
void Console::execDAddChild(std::string desc, std::string child){
	m_varDescriptors.at(desc)->addChild(m_varDescriptors.at(child));
}
void Console::execDRemoveChild(std::string desc, std::string child){
	m_varDescriptors.at(desc)->removeChild(m_varDescriptors.at(child));
}
void Console::execDSetFunc(std::string desc, std::string fun){
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
void Console::newElement(std::string name, int rank){
	if(!isVariable(name)){
		m_variables.emplace(name,TYPE::ELEMENT);
		if(!inElements(name)){
			m_varElements.emplace(name, std::make_shared<Element>(rank));
		}
	}
	else{
		print("Error : Element name already used");
	}
} 

//Enums to Strings ----------------------------------------------------

std::string Console::typeToStr(TYPE t){
	switch(t){
		case TYPE::DESCRIPTOR:
			return "Descriptor";
			break;
		case TYPE::FUNCTION:
			return "Function";
			break;
		case TYPE::ELEMENT:
			return "Element";
		default:
			break;
	}
	return "";
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
		case STATE::D_ADDCHILD:
			return "D_ADDCHILD";
		case STATE::D_ADDCHILD_NEWD:
			return "D_ADDCHILD_NEWD";
		case STATE::D_ADDCHILD_DNAME:
			return "D_ADDCHILD_DNAME";
		case STATE::D_ADDCHILD_FNAME:
			return "D_ADDCHILD_FNAME";
		case STATE::D_ADDCHILD_ARG:
			return "D_ADDCHILD_ARG";
		case STATE::D_GETCHILD:
			return "D_GETCHILD";
		case::STATE::D_GETCHILD_ARG:
			return "D_GETCHILD_ARG";
		case STATE::D_SETFUN:
			return "D_SETFUN";
		case STATE::D_SETFUN_ARG:
			return "D_SETFUN_ARG";
		case STATE::D_PRINT:
			return "D_PRINT";
		case STATE::D_REMOVECHILD:
			return "D_REMOVECHILD";
		case STATE::D_REMOVECHILD_ARG:
			return "D_REMOVECHILD_ARG";
		case STATE::VAR_F:
			return "VAR_F";
		case STATE::F_PRINT:
			return "F_PRINT";
		case STATE::ELEM_NAME:
			return "ELEM_NAME";
		case STATE::ELEM_RANK:
			return "ELEM_RANK";
		case STATE::E_PRINT:
			return "E_PRINT";
		case STATE::E_RAND:
			return "E_RAND";
		case STATE::E_EXALT:
			return "E_EXALT";
		case STATE::E_INHIB:
			return "E_INHIB";
		case STATE::E_INHIB_REAC:
			return "E_INHIB_REAC";
		case STATE::E_INHIB_WEIGHT:
			return "E_INHIB_WEIGHT";
		case STATE::E_ABSORB:
			return "E_ABSORB";
		case STATE::E_ABSORB_ELEM:
			return "E_ABSORB_ELEM";
		case STATE::NEW_ELEM:
			return "NEW_ELEM";
		case STATE::VAR_E:
			return "VAR_E";
		default:
			std::cout<< std::endl << "Unclassified enum : " << std::endl;
			break;
	}
}

//Basics ---------------------------------------------------------

void Console::newCmd(std::string cmd){
	m_cmds.push_back(cmd);
	print(m_cmds.back());	
	m_cmdID = m_cmds.size();
	execute(cmd);//

}
bool Console::is_num(const std::string& str){
    return !str.empty() && std::find_if(str.begin(), 
        str.end(), [](unsigned char c) { return !(std::isdigit(c) || c == '.' || c == ','); }) == str.end();
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

//CMD utilities ----------------------------------------------------------------------

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

//Var existence check -------------------------------------------------

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
bool Console::isElement(std::string str){
	if(!isVariable(str)){
		return false;
	}
	if(m_variables.at(str) == TYPE::ELEMENT){
		return true;
	}
	return false;
}
bool Console::inElements(std::string str){
	if(m_varElements.find(str) != m_varElements.end()){
		return true;
	}
	return false;
}

//Automata detailed errors -----------------------------------------

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
