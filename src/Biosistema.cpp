// Biosistema.cpp: implementation of the CBiosistema class.
//
//////////////////////////////////////////////////////////////////////
// TODO:
//////////////////////////////////////////////////////////////////////

#include <fstream>
#include "Biosistema.h"
#include "Agent.h"
#include "TopologiaToroidal.h"
#include "Missatger.h"
#include "Configuracio.h"

using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Variables estatiques
//////////////////////////////////////////////////////////////////////

static CMemoryOutputer unLog(clog, 26);
//static CColorOutputer unLog(clog);
static CMissatger logAccio("Accions realitzades", NULL, unLog);

//////////////////////////////////////////////////////////////////////
// Predefinicions
//////////////////////////////////////////////////////////////////////

//static uint32 reparteixRegistres (uint32 vector, uint32 bits, uint32 & remanent, uint32 & p1, uint32 &p2, uint32 &p3, uint32 &p4, uint32 &p5);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBiosistema::CBiosistema()
{
	m_biotop = NULL;
	m_comunitat = NULL;
	m_agents = NULL;
//	m_taxonomista = NULL;
	logAccio.activa();

	m_instruccionsUtilsRestants=0;
	m_maxInstruccionsUtils=2;
	m_instruccionsRestants=0;
	m_maxInstruccions=4;
	m_organismeActiu=NULL;
	m_tempsPerAccionarAgents=0;
	m_temps=0;
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
	if (!m_tempsPerAccionarAgents--) {
		m_tempsPerAccionarAgents=m_comunitat->tamany();
		if (m_agents) {
			(*m_agents)();
			logAccio << magenta.brillant() << 'T' << m_temps++ << " Accionant agents" << blanc.fosc() << endl;
		}
	}
	// Execucio d'una instrucció 
	bool bUtil;
	if (!m_instruccionsUtilsRestants || !m_instruccionsRestants) {
		canviaOrganismeActiu();
	}
	// Defuncions
	if (!(*m_comunitat)[m_idOrganismeActiu].cos()->energia()) {
		eliminaOrganismeActiu();
		canviaOrganismeActiu();
	}

//	out << blanc.fosc() << "Actual " << oct << m_idOrganismeActiu << "      " << endl;

	uint32 taxo = (*m_comunitat)[m_idOrganismeActiu].taxo();
	logAccio 
		<< setfill('0') 
		<< setw(3) << (*m_comunitat)[m_idOrganismeActiu].subidentificador() << "-" 
		<< (m_idOrganismeActiu>>6) << CColor(1+((m_idOrganismeActiu&070)>>3)).brillant() << (m_idOrganismeActiu&07) << blanc.fosc() << " " 
		<< (taxo>>6) << negre.fons(1+((taxo&070)>>3)) << (taxo&7) << blanc.fosc() << " "
		<< setfill(' ');

	uint32 instr = (*m_comunitat)[m_idOrganismeActiu].cos()->seguentInstruccio();
	uint32 param1 = (instr>>3)&07;
	uint32 param2 = (instr>>6)&07;
	uint32 param3 = (instr>>9)&07;
	uint32 param4 = (instr>>12)&07;

	switch (instr & 0x7) {
	case 0x1:
		bUtil = organismeAvanca(rnd.get(), rnd.get(), rnd.get());
		break;
	case 0x2:
	case 0x7:
		bUtil = organismeAtaca(rnd.get(), param1&3, param2&3,  param3, rnd.get());
		break;
	case 0x3:
	case 0x0:
	case 0x6:
		bUtil = organismeEngoleix(rnd.get(), param1&3, param2&3);
		break;
	case 0x4:
		bUtil = organismeExcreta(rnd.get(), param1&3, param2&3);
		break;
	case 0x5:
		bUtil = organismeMitosi(rnd.get(), rnd.get());
		break;
	default:
		error << "Eps, accio no valida!!" << endl;
		break;
	}
	if (bUtil) m_instruccionsUtilsRestants--;
	else m_organismeActiu->consumeix(1);
	m_instruccionsRestants--;
	m_organismeActiu->consumeix(1);
	m_comunitat->dumpEnergies(out);
}

void CBiosistema::canviaOrganismeActiu()
{
	// Repoblacio
	if (m_comunitat->tamany()<Config.get("Comunitat/TamanyRegeneracio")) {
		logAccio << "Pocs individuus: Regenerant poblacio" << endl;
		while (!organismeExpontani()||!m_comunitat->tamany());
	}
	m_idOrganismeActiu = m_comunitat->organismeAleatori();
	m_infoOrganismeActiu = &((*m_comunitat)[m_idOrganismeActiu]);
	m_organismeActiu = m_infoOrganismeActiu->cos();
	m_instruccionsUtilsRestants = m_maxInstruccionsUtils;
	m_instruccionsRestants = m_maxInstruccions;
	logAccio << cyan.fosc() << "Organisme actual "<< oct << m_idOrganismeActiu << "." << dec << setw(2) << (*m_comunitat)[m_idOrganismeActiu].subidentificador() << blanc.fosc() << endl;
//	cin.get();
}

