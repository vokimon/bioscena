// Agent.h: interface for the CAgent class.
//
//////////////////////////////////////////////////////////////////////
// El destructor de las subclasses debera destruir los agentes subordinados 
// pero no los agentes de los que depende
//////////////////////////////////////////////////////////////////////
// Change log
// 19990820 VoK - Modificacions per tipus d'agent jerarquic
// 19990821 VoK - 
// 19990821 VoK - Afegit diccionari de Agents
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

using namespace std;

class CAgent 
{
// Algunes amigues
	friend ostream& operator<< (ostream& str, CAgent& ag);
// Construccio/Destruccio
public:
	CAgent();
	virtual ~CAgent();
// Operacions
public:
	string nom(){return m_nom;};
	bool nom(string unNom);
	string tipus() {return m_tipus;};
	virtual void dump(CMissatger& msg);
	void dumpAll(CMissatger& msg);
// Funcions virtuals definides a les subclasses
public:
	virtual list<CAgent *> subordinats() {list<CAgent *> l; return l;};
	virtual list<CAgent *> dependencies() {list<CAgent *> l; return l;};
	virtual void operator() (void)=0;
// Atributs
public:
	string m_tipus;
	bool m_nomDefinit;
	uint32 m_numeroAgent;
private:
	string m_nom; // Privat perque l'acces es complex i cal fer-ho amb els accessors
// Proves
public:
//	static void ProvaClasse();
// Implementacio
private:
// Membres statics
public:
	static uint32 s_ultimNumeroAgent;
	static map<string, CAgent*> s_DiccionariAgents;
	static void dumpDiccionari(CMissatger& msg);
	static CAgent* cercaDiccionari(string s);
};



// Aquesta classe es per poder fer un Agent de qualsevol objecte valid 
// Valid=respongui al operador () Per exemple una funcio.

//template <class T>
class CFuncioAgent : public CAgent
{
// Tipus associats
//	typedef T t_enmascarat;
	typedef void (t_enmascarat)();
// Construccio/Destruccio
public:
	CFuncioAgent(t_enmascarat * accio)
	{
		m_accio=accio;
		m_tipus+="/Funcio Usuari";
	}
// Operacions
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
