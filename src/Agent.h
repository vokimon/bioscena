// Agent.h: interface for the CAgent class.
//
//////////////////////////////////////////////////////////////////////
// El destructor de las subclasses debera destruir los agentes subordinados 
// pero no los agentes de los que depende
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


#if !defined(__KKEP_AGENT_H_INCLUDED)
#define __KKEP_AGENT_H_INCLUDED

#include <string>
#include <list>
#include <map>
#include <strstream>
#include <iomanip>
#include "BioIncludes.h"
#include "Missatger.h"
#include "Substrat.h"
#include "Biotop.h"
#include "Topologia.h"

using namespace std;

class CAgent 
{
// Tipus Propis
	typedef map<string, CAgent*> t_diccionariAgents;
// Algunes amigues
	friend ostream& operator<< (ostream& str, CAgent& ag);
// Construccio/Destruccio
public:
	CAgent();
	virtual ~CAgent();
// Redefinibles
public:
	virtual void operator() (void)=0;
	virtual void dump(CMissatger & msg);
	virtual bool configura(string parametre, istream & nom, t_diccionariAgents & diccionari, CMissatger & errors);
	virtual list<CAgent*> subordinats() {list<CAgent*> l; return l;};
	virtual list<CAgent*> dependencies() {list<CAgent*> l; return l;};
// Operacions
public:
	string nom(){return m_nom;};
	bool nom(string unNom);
	string tipus() {return m_tipus;};
	void dumpAll(CMissatger& msg);
	void dumpTipus(CMissatger& msg);
	void dumpConfiguracions(CMissatger& msg);
	bool subordinant(CAgent *ag);
	CAgent* subordinant(void);
// Atributs
public:
	string m_tipus;
	bool m_nomDefinit;
	uint32 m_numeroAgent;
private:
	string m_nom; // Privat perque l'acces es complex i cal fer-ho amb els accessors
	CAgent * m_subordinant;
// Dades estatiques
public:
	static uint32 s_ultimNumeroAgent;
	static t_diccionariAgents s_DiccionariAgents;
// Funcions estatiques
public:
	static void DumpDiccionari(CMissatger& msg);
	static CAgent * CercaDiccionari(string s);
	static CAgent * ParsejaArxiu(char * nomArxiu, CBiotop<CSubstrat> & biotop, CMissatger & errors);
private:
	static CAgent * CreaAgent(string tipus, CBiotop<CSubstrat> & biotop);
// Proves
public:
	static void ProvaClasse();
};

#endif // !defined(__KKEP_AGENT_H_INCLUDED)
