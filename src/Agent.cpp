// Agent.cpp: implementation of the CAgent class.
//
//////////////////////////////////////////////////////////////////////
// Descripcio:
// Aquest modul implementa la classe abstracta factoria per a tots els 
// agents externs que es poden configurar perque afectin a un biòtop.
// Com a classe abstracta factoria, implementa el nucli dels 
// mecanismes de pasivacio i activacio comuns pels agents.
//////////////////////////////////////////////////////////////////////
// Notes d'implementacio:
// El destructor de las subclasses debera destruir los agentes 
// subordinados pero no los agentes de los que depende
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990820 VoK - Modificacions per tipus d'agent jerarquic
// 19990821 VoK - Funcionalitats de dump
// 19990821 VoK - Afegit diccionari de Agents
// 20000220 VoK - Adaptat a la nova forma de fer topologies
//////////////////////////////////////////////////////////////////////
// TODO:
// - Permetre coneixer el pare
// - Que passa quan ja hi ha un nom amb el meu nom per defecte!!??
//////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <fstream>
#include "Agent.h"
#include "Itineraris.h"
#include "Actuadors.h"
#include "MultiAgent.h"
#include "Temporitzador.h"
#include "Aleaturitzador.h"
#include "Iterador.h"
#include "Color.h"
#include "TopologiaToroidal.h"
#include "Biosistema.h"

using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Construccio/Destruccio
//////////////////////////////////////////////////////////////////////

CAgent::CAgent()
{
	m_tipus="Agent";
	m_numeroAgent=s_ultimNumeroAgent++;
	m_nomDefinit=false;
	ostrstream stream;
	stream 
		<< tipus() << " " 
		<< setfill('0') << setw(4) << m_numeroAgent 
		<< setfill(' ') << ends;
	nom(stream.str());
	m_subordinant=NULL;
};

CAgent::~CAgent() 
{
	if (m_nomDefinit) s_DiccionariAgents.erase(m_nom); 
};

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

void CAgent::dump(CMissatger& msg)
{
	msg << endl
		<< "+ " << nom() << endl
//		<< "- Tipus: " << tipus() << endl
		;
}

