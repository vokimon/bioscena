// Biosistema.cpp: implementation of the CBiosistema class.
//
//////////////////////////////////////////////////////////////////////
// Notes de manteniment:
// - m_infoOrganisme apunta a un element d'un vector. Com a tal es pot
//   invalidar en inserir o eliminar un element. Aixi s'ha obviat la
//   actualitzacio en eliminar doncs sempre s'elimina l'actual i cal 
//   actualitzar despres la variable amb un altre valor. 
// - Es suposa arreu que canviaActual deixa les variables 'actuals'
//   a un valor valid. (Sempre existeix un element com a minim)
//////////////////////////////////////////////////////////////////////

#include <fstream>

#ifdef _MSC_VER
namespace ops {
	#include <conio.h>
	}
#define kbhit ops::_kbhit
#else
#include <pc.h>
#endif

#include "Biosistema.h"
#include "Agent.h"
#include "TopologiaToroidal.h"
#include "Missatger.h"
#include "Configuracio.h"
#include "Grafic.h"

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
	logAccio.desactiva();

	m_maxInstruccionsUtils=Config.get("Biosistema/Quantum/Utils");
	m_maxInstruccions=Config.get("Biosistema/Quantum/Maxim");
	m_instruccionsUtilsRestants=0;
	m_instruccionsRestants=0;
	m_organismeActiu=NULL;
	m_tempsPerAccionarAgents=0;
	m_temps=0;

	m_probabilitatGeneracioExpontanea.fixa(
		Config.get("Comunitat/ProbabilitatGeneracioExpontanea/Mostra"),
		Config.get("Comunitat/ProbabilitatGeneracioExpontanea/Encerts")
		);

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
		m_temps++;
		logAccio << magenta.brillant() << "Biosistema: " << 'T' << m_temps << blanc.fosc() << endl;
		if (m_agents) {
			(*m_agents)();
			logAccio << magenta.brillant() << "Biosistema: " << blanc.fosc() << "Accionant agents externs" << endl;
		}
	}

	// Control del quantum
	if (!m_instruccionsUtilsRestants || !m_instruccionsRestants) {
		canviaOrganismeActiu();
	}
	if (!m_comunitat->esValid(m_idOrganismeActiu)) {
		error << "Identificador d'organisme invalid" << endl;
		cin.get();
	}		
	// Defuncions
	uint32 defuncions=0;
	while (!m_organismeActiu->energia()) {
		eliminaOrganismeActiu();
		canviaOrganismeActiu();
		defuncions++;
	}

//	out << blanc.fosc() << "Actual " << oct << m_idOrganismeActiu << "      " << endl;

	uint32 taxo = (*m_comunitat)[m_idOrganismeActiu].taxo();
	logAccio << (*m_comunitat)[m_idOrganismeActiu].descripcio();

	uint32 instr = (*m_comunitat)[m_idOrganismeActiu].cos()->seguentInstruccio();
	uint32 param1 = (instr>>4)&07;
	uint32 param2 = (instr>>7)&07;
	uint32 param3 = (instr>>10)&07;
	uint32 param4 = (instr>>13)&07;

	bool bUtil;
	switch (instr & 0xF) {
	case 0x0:
	case 0x1:
	case 0x2:
	case 0x3:
		bUtil = organismeAvanca(rnd.get(), rnd.get(), rnd.get());
		break;
	case 0x4:
	case 0x5:
		bUtil = organismeAtaca(rnd.get(), param1&3, param2&3,  param3, rnd.get());
		break;
	case 0x6:
	case 0x7:
//		bUtil = organismeExcreta(rnd.get(), param1&3, param2&3);
//		break;
	case 0x8:
	case 0x9:
		bUtil = organismeMitosi(rnd.get(), rnd.get());
		break;
	case 0xA:
	case 0xB:
	case 0xC:
	case 0xD:
	case 0xE:
	case 0xF:
		bUtil = organismeEngoleix(rnd.get(), param1&3, param2&3);
		break;
	default:
		error << "Eps, accio no valida!!" << endl;
		cin.get();
		break;
	}
	if (bUtil) m_instruccionsUtilsRestants--;
	else m_organismeActiu->consumeix(Config.get("Biosistema/Energia/AdicionalInutil"));
	m_instruccionsRestants--;
	m_organismeActiu->consumeix(Config.get("Biosistema/Energia/FixeInstruccio"));
	m_organismeActiu->m_foo++;
}

