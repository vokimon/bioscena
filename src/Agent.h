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
//////////////////////////////////////////////////////////////////////
// TODO:
// - Permetre coneixer el pare
// - Que passa quan ja hi ha un nom amb el meu nom per defecte?


#if !defined(__KKEP_AGENT_H_INCLUDED)
#define __KKEP_AGENT_H_INCLUDED

#include <string>
#include <list>
#include <map>
#include <strstream>
#include <iomanip>
#include "BioIncludes.h"
#include "Missatger.h"

using namespace std;

class CAgent 
{
// Algunes amigues
	friend ostream& operator<< (ostream& str, CAgent& ag);
// Construccio/Destruccio
public:
	CAgent();
	virtual ~CAgent();
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (void)=0;
	virtual void dump(CMissatger & msg);
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
	static map<string, CAgent*> s_DiccionariAgents;
// Funcions estatiques
public:
	static void dumpDiccionari(CMissatger& msg);
	static CAgent * cercaDiccionari(string s);
};


// Aquesta classe es per poder fer un Agent de qualsevol objecte valid 
// Valid=respongui al operador () Per exemple una funcio.

//template <class T>
class CFuncioAgent : public CAgent
{
// Tipus Propis
//	typedef T t_enmascarat;
	typedef void (t_enmascarat)();

// Construccio/Destruccio
public:
	CFuncioAgent(t_enmascarat * accio)
	{
		m_accio=accio;
		m_tipus+="/Funcio Usuari";
	}
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (void)
	{
		(*m_accio)();
	}
// Atributs
public:
	t_enmascarat * m_accio;
// Proves
public:
	static void ProvaClasse();
};

#endif // !defined(__KKEP_AGENT_H_INCLUDED)
