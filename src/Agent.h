// Agent.h: interface for the CAgent class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __KKEP_AGENT_H_INCLUDED
#define __KKEP_AGENT_H_INCLUDED

#include <string>
#include <list>
#include <map>
#include <sstream>
#include <iomanip>
#include "BioIncludes.h"
#include "Missatger.h"

using namespace std;

class CBiosistema;


/**
* Defines the interface for enviromental agents.
* Enviromental agents allow the user to control the media 
* evolution along the time by ensambling the different 
* kinds of them.
* 
* <h3>Triggering</h3>
* 
* Each environmental agent has an action assigned.
* You may <b>trigger</b> this action by applying the 
* calling operator (empty parenthesys) to it.
* 
* <h3>Agents subordination</h3>
* 
* Agents are <b>subordinate</b> each other this means 
* that whenever an agent is triggered, it may trigger
* another agent. Subordination must form an loopless
* directed such that the whole graf can be accessed from
* a root node. 
* The most commont graph following this is rules is a simple tree.
* 
* Not all the agents call its subordinators, and not in
* the same way. That's a feature because it allows having
* subordinators agents such Probabilitzador (Probability based triggering),
* Temporizador (Cicle based triggering) and Iterador 
* (multible triggerings for each trigger).
*
* <h3>Queries on agents<h3>
* 
* Agents may also <b>query</b> each other values. 
* In order to avoid query loops, querying an agent
* must not imply subquerying another one, the queried 
* value must be calculated on the last triggering of 
* the queried agent.
* 
* <h3>Agent factory, activation and pasivation</h3>
* 
* You may use this abstract class as a factory class
* for agents. You may create an agent of a concrete
* type providing the string or integer identifier
* of the subclass.
*/
class CAgent 
{
// Tipus Propis
public:
	typedef map<string, CAgent*> t_diccionariAgents;
// Algunes amigues
	friend ostream& operator<< (ostream& str, CAgent& ag);
// Construccio/Destruccio
protected:
	CAgent();
public:
	virtual ~CAgent();
// Redefinibles
public:
	/** 
	* Triggers the agent action.
	* Redefine it on your own agents.
	*/
	virtual void operator() (void)=0;
	/**
	* Gives a list of pointers to the subordinated agents.
	* The way you redefine this function is to call the super
	* subordinated list and then append the subordinated agents
	* that your subclass adds.
	* @return A collection of subordinated agents
	*/
	virtual list<CAgent*> subordinats() {list<CAgent*> l; return l;};
	/**
	* Gives a list of pointer to the queried agents.
	* The way you redefine this function is to call the super
	* dependencies list and then append the queried agents
	* that your subclass adds.
	* @return A collection of queried agents
	*/
	virtual list<CAgent*> dependencies() {list<CAgent*> l; return l;};
protected:
	/**
	* Takes a configuration parameter from a configuration stream.
	* When you derive an Agent from another, you must redeclare
	* this function in order to intercept the configuration for
	* the added parameters and parameters that you want to
	* change the way they are configured.
	* 
	* If the received parameter is one of the taken you must
	* return true, if not, you must call super::configura 
	* function to give the superclass the chance to get it,
	* and return whatever it returns, true or false.
	* @param parametre The parameter name.
	* @param content An input stream where to read the 
	* parameter content from.
	* @param dictionary An agent dictionary you may use in 
	* order to get agents from their name. Notice that
	* dictionary names are the names used in the configuration 
	* source may not be the real one used in the activated agent tree. 
	* @return True if the parameter has been accepted and the content
	* parsing has been correct.
	*/
	virtual bool configura(string parameter, istream & content, t_diccionariAgents & diccionari, CMissatger & errors);
	/**
	* Dumps the agent configuration and state on the given stream.
	* Redefine it by calling super's dump and then dumping the new 
	* parameters on the stream.
	* @params stream The stream you may dump the parameters.
	*/
	virtual void dump(CMissatger & stream);
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
	static CAgent * ParsejaArxiu(const char * nomArxiu, CBiosistema & biosistema, CMissatger & errors);
private:
	static CAgent * CreaAgent(string tipus, CBiosistema & biosistema);
// Proves
public:
	static void ProvaClasse();
};

#endif//__KKEP_AGENT_H_INCLUDED
