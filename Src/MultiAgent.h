// MultiAgent.h: interface for the CMultiAgent class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990814 VoK - Creat
// 19990818 VoK - Afegides funcions per recolectar subordinats
// 19990822 VoK - Funcions de dump
// 19990823 VoK - Reordenat els fitxers
// 19990823 VoK - Membre 'desubordina' per treure subordinats
// 19990824 VoK - dump ja no posa ':' despres de nom del parametre
// 19990824 VoK - Funcions de configuracio amb parametres
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_MULTIAGENT_H_INCLUDED)
#define __KKEP_MULTIAGENT_H_INCLUDED

#include <list>
#include "Agent.h"

using namespace std;

class CMultiAgent : public CAgent
{
// Tipus interns
public:
	typedef CAgent inherited;
	typedef CAgent t_accio; // Per proposist son diferents t_accio i inherited
//	typedef void (t_accio)(); // Aixo es per fer proves nomes
	typedef list<t_accio *> t_accions;

// Construccio/Destruccio
public:
	CMultiAgent();
	virtual ~CMultiAgent();
// Redefinibles
public:
	virtual void operator() (void);
	virtual void dump(CMissatger & msg);
	virtual bool configura(string parametre, istream & valor, t_diccionariAgents & diccionari, CMissatger & errors);
	virtual list<CAgent*> subordinats (void);
// Operacions
public:
	void accio(t_accio * ag);
	bool desubordina(CAgent * ag);
// Atributs
public:
	list<t_accio *> m_agents;
// Proves
public:
	static void ProvaClasse();
};

#endif // !defined(__KKEP_MULTIAGENT_H_INCLUDED)
