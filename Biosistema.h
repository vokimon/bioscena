// Biosistema.h: interface for the CBiosistema class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_BIOSISTEMA_H_INCLUDED)
#define __KKEP_BIOSISTEMA_H_INCLUDED
#include "Topologia.h"
#include "Substrat.h"
#include "Agent.h"
#include "Comunitat.h"

class CBiosistema  
{
// Tipus propis
public:
	typedef CTopologia<CSubstrat> t_biotop;
	typedef CComunitat t_comunitat;
	typedef CAgent t_agent;
// Atributs
protected:
	t_biotop * m_biotop;
	t_comunitat * m_comunitat;
	t_agent * m_agents;

	uint32 m_instruccionsUtilsRestants;
	uint32 m_maxInstruccionsUtils;
	uint32 m_instruccionsRestants;
	uint32 m_maxInstruccions;
	uint32 m_organismeActiu;
// Construccio/Destruccio
public:
	CBiosistema();
	virtual ~CBiosistema();
// Operacions
public:
	void operator () (void);
	t_agent * agents();
	void agents(t_agent *);
	void deleteAgents();
// Proves
public:
	static void ProvaClasse(void);
// Implementacio
protected:
};

#endif // !defined(__KKEP_BIOSISTEMA_H_INCLUDED)
