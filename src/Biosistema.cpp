// Biosistema.cpp: implementation of the CBiosistema class.
//
//////////////////////////////////////////////////////////////////////

#include "Biosistema.h"
#include "MultiAgent.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBiosistema::CBiosistema()
{
	// Jaume, ja se que no es optim pero nomes es fa una vegada
	// deixa'm ser correcte.
	m_biotop = NULL;
	m_comunitat = NULL;
	m_agents = NULL;

	m_agents = new CMultiAgent;
}

CBiosistema::~CBiosistema()
{
	if (m_biotop) delete m_biotop;
	if (m_comunitat) delete m_comunitat;
	if (m_agents) delete m_agents;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CBiosistema::ProvaClasse()
{
	
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

void CBiosistema::operator ( )()
// Cicle basic d'un biosistema
{
	// Execucio dels agents
	(*m_agents)();
	// Execucio d'una instrucció 
	bool bUtil;
	if (!m_instruccionsUtilsRestants || !m_instruccionsRestants)
	{
		m_organismeActiu = m_comunitat->organismeAleatori();
		m_instruccionsUtilsRestants = m_maxInstruccionsUtils;
		m_instruccionsRestants = m_maxInstruccions;
	}
	(*m_comunitat)[m_organismeActiu].m_organisme/*->instruccionSiguiente()*/;
	if (bUtil) m_instruccionsUtilsRestants--;
	m_instruccionsRestants--;
}

CBiosistema::t_agent * CBiosistema::agents()
{
	return m_agents;
}