bool CBiosistema::organismeExpontani()
{
	uint32 pos = m_biotop->posicioAleatoria();
	if ((*m_biotop)[pos].esOcupat()) return false;
	// TODO: Cal canviar aixo, quan tinguem taxonomista
	static uint32 ultimTaxo=1;
//	ultimTaxo &= 077;
	if (!(ultimTaxo&070)) ultimTaxo+=8;
	uint32 taxo = ultimTaxo++;
	COrganisme * org = new COrganisme;
	if (!org) {
		error << "Fallo la memoria ocupando un organismo";
		cin.get();
		return false;
	}
	uint32 id = m_comunitat->introdueix(org, pos, taxo);
	(*m_biotop)[pos].ocupa(id);
	logAccio << verd.brillant() << "Generat expontaneament: " << oct << id << dec << blanc.fosc() << endl;
	ofstream logCromo("Cromosomes",ios::out|ios::app);
	CColorOutputer unaSortida(logCromo);
	CMissatger msg (NULL, NULL, unaSortida);
	msg << "Ancestre " << oct << taxo << dec << endl;
	org->m_cariotip.dump(msg);
	return true;
}

bool CBiosistema::eliminaOrganismeActiu()
{
	uint32 pos = m_infoOrganismeActiu->posicio();
	uint32 taxo = m_infoOrganismeActiu->taxo();
	// TODO: Cal eliminar el taxo
	(*m_biotop)[pos].desocupa();
	(*m_comunitat)[m_idOrganismeActiu].taxo(0);
	m_comunitat->extreu(m_idOrganismeActiu);
	logAccio << blau.brillant() << "Defuncio de l'organisme " << oct << m_idOrganismeActiu << dec << blanc.fosc() << endl;
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

bool CBiosistema::organismeMitosi(uint32 desp, uint32 energia)
{
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti  = m_biotop->desplacament(posOrigen, desp);
	logAccio << "Mitosi " << setw(4) << posDesti << ": ";
	CSubstrat & substratOrigen=(*m_biotop)[posOrigen];
	CSubstrat & substratDesti=(*m_biotop)[posDesti];
	if (substratDesti.esOcupat()) {
		logAccio << vermell.brillant() << "Ocupat per " << oct << substratDesti.ocupant() << dec << blanc.fosc() << endl;
		return false; // Error: Ja hi ha penya a la posicio, no la podem ocupar
	}
	// TODO: Cridar al constructor de 'copia' o mitosi millor dit
	COrganisme * nouOrganisme = new COrganisme(m_organismeActiu->m_cariotip);
	if (!nouOrganisme) {
		logAccio << verd.brillant() << "Falta memoria" << blanc.fosc() << endl;
		cin.get();
		return false;
	}
	// TODO: Cal canviar aixo, quan tinguem taxonomista
	uint32 taxo = m_infoOrganismeActiu->taxo(); 
	uint32 id = m_comunitat->introdueix(nouOrganisme, posDesti, taxo);
	substratDesti.ocupa(id);
	m_organismeActiu->m_cariotip.muta();
	m_organismeActiu->consumeix(15);
	logAccio << verd.brillant() << "Nascut " << oct << id << dec << blanc.fosc() << endl;
	// TODO: Logs
	return true;
}

bool CBiosistema::organismeAvanca(uint32 desp, uint32 energia, uint32 clauMoviment)
{
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti  = m_biotop->desplacament(posOrigen, desp);
	logAccio << "Em moc " << setw(3) << posOrigen << ": ";
	CSubstrat & substratOrigen=(*m_biotop)[posOrigen];
	CSubstrat & substratDesti=(*m_biotop)[posDesti];
	if (substratDesti.esOcupat()) {
		logAccio << vermell.brillant() << "Ocupat per " << oct << substratDesti.ocupant() << dec << blanc.fosc() << endl;
		return false; // Error: Ja hi ha penya a la posicio, no la podem ocupar
	}
	// Logica de moviment
	substratOrigen.desocupa();
	substratDesti.ocupa(m_idOrganismeActiu);
	logAccio << groc.brillant() << "Desti " << setw(4) << posDesti << " Ok" << blanc.fosc() << endl;
	(*m_comunitat)[m_idOrganismeActiu].posicio(posDesti);
	// TODO: Complicar la logica: Cost de desplacament, camí lliure, corrents...
	// TODO: Logs
	return true;
}


bool CBiosistema::organismeAtaca(uint32 desp, uint32 elementBase, uint32 tolerancia, uint32 energia, uint32 clauAtac)
{
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti = m_biotop->desplacament(posOrigen, desp);
	logAccio << "Ataco " << setw(4) << posDesti << ": ";
	if (posDesti==posOrigen) {
		logAccio << vermell.brillant() << "Autoagressio! " << oct << m_idOrganismeActiu << dec << blanc.fosc() << endl;
		return false;
	}
	CSubstrat & substratDesti = (*m_biotop)[posDesti];
	if (!substratDesti.esOcupat()) {
		logAccio << vermell.brillant() << "Posicio buida" << blanc.fosc() << endl;
		return false;
	}
	// Logica de contesa
	uint32 idOrganismeAtacat = substratDesti.ocupant();

	list<uint32> elements;
	if (!((*m_comunitat)[idOrganismeAtacat].cos())->defensa(elements, /*Patro atac*/rnd.get(), elementBase, tolerancia)) {
		logAccio << vermell.brillant() << oct << idOrganismeAtacat << dec << " no te " << hex << elementBase << "(" << tolerancia << ")" << dec << blanc.fosc() << endl;
		return false;
	}
	logAccio << groc.brillant() << "N'extrec " << elements.size() << " a " << idOrganismeAtacat << blanc.fosc() << endl;
	// TODO: Logs
	while (elements.size()) {
		m_organismeActiu->engoleix(elements.front());
		elements.pop_front();
	}
	return true;
}

bool CBiosistema::organismeEngoleix(uint32 desp, uint32 elementBase, uint32 tolerancia)
{
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti = m_biotop->desplacament(posOrigen, desp);
	logAccio << "Engolo " << setw(4) << posDesti << ": ";
	CSubstrat & substratDesti = (*m_biotop)[posDesti];
	uint32 element;
	if (!substratDesti.extreu(element, elementBase, tolerancia)) {
		logAccio << vermell.brillant() << "No hi ha " << hex << elementBase << "(" << tolerancia << ")" << dec << blanc.fosc() << endl;
		return false; // Error: No hi ha res que engolir
	}
	logAccio << groc.brillant() << "Em menjo un " << element << blanc.fosc() << endl;
	(*m_comunitat)[m_idOrganismeActiu].cos()->engoleix(element);
	return true;
}	

bool CBiosistema::organismeExcreta(uint32 desp, uint32 elementBase, uint32 tolerancia)
{
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti = m_biotop->desplacament(posOrigen, desp);
	logAccio << "Excreto " << posDesti << ": ";
	CSubstrat & substratDesti = (*m_biotop)[posDesti];
	uint32 element;
	if (!(*m_comunitat)[m_idOrganismeActiu].cos()->excreta(element, elementBase, tolerancia)) {
		logAccio << vermell.brillant() << "No tinc " << hex << elementBase << "(" << tolerancia << ")" << dec << blanc.fosc() << endl;
		return false; // Error: No hi ha res que excretar
	}
	logAccio << groc.brillant() << "Deposito un " << element << blanc.fosc() << endl;
	substratDesti.deposita(element);
	logAccio << "Ja he excretat" << endl;
	return true;
}	

bool CBiosistema::organismeCreaSensor(uint32 sensor, uint32 vector)
{
	// TODO: if (esta ocupat(sensor)) return false;
	uint32 p[5];
//	reparteixRegistres(vector, 5, vector, p[0], p[1], p[2], p[3], p[4]);
	return true;
}


//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CBiosistema::ProvaClasse()
{
	out << clrscr;
	out << blanc.brillant() << "Provant Biosistema" << blanc.fosc() << endl;
	CBiosistema biosistema;
	biosistema.biotop(new CTopologiaToroidal<CBiosistema::t_substrat>(30,30));
	CBiosistema::t_biotop * biotop = biosistema.biotop();
	biosistema.agents(CAgent::ParsejaArxiu("Agents2.ini", *biotop, error));
	biosistema.comunitat(new CComunitat);
	cin.get();
	out << clrscr;
	out << blanc.brillant() << "Provant Biosistema" << blanc.fosc() << endl;
	biosistema.biotop()->debugPresenta(out);
	char a;
	biosistema.biotop()->debugPresenta(out);
	while (true ||cin.get(a)&&a!='q') {
		biosistema.biotop()->debugPresenta(out);
		biosistema();
		out << "\033[H";
	}
}

//////////////////////////////////////////////////////////////////////
// Implementacio
//////////////////////////////////////////////////////////////////////
/*
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

*/

