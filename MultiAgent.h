// MultiAgent.h: interface for the CMultiAgent class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990814 VoK - Creat
// 19990818 VoK - Afegides funcions per recolectar subordinats
// 19990822 VoK - Funcions de dump
// 19990823 VoK - Reordenat els fitxers
// 19990823 VoK - Membre 'desubordina' per treure subordinats
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_MULTIAGENT_H_INCLUDED)
#define __KKEP_MULTIAGENT_H_INCLUDED

#include <list>
#include "Agent.h"

using namespace std;

class CMultiAgent : public CAgent
{
// Tipus Propis
	typedef CAgent t_accio;
//	typedef void (t_accio)(); // Aixo es per fer proves nomes
	typedef list<t_accio *> t_accions;

// Construccio/Destruccio
public:
	CMultiAgent();
	virtual ~CMultiAgent();
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (void);
	virtual void dump(CMissatger & msg);
	virtual list<CAgent*> subordinats (void);
// Operacions
public:
	void accio(t_accio * ag);
// Atributs
public:
	list<t_accio *> m_agents;
// Proves
public:
	bool desubordina(CAgent * ag);
	static void ProvaClasse();
};

#endif // !defined(__KKEP_MULTIAGENT_H_INCLUDED)
