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

	m_instruccionsUtilsRestants=0;
	m_maxInstruccionsUtils=5;
	m_instruccionsRestants=0;
	m_maxInstruccions=4;
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
	out << "Organisme " << m_idOrganismeActiu << endl;
	(*m_comunitat)[m_idOrganismeActiu].cos()->seguentInstruccio();
	switch (rnd.get(1,6)) {
	case 1:
	case 5:
	case 6:
		bUtil = organismeAvanca(rnd.get(), rnd.get(), rnd.get());
		break;
	case 2:
		bUtil = organismeAtaca(rnd.get(), rnd.get(), rnd.get(), rnd.get(), rnd.get());
		break;
	case 3:
		bUtil = organismeEngoleix(rnd.get(), rnd.get(), ~0L);
		break;
	case 4:
		bUtil = organismeExcreta(rnd.get(), rnd.get(), rnd.get());
		break;
	default:
		break;
	}
	if (bUtil) m_instruccionsUtilsRestants--;
	m_instruccionsRestants--;
}

void CBiosistema::canviaOrganismeActiu()
{
	if (!m_comunitat->tamany()) {
		out << "M'he quedat sense individuus: Regenerant poblacio" << endl;
		cin.get();
		uint32 repeticionsMinimes=4;
		while (!organismeExpontani()||repeticionsMinimes--);
	}
	m_idOrganismeActiu = m_comunitat->organismeAleatori();
	m_instruccionsUtilsRestants = m_maxInstruccionsUtils;
	m_instruccionsRestants = m_maxInstruccions;
}

bool CBiosistema::organismeExpontani()
{
	uint32 pos = m_biotop->posicioAleatoria();
	if ((*m_biotop)[pos].esOcupat()) return false;
	uint32 taxo = rnd.get(1,9); // TODO: Cal canviar aixo, quan tinguem taxonomista
	uint32 id = m_comunitat->introdueix(new COrganisme, pos, taxo);
	(*m_biotop)[pos].ocupa(id);
	return true;
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
	uint32 posOrigen = (*m_comunitat)[m_idOrganismeActiu].posicio();
	uint32 posDesti  = m_biotop->desplacament(posOrigen, desp);
	out << "Movent des de " << posOrigen << " fin a " << posDesti << endl;
	CSubstrat & substratOrigen=(*m_biotop)[posOrigen];
	CSubstrat & substratDesti=(*m_biotop)[posDesti];
	if (substratDesti.esOcupat()) {
		out << "Fallada: La posició " << posDesti << " ja esta ocupada per " << substratDesti.ocupant() << endl;
		return false; // Error: Ja hi ha penya a la posicio, no la podem ocupar
	}
	// Logica de moviment
	substratOrigen.desocupa();
	substratDesti.ocupa(m_idOrganismeActiu);
	(*m_comunitat)[m_idOrganismeActiu].posicio(posDesti);
	// TODO: Complicar la logica: Cost de desplacament, camí lliure, corrents...
	// TODO: Logs
	return true;
}


bool CBiosistema::organismeAtaca(uint32 desp, uint32 elementBase, uint32 tolerancia, uint32 energia, uint32 clauAtac)
{
	uint32 posOrigen = (*m_comunitat)[m_idOrganismeActiu].m_posicio;
	uint32 posDesti = m_biotop->desplacament(posOrigen, desp);
	out << "Atacant la posició " << posDesti << endl;
	if (posDesti==posOrigen)
		tracaAnomalies << "Autoagressions de l'organisme " << m_idOrganismeActiu << endl;
	CSubstrat & substratDesti = (*m_biotop)[posDesti];
	if (!substratDesti.esOcupat()) {
		out << "Fallada: A la posició " << posDesti << " no hi ha ningu a qui atacar" << endl;
		return false; // Error: No hi ha ningú per atacar
	}
	// Logica de contesa
	uint32 idOrganismeAtacat = substratDesti.ocupant();

	list<uint32> elements;
	return ((*m_comunitat)[idOrganismeAtacat].cos())->defensa(elements, rnd.get(), rnd.get(), rnd.get());
	// TODO: Logs
}

bool CBiosistema::organismeEngoleix(uint32 desp, uint32 elementBase, uint32 tolerancia)
{
	uint32 posOrigen = (*m_comunitat)[m_idOrganismeActiu].m_posicio;
	uint32 posDesti = m_biotop->desplacament(posOrigen, desp);
	out << "Engolint a la posició " << posDesti << endl;
	CSubstrat & substratDesti = (*m_biotop)[posDesti];
	uint32 element;
	if (!substratDesti.extreu(element, elementBase, tolerancia)) {
		out << "Fallada: A la posició " << posDesti << " no hi ha res així per engolir" << endl;
		return false; // Error: No hi ha res que engolir
	}
	(*m_comunitat)[m_idOrganismeActiu].cos()->engoleix(element);
	return true;
}	

bool CBiosistema::organismeExcreta(uint32 desp, uint32 elementBase, uint32 tolerancia)
{
	uint32 posOrigen = (*m_comunitat)[m_idOrganismeActiu].m_posicio;
	uint32 posDesti = m_biotop->desplacament(posOrigen, desp);
	out << "Excretant cap a la posició " << posDesti << endl;
	CSubstrat & substratDesti = (*m_biotop)[posDesti];
	uint32 element;
	if (!(*m_comunitat)[m_idOrganismeActiu].cos()->excreta(element, elementBase, tolerancia)) {
		out << "Fallada: No he trobat al meu protoplasma res que s'hi assembli per excretar" << endl;
		return false; // Error: No hi ha res que excretar
	}
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
	out << "\033[J";// Un clrscr xapuser pero standard (ANSI)
	out << blanc.brillant() << "Provant Biosistema" << blanc.fosc() << endl;
	CBiosistema biosistema;
	biosistema.biotop(new CTopologiaToroidal<CBiosistema::t_substrat>(40,20));
	CBiosistema::t_biotop * biotop = biosistema.biotop();
	biosistema.agents(CAgent::ParsejaArxiu("Agents2.ini", *biotop, error));
	biosistema.comunitat(new CComunitat);
	biosistema.organismeExpontani();
	biosistema.organismeExpontani();
	biosistema.organismeExpontani();
	biosistema.organismeExpontani();
	biosistema.organismeExpontani();
	biosistema.organismeExpontani();
	biosistema.biotop()->debugPresenta(out);
	char a;
	biosistema.biotop()->debugPresenta(out);
	while (cin.get(a)&&a!='q') {
		biosistema.biotop()->debugPresenta(out);
		biosistema();
		out << "\033[J";// Un clrscr xapuser pero standard (ANSI)
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