bool CAgent::configura(string parametre, istream & nom, t_diccionariAgents & diccionari, CMissatger & errors)
{
	if (parametre=="Com") {
		// Ignorem el contingut
		// TODO: Guardar el contingut
		return true;
	}
	// CAgent no te cap parametre configurar
	// El retorn diu si l'agent ha interceptat el parametre
	return false;
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

bool CAgent::nom(string unNom)
{
	// Primer em trec del diccionari amb el nom antic
	if (m_nomDefinit) s_DiccionariAgents.erase(m_nom); 
	replace(unNom.begin(), unNom.end(), ' ', '_');
//	replace(unNom.begin(), unNom.end(), '\"', '\'');
	// Si ja existia no fem pair
	t_diccionariAgents::value_type unParell(unNom,this);
	if (!(s_DiccionariAgents.insert(unParell)).second) 
		return false;
	m_nomDefinit=true;
	m_nom=unNom;
	return true;
}

bool CAgent::subordinant(CAgent * ag)
{
	if (m_subordinant&&ag) // Ep! Que ja en tinc pare!
		return false;
	m_subordinant=ag;
	return true;
}

CAgent * CAgent::subordinant(void)
{
	return m_subordinant;
}

void CAgent::dumpAll(CMissatger& msg) 
{
	dumpTipus(msg);
	dumpConfiguracions(msg);
}

void CAgent::dumpConfiguracions(CMissatger& msg)
{
	list<CAgent*> l=subordinats();
	for (list<CAgent*>::iterator it=l.begin(); it!=l.end(); it++)
		(*it)->dumpConfiguracions(msg);
	dump(msg);
}

void CAgent::dumpTipus(CMissatger& msg)
{
	msg << "* " << nom() << " " << tipus() << endl;
	list<CAgent*> l=subordinats();
	for (list<CAgent*>::iterator it=l.begin(); it!=l.end(); it++)
		(*it)->dumpTipus(msg);
}

//////////////////////////////////////////////////////////////////////
// Dades estatiques
//////////////////////////////////////////////////////////////////////

uint32 CAgent::s_ultimNumeroAgent=0;
map<string, CAgent*> CAgent::s_DiccionariAgents;

//////////////////////////////////////////////////////////////////////
// Funcions estatiques
//////////////////////////////////////////////////////////////////////

void CAgent::DumpDiccionari(CMissatger& msg) 
{
//	msg << groc.brillant() << "Diccionari d'agents" << blanc.fosc() << endl;
	map<string, CAgent*>::iterator it;
	for (it=s_DiccionariAgents.begin(); it!=s_DiccionariAgents.end(); it++)
		msg << "* " << it->second->nom() << " " << it->second->tipus() << endl;
}

CAgent* CAgent::CercaDiccionari(string s) 
{
	t_diccionariAgents::iterator it;
	it=s_DiccionariAgents.find(s); 
	if (it==s_DiccionariAgents.end())
		return NULL;
	return it->second;
}

CAgent * CAgent::ParsejaArxiu(char * nomArxiu, CBiosistema & biosistema, CMissatger & errors)
{
	// Com que els noms del fitxer poden estar agafats, fem servir
	// el nostre propi diccionari per manegar els noms de l'arxiu
	t_diccionariAgents diccionari;
	// Obrim l'arxiu
	ifstream entrada(nomArxiu);
	string nom, tipus, parametre, valor;
	string prefetch;
	CAgent * agentArrel = NULL;

	out << "Carregant la configuracio dels agents externs [" << nomArxiu << "]" << endl;
	if (!entrada) {
		errors 
			<< "Error obrint '" << nomArxiu << endl;
		return NULL;
	}

	entrada >> prefetch;
	while (prefetch=="*") {	
		entrada >> nom >> tipus >> prefetch;
		// TODO: Esborrar aquesta traca (o no)
//		out << '\t' << nom << '\t' << tipus << endl;
		CAgent * ag = CreaAgent(tipus, biosistema);
		if (!agentArrel) agentArrel= ag;
		if (!ag) {
			errors 
				<< "No esta soportat importar agents de tipus '" << tipus 
				<< "' com l'especificat per a l'agent '" << nom 
				<< "' o no hi ha memoria suficient." << endl;
			continue;
		}
		if (diccionari.find(nom)!=diccionari.end()) {
			errors 
				<< "El nom de l'agent '" << nom << "' esta duplicat a l'arxiu. " 
				<< "Renombrant-ho com a '" << ag->nom() << "'." 
				<< "Les possibles referencies es perdran." << endl;
			continue;
		}
		diccionari[nom]=ag;
		if (!ag->nom(nom)) {
			errors 
				<< "El nom de l'agent '" << nom << "' ja existeix al sistema. " 
				<< "Renombrant-ho com a '" << ag->nom() << "'." << endl;
			continue;
		}
	}
	while (entrada && prefetch=="+") {
		entrada >> nom;
		entrada >> prefetch;
		t_diccionariAgents::iterator it = diccionari.find(nom);
		if (it==diccionari.end()) {
			errors 
				<< "Configurant l'agent '" << nom 
				<< "' sense haver especificat el seu tipus abans. " << endl;
			do entrada >> prefetch;
			while (entrada && prefetch!="+");
			continue;
		}
		while (entrada && prefetch=="-") {
			string parametre, valor;
			entrada >> parametre;
			getline(entrada, valor);
			entrada >> prefetch;
			strstream stream;
			stream << valor;
			if (!((*it).second->configura(parametre, stream, diccionari, errors))) {
				errors 
					<< "L'agent '" << nom 
					<< "' del tipus '" << (*it).second->tipus() 
					<< "' no respon al parametre de configuracio '" << parametre 
					<< "'"<< endl;
			}
		}
	}
	if (entrada) {
		errors << "No s'esperava '" << prefetch << "'"<< endl;
	}

	// TODO: Comprovacions de que es tracta d'una estructura arborea correcta

	out << "Configuracio dels agents externs [" << nomArxiu << "] carregada" << endl;

	return agentArrel;
}

CAgent * CAgent::CreaAgent(string tipus, CBiosistema & biosistema)
{
	if (tipus=="Agent/Multiple") return new CMultiAgent;
	if (tipus=="Agent/Multiple/Temporitzador") return new CTemporitzador;
	if (tipus=="Agent/Multiple/Iterador") return new CIterador;
	if (tipus=="Agent/Multiple/Aleaturitzador") return new CAleaturitzador;
	if (tipus=="Agent/Posicionador") return new CPosicionador(*(biosistema.biotop()->topologia()));
	if (tipus=="Agent/Posicionador/Aleatori") return new CPosicionadorAleatori(*(biosistema.biotop()->topologia()));
	if (tipus=="Agent/Posicionador/Zonal") return new CPosicionadorZonal(*(biosistema.biotop()->topologia()));
	if (tipus=="Agent/Posicionador/Direccional") return new CItinerari(*(biosistema.biotop()->topologia()));
	if (tipus=="Agent/Direccionador") return new CDireccionador(*(biosistema.biotop()->topologia()));
	if (tipus=="Agent/Direccionador/Aleatori") return new CDireccionadorAleatori(*(biosistema.biotop()->topologia()));
	if (tipus=="Agent/Actuador/Nutridor") return new CNutridor(*(biosistema.biotop()));
	if (tipus=="Agent/Actuador/Nutridor/Invers") return new CDesnutridor(*(biosistema.biotop()));
	if (tipus=="Agent/Actuador/Inoculador") return new CInoculador(biosistema);

	return NULL;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CAgent::ProvaClasse()
{
	out << clrscr;
	out << blanc.brillant() << "Provant Agent Parsing" << blanc.fosc() << endl;
	CBiosistema biosistema;
	CTopologia * topo = new CTopologiaToroidal(70,21);
	biosistema.biotop(new CBiotop<CBiosistema::t_substrat>(topo));
	CAgent * agentArrel = ParsejaArxiu("AgentsLog.txt", biosistema, error);
	biosistema.comunitat(new CComunitat);
//	biosistema.taxonomista(new CTaxonomista);
	cin.get(); 
	if (agentArrel) {
		ofstream file("AgentsLog2.txt");
		CColorOutputer miout(file);
		CMissatger msg(NULL, NULL, miout);
		agentArrel->dumpAll(msg);
		agentArrel->dumpAll(out);
	}
	cin.get(); 
}

