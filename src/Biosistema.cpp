// Biosistema.cpp: implementation of the CBiosistema class.
//
//////////////////////////////////////////////////////////////////////

#include "Biosistema.h"
#include "Agent.h"
#include "TopologiaToroidal.h"

//////////////////////////////////////////////////////////////////////
// Variables estatiques
//////////////////////////////////////////////////////////////////////

static CMissatger tracaAnomalies;

//////////////////////////////////////////////////////////////////////
// Predefinicions
//////////////////////////////////////////////////////////////////////

static uint32 reparteixRegistres (uint32 vector, uint32 bits, uint32 & remanent, uint32 & p1, uint32 &p2, uint32 &p3, uint32 &p4, uint32 &p5);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBiosistema::CBiosistema()
{
	m_biotop = NULL;
	m_comunitat = NULL;
	m_agents = NULL;
//	m_taxonomista = NULL;
	tracaAnomalies.activa();
}

CBiosistema::~CBiosistema()
{
	if (m_biotop) delete m_biotop;
	if (m_comunitat) delete m_comunitat;
	if (m_agents) delete m_agents;
//	if (m_taxonomista) delete m_taxonomista;
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
		canviaOrganismeActiu();
	(*m_comunitat)[m_idOrganismeActiu].cos()->seguentInstruccio();
	if (bUtil) m_instruccionsUtilsRestants--;
	m_instruccionsRestants--;
}

void CBiosistema::canviaOrganismeActiu()
{
	if (!m_comunitat->tamany()) {
		m_comunitat->introdueix(new COrganisme, m_biotop->posicioAleatoria(), 0);
		m_comunitat->introdueix(new COrganisme, m_biotop->posicioAleatoria(), 0);
		m_comunitat->introdueix(new COrganisme, m_biotop->posicioAleatoria(), 0);
		m_comunitat->introdueix(new COrganisme, m_biotop->posicioAleatoria(), 0);
	}
	m_idOrganismeActiu = m_comunitat->organismeAleatori();
	m_instruccionsUtilsRestants = m_maxInstruccionsUtils;
	m_instruccionsRestants = m_maxInstruccions;
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
		tracaAnomalies << "Autoagressions de l'organisme " << m_idOrganismeActiu << endl;
	CSubstrat & substratDesti = (*m_biotop)[posDesti];
	if (!substratDesti.esOcupat())
		return false; // Error: No hi ha ningú per atacar

	// Logica de contesa
	uint32 idOrganismeAtacat = substratDesti.ocupant();

//	uint32 element;
	if ((*m_comunitat)[m_idOrganismeActiu].cos());//->defensa(element, energia, clauAtac^))


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
	(*m_comunitat)[m_idOrganismeActiu].cos()->engoleix(element);
	return true;
}	

bool CBiosistema::organismeExcreta(uint32 desp, uint32 elementBase, uint32 tolerancia)
{
	uint32 posOrigen = (*m_comunitat)[m_idOrganismeActiu].m_posicio;
	uint32 posDesti = m_biotop->desplacament(posOrigen, desp);
	CSubstrat & substratDesti = (*m_biotop)[posDesti];
	uint32 element;
	if (!(*m_comunitat)[m_idOrganismeActiu].cos()->excreta(element, elementBase, tolerancia))
		return false; // Error: No hi ha res que excretar
	substratDesti.deposita(element);
	return true;
}	

bool CBiosistema::organismeCreaSensor(uint32 sensor, uint32 vector)
{
	// TODO: if (esta ocupat(sensor)) return false;
	uint32 p[5];
	reparteixRegistres(vector, 5, vector, p[0], p[1], p[2], p[3], p[4]);
	return true;
}


//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CBiosistema::ProvaClasse()
{
	CBiosistema biosistema;
	biosistema.biotop(new CTopologiaToroidal<CBiosistema::t_substrat>(70,20));
	CBiosistema::t_biotop * biotop = biosistema.biotop();
	biosistema.agents(CAgent::ParsejaArxiu("Agents.ini", *biotop, error));
	biosistema.comunitat(new CComunitat);
	biosistema.comunitat()->introdueix(new COrganisme,1,1);
	biosistema.comunitat()->introdueix(new COrganisme,2,2);
	biosistema.comunitat()->introdueix(new COrganisme,3,3);
	while (cin.get()!=9) {
		biosistema();
		biosistema.biotop()->debugPresenta(out);
	}
}

//////////////////////////////////////////////////////////////////////
// Implementacio
//////////////////////////////////////////////////////////////////////

uint32 reparteixRegistres (uint32 vector, uint32 bits, uint32 & remanent, uint32 & p1, uint32 &p2, uint32 &p3, uint32 &p4, uint32 &p5)
{
	uint32 i;
	uint32 mascara;
	for (i=bits, mascara = 0; i--; mascara<<=1, mascara++); 
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
	return remanent=vector;
}

