#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <iostream>
#include <memory>
#include <vector>
/*
enum CATEGORY{
	
	MATIERE,VEGETAL, ANIMAL, MINERAL, METAL, NON_METAL,
	COULEUR, ROUGE,VERT,JAUNE,VIOLET,BLEU,ORANGE, //EACH*n
	FORME, GEOMETRIE, COMPOSITION, SOUVENIR, VOLONTER,
	//UTILE
	DIRECTION, DUPLICATION, FRACTALISATION, CREUX,CONTROLABLE,VITESSE,POIDS,FUSION,
	//ETAT
	TEMPERATURE,LIQUIDE,GAZEUX,SOLIDITE,USURE,AGITER,VIVANT
};

enum ELEMENT{
	MINERAL, 
	METAL, 
	NON_METAL

	GAZ, //inoffensif, stable, pot. instable
		PAUVRE, 
			PLURIATOMIQUES, MONOATOMIQUES
		RICHE, 
			ODORANTS, VISIBLES
		REACTIF 
			INFLAMMABLES, EXPLOSIFS, ELEC_STATIC, LIQUEFACTION
	METAL, // mineral, metal, pot. instable
		PAUVRE, 
			TERRES, NON_CONDUCTEURS
		RICHE, 
			CONDUCTEURS, INOXYDABLES
		REACTIF 
			ELEC, RADIOACTIF, TOXIC
	NON-METAL, //mineral,organique,pot. instable
		PAUVRE, 
			CRISTAUX, DESORDONNÉS, ORDONNÉS
		RICHE, 
			CELLULES, VEGETAL, ANIMAL
		REACTIF 
			INFLAMMABLES, GELE, CORROSIF, ISOLANT

	REACTIONS, SOUFFLE, INFLAMMABLES, GELE, CORROSIF, ISOLANT, ELEC, RADIOACTIF, TOXIC, EXPLOSIFS, ELEC_STATIC, LIQUEFACTION
	ACTIVATION, 
	RESISTANCE

}

*/

class Function{

	public:
		Function();
		Function(std::string name);
		~Function();

		bool isLeaf();
		void addAttribute(const std::shared_ptr<Function>& attr);
		void print(int indent=0);
		std::string getName();
		std::vector<std::shared_ptr<Function>> getAttributes();
		void rename(std::string name);
		
	private:
		std::string m_name;
		std::vector<std::shared_ptr<Function>> m_attributes;
};

#endif