void CBiosistema::canviaOrganismeActiu()
// Post: Agafa un organisme valid com a actiu.
{
	// Repoblacio
	if (m_comunitat->tamany()<Config.get("Comunitat/TamanyRegeneracio")) {
		logAccio << magenta.brillant() << "Biosistema: " << blanc.fosc() << "Detectats pocs individuus" << endl;
		while (!organismeExpontani()||!m_comunitat->tamany());
	}
	else if (m_probabilitatGeneracioExpontanea.esDona())
		organismeExpontani();
	m_idOrganismeActiu = m_comunitat->organismeAleatori();
	m_infoOrganismeActiu = &((*m_comunitat)[m_idOrganismeActiu]);
	m_organismeActiu = m_infoOrganismeActiu->cos();
	m_instruccionsUtilsRestants = m_maxInstruccionsUtils;
	m_instruccionsRestants = m_maxInstruccions;
//	logAccio << cyan.fosc() << "Organisme actual " << blanc.fosc() << m_infoOrganismeActiu->descripcio() << endl;
}

bool CBiosistema::organismeExpontani()
{
	uint32 pos = m_biotop->posicioAleatoria();
	if ((*m_biotop)[pos].esOcupat()) return false;
	// TODO: Cal canviar aixo, quan tinguem taxonomista
	static uint32 ultimTaxo=0;
	if (!(ultimTaxo&070)) ultimTaxo+=010;
	uint32 taxo = ultimTaxo++;
	COrganisme * org = new COrganisme;
	if (!org) {
		error << "CBiosistema: Fallo la memoria ocupando un organismo expontaneo" << endl;
		cin.get();
		return false;
	}
	uint32 id = m_comunitat->introdueix(org, pos, taxo);
	(*m_biotop)[pos].ocupa(id);
	logAccio << (*m_comunitat)[id].descripcio() << verd.brillant() << "Generat expontaneament" << blanc.fosc() << endl;
	ofstream logCromo("poolgen.txt",ios::out|ios::app);
	CColorOutputer unaSortida(logCromo);
	CMissatger msg (NULL, NULL, unaSortida);
	msg << "Ancestre " << oct << taxo << dec << endl;
	org->m_cariotip.dump(msg);
	return true;
}

