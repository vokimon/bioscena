// ParserAgents.cpp: implementation of the CParserAgents class.
//
//////////////////////////////////////////////////////////////////////

#include <fstream>
#include "ParserAgents.h"
#include "TopologiaToroidal.h"
#include "Itineraris.h"
#include "Actuadors.h"
#include "MultiAgent.h"
#include "Temporitzador.h"
#include "Aleaturitzador.h"
#include "Iterador.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParserAgents::CParserAgents()
{

}

CParserAgents::~CParserAgents()
{

}

CAgent * CParserAgents::ParsejaArxiuAgents(char * nomArxiu, CTopologia<CSubstrat> & biotop, list<string> & errors)
{
	// Com que els noms del fitxer poden estar agafats, fem servir
	// el nostre propi diccionari per manegar els noms de l'arxiu
	map<string, CAgent*> diccionari;
	// Obrim l'arxiu
	ifstream entrada(nomArxiu);
	string prefetch, nom, tipus;
	entrada >> prefetch;
	while (prefetch=="*") {	
		entrada >> nom >> tipus >> prefetch;
		out << nom << " " << tipus << endl;
		CAgent * ag = CreaAgent(tipus, biotop);
		if (!ag) {
			strstream stream;
			stream 
				<< "El tipus " << tipus << " especificat per a l'agent '" 
				<< nom << "' no esta soportat o no hi ha memoria suficient." << ends;
			errors.push_back(string(stream.str()));
			continue;
		}
		if (diccionari.find(nom)!=diccionari.end()) {
			strstream stream;
			stream 
				<< "El nom de l'agent '" << nom << "' esta duplicat a l'arxiu. " 
				<< "Renombrant-ho com a '" << ag->nom() << "'." 
				<< "Les possibles referencies es perdran." << ends;
			errors.push_back(string(stream.str()));
			continue;
		}
		diccionari[nom]=ag;
		if (!ag->nom(nom)) {
			strstream stream;
			stream 
				<< "El nom de l'agent '" << nom << "' ja existeix al sistema. " 
				<< "Renombrant-ho com a '" << ag->nom() << "'." << ends;
			errors.push_back(string(stream.str()));
			continue;
		}
	}
	while (prefetch=="+") {
		entrada >> nom;
		entrada >> prefetch;
		map<string, CAgent*>::iterator it = diccionari.find(nom);
		if (it==diccionari.end()) {
			strstream stream;
			stream 
				<< "Configurant l'agent '" << nom 
				<< "' sense haver especificat el seu tipus abans. " << ends;
			errors.push_back(string(stream.str()));
			continue;
		}
	

	}

	return NULL;
}

void CParserAgents::ProvaClasse()
{
	list<string> errors;
	CTopologiaToroidal<CSubstrat> biotop;
	CAgent * agentArrel = ParsejaArxiuAgents("AgentsLog.txt", biotop, errors);
	list<string>::iterator it;
	for (it=errors.begin(); it!=errors.end(); it++)
		out << (*it) << endl;
//	out << "Prem return per veure el resultat" << endl;
//	cin.get(); 
}

CAgent * CParserAgents::CreaAgent(string tipus, CTopologia<CSubstrat> &biotop)
{
	if (tipus=="Agent/Multiple") return new CMultiAgent;
	if (tipus=="Agent/Multiple/Temporitzador") return new CTemporitzador;
	if (tipus=="Agent/Multiple/Iterador") return new CIterador;
	if (tipus=="Agent/Multiple/Aleaturitzador") return new CAleaturitzador;
	if (tipus=="Agent/Posicionador") return new CPosicionador(biotop);
	if (tipus=="Agent/Posicionador/Aleatori") return new CPosicionadorAleatori(biotop);
	if (tipus=="Agent/Posicionador/Zonal") return new CPosicionadorZonal(biotop);
	if (tipus=="Agent/Posicionador/Direccional") return new CItinerari(biotop);
	if (tipus=="Agent/Direccionador") return new CDireccionador(biotop);
	if (tipus=="Agent/Direccionador/Aleatori") return new CDireccionadorAleatori(biotop);
	if (tipus=="Agent/Actuador/Nutridor") return new CNutridor;
	if (tipus=="Agent/Actuador/Nutridor/Invers") return new CDesnutridor;

	return NULL;
}
