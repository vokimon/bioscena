// Biosistema.cpp: implementation of the CBiosistema class.
//
//////////////////////////////////////////////////////////////////////

#include "Biosistema.h"
#include "Agent.h"
#include "TopologiaToroidal.h"

//////////////////////////////////////////////////////////////////////
// Predefinicions
//////////////////////////////////////////////////////////////////////

static uint32 reparteixRegistres (uint32 vector, uint32 bits, uint32 & remanent, uint32 & p1, uint32 &p2, uint32 &p3, uint32 &p4 uint32 &p5);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBiosistema::CBiosistema()
{
	m_biotop = NULL;
	m_comunitat = NULL;
	m_agents = NULL;
	m_taxonomista = NULL;
}

CBiosistema::~CBiosistema()
{
	if (m_biotop) delete m_biotop;
	if (m_comunitat) delete m_comunitat;
	if (m_agents) delete m_agents;
	if (m_taxonomista) delete m_taxonomista;
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

void CBiosistema::operator ( )()
// Cicle basic d'un biosistema
{
	// Execucio dels agents
	if (m_agents) (*m_agents)();
	// Execucio d'una instrucció 
	bool bUtil;
	if (!m_instruccionsUtilsRestants || !m_instruccionsRestants)
	{
		m_idOrganismeActiu = m_comunitat->organismeAleatori();
		m_instruccionsUtilsRestants = m_maxInstruccionsUtils;
		m_instruccionsRestants = m_maxInstruccions;
	}
	(*m_comunitat)[m_idOrganismeActiu].m_organisme->seguentInstruccio();
	if (bUtil) m_instruccionsUtilsRestants--;
	m_instruccionsRestants--;
}

CBiosistema::t_biotop * CBiosistema::biotop()
{
	return m_biotop;
}

void CBiosistema::biotop(t_biotop * biotop)
{
	if (!m_biotop)
		m_biotop=biotop;
}

void CBiosistema::deleteBiotop()
{
	if (m_biotop)
		delete m_biotop;
	m_biotop=NULL;
}

CBiosistema::t_comunitat * CBiosistema::comunitat()
{
	return m_comunitat;
}

void CBiosistema::comunitat(t_comunitat * comunitat)
{
	if (!m_comunitat)
		m_comunitat=comunitat;
}

void CBiosistema::deleteComunitat()
{
	if (m_comunitat)
		delete m_comunitat;
	m_comunitat=NULL;
}

CBiosistema::t_agent * CBiosistema::agents()
{
	return m_agents;
}

void CBiosistema::agents(t_agent * agents)
{
	if (!m_agents)
		m_agents=agents;
}

void CBiosistema::deleteAgents()
{
	if (m_agents)
		delete m_agents;
	m_agents=NULL;
}

//////////////////////////////////////////////////////////////////////
// Operacions (Execucio d'intruccions)
//////////////////////////////////////////////////////////////////////

bool CBiosistema::organismeAvanca(uint32 desp, uint32 energia, uint32 clauMoviment)
{
	uint32 & posOrigen = (*m_comunitat)[m_idOrganismeActiu].m_posicio;
	uint32   posDesti  = m_biotop->desplacament(posOrigen, desp);
	CSubstrat & substratOrigen=(*m_biotop)[posOrigen];
	CSubstrat & substratDesti=(*m_biotop)[posDesti];
	if (substratDesti.esOcupat())
		return false; // Error: Ja hi ha penya a la posicio, no la podem ocupar

	// Logica de moviment
	substratOrigen.desocupa();
	substratDesti.ocupa(m_idOrganismeActiu);
	posOrigen = posDesti;
	// TODO: Complicar la logica: Cost de desplacament, camí lliure, corrents...

	// TODO: Logs
	return true;
}


bool CBiosistema::organismeAtaca(uint32 desp, uint32 elementBase, uint32 tolerancia, uint32 energia, uint32 clauAtac)
{
	uint32 posOrigen = (*m_comunitat)[m_idOrganismeActiu].m_posicio;
	uint32 posDesti = m_biotop->desplacament(posOrigen, desp);
	if (posDesti==posOrigen)
		traceOrganismes << "Autoagressions de l'organisme " << m_idOrganismeActiu << endl;
	CSubstrat & substratDesti = (*m_biotop)[posDesti];
	if (!substratDesti.esOcupat())
		return false; // Error: No hi ha ningú per atacar

	// Logica de contesa
	uint32 idOrganismeAtacat = substratDesti.ocupant();

	uint32 element;
	if ((*m_comunitat)[m_idOrganismeActiu].m_organisme->defensa(element, energia, clauAtac^))


	// TODO: Logs
	return true;
}

bool CBiosistema::organismeEngoleix(uint32 desp, uint32 elementBase, uint32 tolerancia)
{
	uint32 posOrigen = (*m_comunitat)[m_idOrganismeActiu].m_posicio;
	uint32 posDesti = m_biotop->desplacament(posOrigen, desp);
	CSubstrat & substratDesti = (*m_biotop)[posDesti];
	uint32 element;
	if (!substratDesti.extreu(element, elementBase, tolerancia))
		return false; // Error: No hi ha res que engolir
	(*m_comunitat)[m_idOrganismeActiu].m_organisme->engoleix(element);
	return true;
}	

bool CBiosistema::organismeExcreta(uint32 desp, uint32 elementBase, uint32 tolerancia)
{
	uint32 posOrigen = (*m_comunitat)[m_idOrganismeActiu].m_posicio;
	uint32 posDesti = m_biotop->desplacament(posOrigen, desp);
	CSubstrat & substratDesti = (*m_biotop)[posDesti];
	uint32 element;
	if (!(*m_comunitat)[m_idOrganismeActiu].m_organisme->excreta(element, elementBase, tolerancia))
		return false; // Error: No hi ha res que excretar
	substratDesti.deposita(element);
	return true;
}	

bool CBiosistema::organismeCreaSensor(uint32 sensor, uint32 vector)
{
	// TODO: if (esta ocupat(sensor)) return false;
	uint32 p[5];
	reparteixRegistres(vector, 5, vector, p[0]. p[1]. p[2]. p[3]. p[4]);
}


//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CBiosistema::ProvaClasse()
{
	CBiosistema biosistema;
	CBiosistema::t_biotop * biotop = new CTopologiaToroidal<CBiosistema::t_substrat>(20,70);
	biosistema.agents(CAgent::ParsejaArxiu("Agents.ini", *biotop, error));
	biosistema();
}

//////////////////////////////////////////////////////////////////////
// Implementacio
//////////////////////////////////////////////////////////////////////

uint32 reparteixRegistres (uint32 vector, uint32 bits, uint32 & remanent, uint32 & p1, uint32 &p2, uint32 &p3, uint32 &p4 uint32 &p5)
{
	for (uint32 i=bits, mascara = 0; i--; mascara<<=1, mascara++); 
	p1=vector&mascara;
	vector>>=bits;
	p2=vector&mascara;
	vector>>=bits;
	p3=vector&mascara;
	vector>>=bits;
	p4=vector&mascara;
	vector>>=bits;
	p5=vector&mascara;
	vector>>=bits;
	remanent=vector;
}