bool CBiosistema::eliminaOrganismeActiu()
{
	logAccio << m_infoOrganismeActiu->descripcio() << flush << blau.brillant() << "Defuncio" << blanc.fosc() << endl;

	// Alliberem la posicio
	uint32 pos = m_infoOrganismeActiu->posicio();
	(*m_biotop)[pos].desocupa();

	// Informem al taxonomista
	uint32 taxo = m_infoOrganismeActiu->taxo();
	// TODO: Cal eliminar el taxo

	// El desterrem de la comunitat
	m_comunitat->extreu(m_idOrganismeActiu);
	(*m_comunitat)[m_idOrganismeActiu].taxo(0); // Aixo indica que ha estat borrat adecuadament
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
		logAccio << vermell.brillant() << "Ocupat per " << blanc.fosc() << (*m_comunitat)[substratDesti.ocupant()].descripcio() << endl;
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
	m_organismeActiu->consumeix(Config.get("Biosistema/Energia/Mitosi"));
	logAccio << verd.brillant() << "Nascut " << blanc.fosc() << (*m_comunitat)[id].descripcio() << endl;
	// Restaurem el punter que probablement haura quedat invalidat
	m_infoOrganismeActiu = &((*m_comunitat)[m_idOrganismeActiu]);
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
		logAccio << vermell.brillant() << "Ocupat per " << blanc.fosc() << (*m_comunitat)[substratDesti.ocupant()].descripcio() << endl;
		return false; // Error: Ja hi ha penya a la posicio, no la podem ocupar
	}
	// Logica de moviment
	substratOrigen.desocupa();
	substratDesti.ocupa(m_idOrganismeActiu);
	m_organismeActiu->consumeix(Config.get("Biosistema/Energia/Moviment"));
	(*m_comunitat)[m_idOrganismeActiu].posicio(posDesti);
	logAccio << groc.brillant() << "Desti " << setw(4) << posDesti << " Ok" << blanc.fosc() << endl;
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
		logAccio << vermell.brillant() << "Autoagressio! " << blanc.fosc() << endl;
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
		logAccio << (*m_comunitat)[idOrganismeAtacat].descripcio() << vermell.brillant() << " no te " << hex << elementBase << "(" << tolerancia << ")" << dec << blanc.fosc() << endl;
		return false;
	}
	logAccio << groc.brillant() << "N'extrec " << elements.size() << " a " << blanc.fosc() << (*m_comunitat)[idOrganismeAtacat].descripcio() << endl;
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

	Config.parsejaArxiu("Bioscena.ini", error);
	CBiosistema biosistema;
	biosistema.biotop(new CTopologiaToroidal<CBiosistema::t_substrat>(Config.get("Biotop/CassellesAltitud"),Config.get("Biotop/CassellesAmplitud")));
	biosistema.agents(CAgent::ParsejaArxiu("Agents2.ini", *(biosistema.biotop()), error));
	biosistema.comunitat(new CComunitat);
	enum {Blanc, Mapa, MapaOrganismes, Organismes} mode = MapaOrganismes;
	bool modeCanviat=true;
	CComparativaOrganismes graf1(biosistema.comunitat());
	CComparativaOrganismes graf2(biosistema.comunitat());
	CComparativaOrganismes graf3(biosistema.comunitat());
	CComparativaOrganismes graf4(biosistema.comunitat());
	CComparativaOrganismes graf5(biosistema.comunitat());
	CMapa mapa1(&biosistema);

	cin.get();
	mapa1.primeraPosicio(0);
	while (true) {
		if (modeCanviat) {
			out << clrscr;
			switch (mode) {
			case Mapa:
			default:
				mapa1.tamany(1,2,80,45); 
				break;
			case MapaOrganismes:
				mapa1.tamany(1,2,40,40);
				graf1.tamany(41,1,40,8);
				graf1.primerOrganisme(0);
				graf2.tamany(41,11,40,8);
				graf2.primerOrganisme(40);
				graf3.tamany(41,21,40,8);
				graf3.primerOrganisme(80);
				graf4.tamany(41,31,40,8);
				graf4.primerOrganisme(120);
				graf5.tamany(41,41,40,8);
				graf5.primerOrganisme(160);
				break;
			case Organismes:
				graf1.tamany(1,1,80,8);
				graf1.primerOrganisme(0);
				graf2.tamany(1,11,80,8);
				graf2.primerOrganisme(80);
				graf3.tamany(1,21,80,8);
				graf3.primerOrganisme(160);
				graf4.tamany(1,31,80,8);
				graf4.primerOrganisme(240);
				graf5.tamany(1,41,80,8);
				graf5.primerOrganisme(320);
			}
			modeCanviat=false;
		}

		biosistema();
		if (!biosistema.temps()) {
			if (mode==Mapa||mode==MapaOrganismes)
				mapa1.visualitza(out);
			if (mode==MapaOrganismes||mode==Organismes) {
				graf1.visualitza(out);
				graf2.visualitza(out);
				graf3.visualitza(out);
				graf4.visualitza(out);
				graf5.visualitza(out);
			}
			out << gotoxy(1, 1) << "Poblacio: " << setw(6) << biosistema.comunitat()->tamany() << endl;
			out << gotoxy(1,26) << "Temps: " << setw(8) << biosistema.m_temps << endl;
			cout << gotoxy(48,20) << flush;
//			cin.rdbuf()->sync();
			if (kbhit() || cin.rdbuf()->in_avail()) 
			{
				char a=cin.get();
				if (cin) {
					switch (a) 
					{
					case 'D': mapa1.scrollPageRight(1); break;
					case 'd': mapa1.scrollRight(1); break;
					case 'A': mapa1.scrollPageLeft(1); break;
					case 'a': mapa1.scrollLeft(1); break;
					case 'W': mapa1.scrollPageUp(1); break;
					case 'w': mapa1.scrollUp(1); break;
					case 'S': mapa1.scrollPageDown(1); break;
					case 's': mapa1.scrollDown(1); break;
					case 'M': 
						modeCanviat = true;
						if (mode==MapaOrganismes) mode=Organismes;
						else if (mode==Mapa) mode=Blanc;
						else modeCanviat = false;
						break;
					case 'm': 
						modeCanviat = true;
						if (mode==Organismes) mode=MapaOrganismes;
						else if (mode==Blanc) mode=Mapa;
						else modeCanviat = false;
						break;
					case 'O': 
						modeCanviat = true;
						if (mode==MapaOrganismes) mode=Mapa;
						else if (mode==Organismes) mode=Blanc;
						else modeCanviat = false;
						break;
					case 'o': 
						modeCanviat = true;
						if (mode==Mapa) mode=MapaOrganismes;
						else if (mode==Blanc) mode=Organismes;
						else modeCanviat = false;
						break;
					case 'Q': 
					case 'q': return;
					case 'R': 
					case 'r': /*TODO: Reset Biosistema */ break;
					}
					out << gotoxy(48,1) << "Tecla: " << '\'' << (a?a:'#') << '\''<< endl;
				}
			}
		}
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


uint32 CBiosistema::temps()
{
	return m_tempsPerAccionarAgents;

}
