// Agent.h: interface for the CAgent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_AGENT_H_INCLUDED)
#define __KKEP_AGENT_H_INCLUDED

#include <string>
#include <list>
using namespace std;

class CAgent 
{
// Construccio/Destruccio
public:
	CAgent()
	{
		m_nom="Agent";
	};
	virtual ~CAgent(){};
// Operacions
public:
	string nom(){return m_nom;}
	void nom(string unNom){m_nom=unNom;}
// Funcions virtuals definides a les subclasses
public:
	virtual void operator() (void)=0;
// Atributs
public:
	string m_nom;
// Proves
public:
//	static void ProvaClasse();
// Implementacio
private:
};

class CMultiAgent : public CAgent
{
// Construccio/Destruccio
public:
	CMultiAgent()
	{
		m_nom="Agent multiple";
	};
	virtual ~CMultiAgent(){};
// Operacions
public:
	void add(CAgent * ag){m_agents.push_back(ag);}
// Funcions virtuals definides a les subclasses
public:
	virtual void operator() (void){
		for (list<CAgent*>::iterator it=m_agents.begin(); it!=m_agents.end(); it++)
			(**it)();
	};
// Atributs
public:
	list<CAgent *> m_agents;
// Proves
public:
	static void ProvaClasse();
// Implementacio
private:
};

// Aquesta classe es per poder fer un Agent de qualsevol objecte valid 
// Valid=respongui al operador () Per exemple una funcio.
template <class T>
class CAgentEnmascarador : public CAgent
{
// Tipus associats
	typedef T t_enmascarat;
// Construccio/Destruccio
public:
	CAgentEnmascarador(t_enmascarat * accio)
	{
		m_accio=accio;
	}
	void operator() (void)
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

typedef void voidfnc (void);
voidfnc hola;

template <class T>
void CAgentEnmascarador<T>::ProvaClasse ()
{
	CAgentEnmascarador<voidfnc> ag(hola);
	ag();
}

#endif // !defined(__KKEP_AGENT_H_INCLUDED)
