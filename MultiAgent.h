// MultiAgent.h: interface for the CMultiAgent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_MULTIAGENT_H_INCLUDED)
#define __KKEP_MULTIAGENT_H_INCLUDED

#include <list>
#include "Agent.h"

using namespace std;

class CMultiAgent : public CAgent
{
// Construccio/Destruccio
public:
	CMultiAgent()
	{
		m_tipus+="/Multiple";
	};
	virtual ~CMultiAgent()
	{
		while (!m_agents.empty()) {
			delete (*(m_agents.begin()));
			m_agents.pop_front();
		}
	};
// Operacions
public:
	void accio(CAgent * ag) {m_agents.push_back(ag);}
// Funcions virtuals definides a les subclasses
public:
	virtual list<CAgent*> subordinats() {
		list<CAgent*> l=CAgent::subordinats();
		for (list<CAgent*>::iterator it=m_agents.begin(); it!=m_agents.end(); it++)
			l.push_back(*it);
		return l;
	};
	virtual void operator() (void){
		for (list<CAgent*>::iterator it=m_agents.begin(); it!=m_agents.end(); it++)
			(**it)();
	};
// Atributs
public:
	list<CAgent *> m_agents;
// Proves
public:
	void dump (CMissatger & msg);
	static void ProvaClasse();
// Implementacio
private:
};

#endif // !defined(__KKEP_MULTIAGENT_H_INCLUDED)
