// Biosistema.cpp: implementation of the CBiosistema class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990831 VoK - Prototips instruccions organismes
// 19990831 VoK - Ja tenim taxonomista per natros sols
// 19991120 VoK - Implementades les operacions de fenotip
// 19991122 VoK - Proves: Control de teclat + estat visualitzacio
// 19991128 VoK - Els opcodes es llegeixen d'un fitxer
// 19991130 VoK - Implementades les operacions sensorials quimiques
// 19991130 VoK - Reorganitzada la classe
// 19991205 VoK - Implementades les operacions sensorials presencia
// 19991210 VoK - Traspasats des d'Organisme.cpp els costos d'engolir, 
//                excretar i atacar
// 19991211 VoK - Traspasats des d'Organisme.cpp els costos de mitosi
//                i generacio expontanea
// 19991214 VoK - Incorporada kbhit portable
// 19991214 VoK - Funcio 'nibble' per fer mes llegible el codi.
// 19991220 VoK - Afegides operacions de Shift
// 20000103 VoK - Sensibilitat interna
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

#include "turbioconio.h"
#include "Biosistema.h"
#include "Agent.h"
#include "TopologiaToroidal.h"
#include "Missatger.h"
#include "Configuracio.h"
#include "Grafic.h"
#include "Compatibilitat.h"

using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Variables estatiques
//////////////////////////////////////////////////////////////////////

static CMemoryOutputer unLog(clog, 40);
//static CColorOutputer unLog(clog);
static CMissatger logAccio("Accions realitzades", NULL, unLog);
// TODO: Cal canviar aixo, quan tinguem taxonomista
static uint32 ultimTaxo=0;

//////////////////////////////////////////////////////////////////////
// Funcions Auxiliars (Locals al modul)
//////////////////////////////////////////////////////////////////////

struct COpcodeInfo {
	char *mnemonic;
	CBiosistema::t_accioOrganisme fn;
	char *param[8];
	};

COpcodeInfo opcodes [] = {
	{"NoOperacio", &CBiosistema::organismeNoOperacio, {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}},
	{"Avanca", &CBiosistema::organismeAvanca, {"dir","ener","clau",NULL,NULL,NULL,NULL,NULL}},
	{"Mitosi", &CBiosistema::organismeMitosi, {"dir","ener",NULL,NULL,NULL,NULL,NULL,NULL}},
	{"Ataca", &CBiosistema::organismeAtaca, {"dir","base","tol","clau",NULL,NULL,NULL,NULL}},
	{"Engoleix", &CBiosistema::organismeEngoleix, {"dir","base","tol",NULL,NULL,NULL,NULL,NULL}},
	{"Excreta", &CBiosistema::organismeExcreta, {"dir","base","tol",NULL,NULL,NULL,NULL,NULL}},
	{"And", &CBiosistema::organismeAnd, {"dest","op1","op2",NULL,NULL,NULL,NULL,NULL}},
	{"Xor", &CBiosistema::organismeXor, {"dest","op1","op2",NULL,NULL,NULL,NULL,NULL}},
	{"Or", &CBiosistema::organismeOr, {"dest","op1","op2",NULL,NULL,NULL,NULL,NULL}},
	{"ShiftR", &CBiosistema::organismeShiftRight, {"dest","op1","num",NULL,NULL,NULL,NULL,NULL}},
	{"ShiftL", &CBiosistema::organismeShiftLeft, {"dest","op1","num",NULL,NULL,NULL,NULL,NULL}},
	{"Not", &CBiosistema::organismeNot, {"dest","op1",NULL,NULL,NULL,NULL,NULL,NULL}},
	{"Oposa", &CBiosistema::organismeOposa, {"dest","op1",NULL,NULL,NULL,NULL,NULL,NULL}},
	{"Copia", &CBiosistema::organismeCopia, {"dest","op1",NULL,NULL,NULL,NULL,NULL,NULL}},
	{"Random", &CBiosistema::organismeRandom, {"dest",NULL,NULL,NULL,NULL,NULL,NULL,NULL}},
	{"Carrega", &CBiosistema::organismeCarrega, {"dest",NULL,NULL,NULL,NULL,NULL,NULL,NULL}},
	{"SensorQ", &CBiosistema::organismeSensorQuimic, {"dir","radi","base","tol","ddir","dtip",NULL,NULL}},
	{"SensorP", &CBiosistema::organismeSensorPresencia, {"dir","radi","base","tol","ddir","dtip",NULL,"reg"}},
	{"SensorI", &CBiosistema::organismeSensorIntern, {NULL,NULL,"base","tol","denr","dtip",NULL,NULL}},
	{"Anabol", &CBiosistema::organismeAnabolitza, {"patA","patB","tolA","tolB","desC",NULL,NULL,NULL}},
	{"Catabol", &CBiosistema::organismeCatabolitza,{"patA","tolA","clau","desB","desC",NULL,NULL,NULL}}, 
	{NULL, NULL, {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}}
};

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

	m_calEspeciar=false;
	m_probabilitatGeneracioExpontanea.fixa(
		Config.get("Comunitat/ProbabilitatGeneracioExpontanea/Mostra"),
		Config.get("Comunitat/ProbabilitatGeneracioExpontanea/Encerts")
		);

	m_bitsCodiOperacio=Config.get("Biosistema/OpCodes/BitsOperacio");
	m_nCodisOperacio=1<<m_bitsCodiOperacio;
	m_mascaraCodis = m_nCodisOperacio-1;
	m_opcodes = NULL;

	m_mascaraQuimica = Config.get("Biosistema/Metabolisme/BitsSignificatius");
}

CBiosistema::~CBiosistema()
{
	if (m_biotop) delete m_biotop;
	if (m_comunitat) delete m_comunitat;
	if (m_agents) delete m_agents;
//	if (m_taxonomista) delete m_taxonomista;
	if (m_opcodes==NULL) delete [] m_opcodes;
}

//////////////////////////////////////////////////////////////////////
// Operacions (Acces als membres)
//////////////////////////////////////////////////////////////////////

CBiosistema::t_biotop * CBiosistema::biotop() const
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

CBiosistema::t_comunitat * CBiosistema::comunitat() const
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

CBiosistema::t_agent * CBiosistema::agents() const
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
// Operacions (Inicialitzacio)
//////////////////////////////////////////////////////////////////////

bool CBiosistema::carregaOpCodes(char * nomArxiu, CMissatger & errors)
{
	string prefetch;
	string mnemonicOperacio;
	uint32 valor;

	// Obrim l'arxiu
	ifstream entrada(nomArxiu);

	out << "Carregant la taula de codis d'operacions [" << nomArxiu << "]" << endl;
	if (!entrada) {
		errors 
			<< "Error obrint '" << nomArxiu << '\'' << endl;
		return false;
	}
	if (m_opcodes) delete [] m_opcodes;
	m_opcodes = new t_accioOrganisme[m_nCodisOperacio];
	if (m_opcodes)
		for (uint32 i=m_nCodisOperacio; i--;) {
			m_opcodes[i]= (t_accioOrganisme) NULL;
		}
	else {
		errors 
			<< "Error memoria carregant els Codis d'operacio" << endl;
		return false;
	}

	//TODO: (MSVC5) Pk quan s'executa la seguent instruccio s'enguarra m_opcodes?????
	entrada >> prefetch;
	while (entrada && prefetch=="*") {	
		entrada >> hex >> valor >> dec >> mnemonicOperacio >> prefetch;
		// TODO: Esborrar aquesta traca (o no)
		out << '\t' << hex << valor << dec << '\t' << mnemonicOperacio << endl;
		if (valor>=m_nCodisOperacio) {
			errors 
				<< "El valor " << valor 
				<< " no es pot representar amb els " << m_bitsCodiOperacio 
				<< " bits que hi ha pel codi d'operacio" << endl;
			continue;
		}
// TODO: (MSVC5) Restaurar aquesta proteccio quan l'enguarrada de m_opcodes es solucioni
#ifndef _MSC_VER
		if (m_opcodes[valor]!=NULL) {
			errors
				<< "El valor " << valor
				<< " ja estava assignat a una altra operacio dins del fitxer." << endl;
			continue;
		}
#endif
		uint32 i;
		for (i=0; opcodes[i].mnemonic && mnemonicOperacio!=opcodes[i].mnemonic; i++);
		if (opcodes[i].mnemonic) {
			m_opcodes[valor]=opcodes[i].fn;
		}
		else {
			errors
				<< "El mnemonic '" << mnemonicOperacio
				<< "' no es un mnemonic valid per a aquest biosistema" << endl;
			m_opcodes[valor]=&CBiosistema::organismeNoOperacio;
		}
	}
	if (entrada) {
		errors << "No s'esperava '" << prefetch << "'"<< endl;
	}

	// Comprovacions a posteriori
	for (uint32 i=0; i<m_nCodisOperacio; i++)
		if (m_opcodes[i]==NULL) {
			warning <<
				"Codi " << i << " no assignat a cap operacio; per defecte: NoOperacio" << endl;
			m_opcodes[i]=&CBiosistema::organismeNoOperacio;
		}

	out << "Taula codis d'operacio [" << nomArxiu << "] carregada" << endl;

	return true;

}

//////////////////////////////////////////////////////////////////////
// Operacions (Cicle basic)
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
	while (!m_organismeActiu->energia()) {
		eliminaOrganismeActiu();
		canviaOrganismeActiu();
	}
	
	// Fetch i Execucio
	uint32 instr = m_organismeActiu->seguentInstruccio();
	bool bUtil = m_opcodes && (this->*m_opcodes[(instr>>24)&m_mascaraCodis] )(instr);

	// Actualitzacio del quantum i l'energia (l'energia tb es modifica en altres llocs)
	if (bUtil) m_instruccionsUtilsRestants--;
	else m_organismeActiu->consumeixEnergia(Config.get("Biosistema/Energia/AdicionalInutil"));
	m_instruccionsRestants--;
	m_organismeActiu->consumeixEnergia(Config.get("Biosistema/Energia/FixeInstruccio"));

	// TODO: Treure modificacio del foo. 
	// Es per asegurar-nos de que la memoria de l'organisme es nostra. Si no ho es, casca.
	m_organismeActiu->m_foo++;
}

void CBiosistema::canviaOrganismeActiu()
// Post: Agafa un organisme valid com a actiu. (Si no n'hi ha, el crea)
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
	// Escollim una posicio aleatoria, si esta lliure continuem
	uint32 pos = m_biotop->posicioAleatoria();
	if ((*m_biotop)[pos].esOcupat()) return false;
	// Creem un taxo per l'ancestre
	// TODO: Cal canviar aixo, quan tinguem taxonomista
	if (!(ultimTaxo&070)) ultimTaxo+=010;
	uint32 taxo = ultimTaxo++;
	// 
	COrganisme * org = new COrganisme;
	if (!org) {
		error << "CBiosistema: Fallo la memoria ocupando un organismo expontaneo" << endl;
		cin.get();
		return false;
	}
	// Li posem l'energia inicial
	org->guanyaEnergia(Config.get("Organisme/Energia/Inicial"));
	// Ho introduim en societat (a la comunitat i al biotop)
	uint32 id = m_comunitat->introdueix(org, pos, taxo);
	(*m_biotop)[pos].ocupa(id);
	// Logging
	logAccio << verd.brillant() << "Generat expontaneament" << blanc.fosc() << (*m_comunitat)[id].descripcio() << endl;
	// Registrem el nou 'primigeni'
/*	ofstream logCromo("poolgen.txt",ios::out|ios::app);
	CColorOutputer unaSortida(logCromo);
	CMissatger msg (NULL, NULL, unaSortida);
	msg << "Ancestre " << oct << taxo << dec << endl;
	org->m_cariotip.dump(msg);
*/	return true;
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

//////////////////////////////////////////////////////////////////////
// Operacions (Execucio d'intruccions)
//////////////////////////////////////////////////////////////////////

bool CBiosistema::organismeMitosi(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 desp =    (*m_organismeActiu)[nibble(0,parametres)];
	uint32 energia = (*m_organismeActiu)[nibble(1,parametres)];
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti  = m_biotop->desplacament(posOrigen, desp);
	CSubstrat & substratDesti=(*m_biotop)[posDesti];

	// Fem pagar al futur pare la penalitzacio per cromosoma llarg
	uint32 tamanyPenat = Config.get("Organisme/Cariotip/PenalitzacioLlarg/BitsTamanyMaxim");
	uint32 factorPenat = Config.get("Organisme/Cariotip/PenalitzacioLlarg/Factor");
	m_organismeActiu->consumeixEnergia((m_organismeActiu->m_cariotip.tamanyCodons()>>tamanyPenat)*factorPenat);

	// Inici del log
	logAccio << m_infoOrganismeActiu->descripcio() << "Mitosi " << hex << nibble(0,parametres) << ":" << setw(8) << desp << dec << ": ";

	// Mirem si encara tenim energia per fer la mitosi
	if (!m_organismeActiu->consumeixEnergia(Config.get("Biosistema/Energia/Mitosi")*Config.get("Biosistema/Energia/FactorDescendencia"))) {
		logAccio << vermell << "No tinc energia" << blanc.fosc() << endl;
		return false;
		}

	if (substratDesti.esOcupat()) {
		logAccio << vermell.brillant() << "Ocupat " << blanc.fosc() << (*m_comunitat)[substratDesti.ocupant()].descripcio() << endl;
		return false; // Error: Ja hi ha penya a la posicio, no la podem ocupar
	}
	COrganisme * nouOrganisme = new COrganisme(m_organismeActiu->m_cariotip);
	if (!nouOrganisme) {
		logAccio << vermell.brillant() << "Falta memoria" << blanc.fosc() << endl;
		cin.get();
		return false;
	}
	// TODO: Cal canviar aixo, quan tinguem taxonomista
	uint32 taxo;
	if (m_calEspeciar && nouOrganisme->m_mutat)
	{
		if (!(ultimTaxo&070)) ultimTaxo+=010;
		taxo = ultimTaxo++;
	}
	else 
		taxo = m_infoOrganismeActiu->taxo();

	nouOrganisme->guanyaEnergia(Config.get("Biosistema/Energia/Mitosi"));
	uint32 id = m_comunitat->introdueix(nouOrganisme, posDesti, taxo);
	// Restaurem el punter que probablement haura quedat invalidat
	m_infoOrganismeActiu = &((*m_comunitat)[m_idOrganismeActiu]);
	substratDesti.ocupa(id);
	logAccio << verd.brillant() << "Nascut " << blanc.fosc() << (*m_comunitat)[id].descripcio() << endl;

	// Logging adicional (i provisional)
	if (taxo!=m_infoOrganismeActiu->taxo()) {
		logAccio << "Eps! Mutacio " /*<< m_infoOrganismeActiu->taxo() << "->" << taxo*/ << endl;
	}
	// TODO: Logs
	return true;
}

bool CBiosistema::organismeAvanca(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 desp =         (*m_organismeActiu)[nibble(0,parametres)];
	uint32 energia =      (*m_organismeActiu)[nibble(1,parametres)];
	uint32 clauMoviment = (*m_organismeActiu)[nibble(2,parametres)];
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti  = m_biotop->desplacament(posOrigen, desp);
	logAccio << m_infoOrganismeActiu->descripcio() << "Em moc " << hex << nibble(0,parametres) << ":" << setw(8) << desp << dec << ": ";
	CSubstrat & substratOrigen=(*m_biotop)[posOrigen];
	CSubstrat & substratDesti=(*m_biotop)[posDesti];
	if (substratDesti.esOcupat()) {
		logAccio << vermell.brillant() << "Ocupat " << blanc.fosc() << (*m_comunitat)[substratDesti.ocupant()].descripcio() << endl;
		return false; // Error: Ja hi ha penya a la posicio, no la podem ocupar
	}
	// Logica de moviment
	substratOrigen.desocupa();
	substratDesti.ocupa(m_idOrganismeActiu);
	m_organismeActiu->consumeixEnergia(Config.get("Biosistema/Energia/Moviment"));
	(*m_comunitat)[m_idOrganismeActiu].posicio(posDesti);
	logAccio << groc.brillant() << "Desti " << setw(4) << posDesti << " Ok" << blanc.fosc() << endl;
	// TODO: Complicar la logica: Cost de desplacament, camí lliure, corrents...
	// TODO: Logs
	return true;
}


bool CBiosistema::organismeAtaca(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 desp=       (*m_organismeActiu)[nibble(0,parametres)];
	uint32 elementBase=(*m_organismeActiu)[nibble(1,parametres)]&m_mascaraQuimica;
	uint32 tolerancia= (*m_organismeActiu)[nibble(2,parametres)]|~m_mascaraQuimica;
	uint32 clauAtac=   (*m_organismeActiu)[nibble(3,parametres)];
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti = m_biotop->desplacament(posOrigen, desp);
	logAccio << m_infoOrganismeActiu->descripcio() << "Ataco  " << hex << nibble(0,parametres) << ":" << setw(8) << desp << dec << ": ";
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
	if (!((*m_comunitat)[idOrganismeAtacat].cos())->defensa(elements, clauAtac , elementBase, tolerancia)) {
		logAccio << (*m_comunitat)[idOrganismeAtacat].descripcio() << vermell.brillant() << " no te " << hex << elementBase << "(" << ~tolerancia << ")" << dec << blanc.fosc() << endl;
		return false;
	}
	uint32 agarrapinyats = elements.size();
	logAccio << groc.brillant() << "N'extrec " << agarrapinyats << " a " << blanc.fosc() << (*m_comunitat)[idOrganismeAtacat].descripcio() << endl;
	m_organismeActiu->guanyaEnergia(Config.get("Biosistema/Energia/Engolir")*agarrapinyats);
	(*m_comunitat)[idOrganismeAtacat].cos()->consumeixEnergia(Config.get("Biosistema/Energia/Extraccio")*agarrapinyats);
	while (elements.size()) {
		m_organismeActiu->engoleix(elements.front());
		elements.pop_front();
	}
	// TODO: Logs
	return true;
}

bool CBiosistema::organismeEngoleix(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 desp=	   (*m_organismeActiu)[nibble(0,parametres)];
	uint32 elementBase=(*m_organismeActiu)[nibble(1,parametres)]&m_mascaraQuimica;
	uint32 tolerancia= (*m_organismeActiu)[nibble(2,parametres)]|~m_mascaraQuimica;
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti = m_biotop->desplacament(posOrigen, desp);
	logAccio << m_infoOrganismeActiu->descripcio() << "Engolo " << hex << nibble(0,parametres) << ":" << setw(8) << desp << dec << ": ";
	CSubstrat & substratDesti = (*m_biotop)[posDesti];
	uint32 element;
	if (!substratDesti.extreu(element, elementBase, tolerancia)) {
		logAccio << vermell.brillant() << "No hi ha " << hex << elementBase << "(" << ~tolerancia << ")" << dec << blanc.fosc() << endl;
		return false; // Error: No hi ha res que engolir
	}
	logAccio << groc.brillant() << "Em menjo un " << hex << element << dec << blanc.fosc() << endl;
	(*m_comunitat)[m_idOrganismeActiu].cos()->engoleix(element);
	m_organismeActiu->guanyaEnergia(Config.get("Biosistema/Energia/Engolir"));
	return true;
}	

bool CBiosistema::organismeExcreta(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 desp=	   (*m_organismeActiu)[nibble(0,parametres)];
	uint32 elementBase=(*m_organismeActiu)[nibble(1,parametres)]&m_mascaraQuimica;
	uint32 tolerancia= (*m_organismeActiu)[nibble(2,parametres)]|~m_mascaraQuimica;
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti = m_biotop->desplacament(posOrigen, desp);
	logAccio << m_infoOrganismeActiu->descripcio() << "Excret " << hex << nibble(0,parametres) << ":" << setw(8) << desp << dec << ": ";
	CSubstrat & substratDesti = (*m_biotop)[posDesti];
	uint32 element;
	if (!(*m_comunitat)[m_idOrganismeActiu].cos()->excreta(element, elementBase, tolerancia)) {
		logAccio << vermell.brillant() << "No tinc " << hex << elementBase << "(" << ~tolerancia << ")" << dec << blanc.fosc() << endl;
		return false; // Error: No hi ha res que excretar
	}
	logAccio << groc.brillant() << "Deposito un " << element << blanc.fosc() << endl;
	substratDesti.deposita(element);
	m_organismeActiu->consumeixEnergia(Config.get("Biosistema/Energia/Excretar"));
	return true;
}	

bool CBiosistema::organismeAnabolitza(uint32 parametres)
	// Realitza la reaccio A + B (+/-) energia -> C + D
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 patroA      = (*m_organismeActiu)[nibble(0,parametres)]&m_mascaraQuimica;
	uint32 patroB      = (*m_organismeActiu)[nibble(1,parametres)]&m_mascaraQuimica;
	uint32 toleranciaA = (*m_organismeActiu)[nibble(2,parametres)]|~m_mascaraQuimica;
	uint32 toleranciaB = (*m_organismeActiu)[nibble(3,parametres)]|~m_mascaraQuimica;
	uint32 &C          = (*m_organismeActiu)[nibble(4,parametres)];


	uint32 A,B;
	if (!m_organismeActiu->excreta(A, patroA, toleranciaA)) {
		logAccio << m_infoOrganismeActiu->descripcio() << "Anab   R" << hex << nibble(0,parametres) << "+R" << nibble(1,parametres) << "->R" << nibble(4,parametres) << dec << ": " << vermell.brillant() << "No tinc " << hex << patroA << "(" << ~toleranciaA << ")" << dec << blanc.fosc() << endl;
		return false; // No he pogut trobar el primer reactiu
	}
	
	if (!m_organismeActiu->excreta(B, patroB, toleranciaB)) {
		logAccio << m_infoOrganismeActiu->descripcio() << "Anab   R" << hex << nibble(0,parametres) << "+R" << nibble(1,parametres) << "->R" << nibble(4,parametres) << dec << ": " << vermell.brillant() << "No tinc " << hex << patroB << "(" << ~toleranciaB << ")" << dec << blanc.fosc() << endl;
		return false; // No he pogut trobar el primer reactiu
	}
	C=A|B;
	uint32 energia=comptaUns(C)- min(comptaUns(A),comptaUns(B));
	m_organismeActiu->engoleix(C);
	logAccio << m_infoOrganismeActiu->descripcio() << "Anab   " << hex << nibble(4,parametres) << "=" << nibble(0,parametres) << ":" << setw(8) << patroA << " " << nibble(1,parametres) << ":" << patroB << dec << ": ";
	logAccio << groc.brillant() << hex << A << "+" << B << "->" << C << dec << "-E(" << energia << ")" << blanc.fosc() << endl;
	m_organismeActiu->consumeixEnergia(energia);
	return true;
}	

bool CBiosistema::organismeCatabolitza(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 patroA         = (*m_organismeActiu)[nibble(0,parametres)]&m_mascaraQuimica;
	uint32 toleranciaA    = (*m_organismeActiu)[nibble(1,parametres)]|~m_mascaraQuimica;
	uint32 clauCatabolica = (*m_organismeActiu)[nibble(2,parametres)]|~m_mascaraQuimica;
	uint32 &B  = (*m_organismeActiu)[nibble(3,parametres)];
	uint32 &C  = (*m_organismeActiu)[nibble(4,parametres)];

	logAccio << m_infoOrganismeActiu->descripcio() << "Catab  " << hex << nibble(3,parametres) << "," << nibble(4,parametres) << "=" << setw(8) << patroA << ">" << nibble(2,parametres) << ":" << clauCatabolica << dec << ": ";

	uint32 A;
	if (!m_organismeActiu->excreta(A, patroA, toleranciaA)) {
		logAccio << vermell.brillant() << "No tinc " << hex << patroA << "(" << ~toleranciaA << ")" << dec << blanc.fosc() << endl;
		return false; // No he pogut trobar el primer reactiu
	}
	
	B=(A^clauCatabolica)&A;
	C=(A^~clauCatabolica)&A;
	m_organismeActiu->engoleix(C);
	m_organismeActiu->engoleix(C);
	uint32 energia=comptaUns(A)- max(comptaUns(B),comptaUns(C));
	logAccio << groc.brillant() << hex << A << "->" << B << "+" << C << dec << "+E(" << energia << ")" << blanc.fosc() << endl;
	m_organismeActiu->guanyaEnergia(energia);

	return true;
}

bool CBiosistema::organismeAnd(uint32 parametres)
{
	(*m_organismeActiu)[nibble(0,parametres)] =
	(*m_organismeActiu)[nibble(1,parametres)] &
	(*m_organismeActiu)[nibble(2,parametres)] ;
	logAccio << m_infoOrganismeActiu->descripcio() << "And    " << hex << nibble(0,parametres) << "=" << setw(8) << (*m_organismeActiu)[nibble(0,parametres)] << " " << nibble(1,parametres) << "&" << nibble(2,parametres) << dec << endl;
	return true;
}

bool CBiosistema::organismeOr(uint32 parametres)
{
	(*m_organismeActiu)[nibble(0,parametres)] =
	(*m_organismeActiu)[nibble(1,parametres)] |
	(*m_organismeActiu)[nibble(2,parametres)] ;
	logAccio << m_infoOrganismeActiu->descripcio() << "Or     " << hex << nibble(0,parametres) << "=" << setw(8) << (*m_organismeActiu)[nibble(0,parametres)] << " " << nibble(1,parametres) << "|" << nibble(2,parametres) << dec << endl;
	return true;
}

bool CBiosistema::organismeXor(uint32 parametres)
{
	(*m_organismeActiu)[nibble(0,parametres)] =
	(*m_organismeActiu)[nibble(1,parametres)] ^
	(*m_organismeActiu)[nibble(2,parametres)] ;
	logAccio << m_infoOrganismeActiu->descripcio() << "Xor    " << hex << nibble(0,parametres) << "=" << setw(8) << (*m_organismeActiu)[nibble(0,parametres)] << " " << nibble(1,parametres) << "^" << nibble(2,parametres) << dec << endl;
	return true;
}

bool CBiosistema::organismeShiftRight(uint32 parametres)
{
	(*m_organismeActiu)[nibble(0,parametres)] =
	(*m_organismeActiu)[nibble(1,parametres)] >>
	nibble(2,parametres);
	logAccio << m_infoOrganismeActiu->descripcio() << "ShiftR " << hex << nibble(0,parametres) << "=" << setw(8) << (*m_organismeActiu)[nibble(0,parametres)] << " " << nibble(1,parametres) << ">>" << dec << setw(2) << nibble(2,parametres) << endl;
	return true;
}

bool CBiosistema::organismeShiftLeft(uint32 parametres)
{
	(*m_organismeActiu)[nibble(0,parametres)] =
	(*m_organismeActiu)[nibble(1,parametres)] <<
	nibble(2,parametres);
	logAccio << m_infoOrganismeActiu->descripcio() << "ShiftL " << hex << nibble(0,parametres) << "=" << setw(8) << (*m_organismeActiu)[nibble(0,parametres)] << " " << nibble(1,parametres) << "<<"<< dec << setw(2) << nibble(2,parametres) << endl; 
	return true;
}

bool CBiosistema::organismeNot(uint32 parametres)
{
	(*m_organismeActiu)[nibble(0,parametres)] =
	~ (*m_organismeActiu)[nibble(1,parametres)] ;
	logAccio << m_infoOrganismeActiu->descripcio() << "Negat  " << hex << nibble(0,parametres) << "=" << setw(8) << (*m_organismeActiu)[nibble(0,parametres)] << " ~" << nibble(1,parametres) << dec << endl;
	return true;
}

bool CBiosistema::organismeOposa(uint32 parametres)
// Serveix per obtenir la direccio oposada
{
	(*m_organismeActiu)[nibble(0,parametres)] =
	(*m_organismeActiu)[nibble(1,parametres)] ^ 0x77777777;
	logAccio << m_infoOrganismeActiu->descripcio() << "Oposa  " << hex << nibble(0,parametres) << "=" << setw(8) << (*m_organismeActiu)[nibble(0,parametres)] << " " << nibble(1,parametres) << "^77777777" << dec << endl;
	return true;
}

bool CBiosistema::organismeRandom(uint32 parametres)
{
	(*m_organismeActiu)[nibble(0,parametres)] = rnd.get();
	logAccio << m_infoOrganismeActiu->descripcio() << "Random " << hex << nibble(0,parametres) << "=" << setw(8) << (*m_organismeActiu)[nibble(0,parametres)] << dec << endl;
	return true;
}

bool CBiosistema::organismeCopia(uint32 parametres)
{
	(*m_organismeActiu)[nibble(0,parametres)] =
	(*m_organismeActiu)[nibble(1,parametres)];
	logAccio << m_infoOrganismeActiu->descripcio() << "Copy   " << hex << nibble(0,parametres) << "=" << setw(8) << (*m_organismeActiu)[nibble(0,parametres)] << " " << nibble(1,parametres) << dec << endl;
	return true;
}

bool CBiosistema::organismeCarrega(uint32 parametres)
{
	(*m_organismeActiu)[nibble(0,parametres)] =
	m_organismeActiu->seguentInstruccio();
	logAccio << m_infoOrganismeActiu->descripcio() << "Load   " << hex << nibble(0,parametres) << "=" << setw(8) << (*m_organismeActiu)[nibble(0,parametres)] << dec << endl;
	return true;
}

bool CBiosistema::organismeNoOperacio(uint32 parametres)
{
	logAccio << m_infoOrganismeActiu->descripcio() << "NoOper NOP NOP NOP ..." << endl;
//	error << "Eps, accio no valida!!" << endl;
//	cin.get();
	return false;
}

bool CBiosistema::organismeSensorQuimic(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 direccio        = (*m_organismeActiu)[nibble(0,parametres)];
	uint32 radi            = (*m_organismeActiu)[nibble(1,parametres)]&31;
	uint32 clauElement     = (*m_organismeActiu)[nibble(2,parametres)]&m_mascaraQuimica;
	uint32 tolerancia      = (*m_organismeActiu)[nibble(3,parametres)]|~m_mascaraQuimica;
	uint32 & destiDireccio = (*m_organismeActiu)[nibble(4,parametres)];
	uint32 & destiTipus    = (*m_organismeActiu)[nibble(5,parametres)];
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posBase = m_biotop->desplacament(posOrigen, direccio);

	for (uint32 i=Config.get("Sensor/Quimic/Intents"); i--;) {
		uint32 posDesti = m_biotop->desplacamentAleatori (posBase, radi);
		if ((*m_biotop)[posDesti].rastreja(destiTipus, clauElement, tolerancia)) {
			// TODO: Aixo retorna true si hi arribes nomes amb el desplacament. Ho fem servir?
			m_biotop->unio(posOrigen,posDesti,destiDireccio);
			logAccio << m_infoOrganismeActiu->descripcio() << "SentoQ " << hex << nibble(4,parametres) << "=" << setw(8) << destiDireccio << " " << nibble(5,parametres) << "=" << setw(8) << destiTipus << " " << clauElement << "(" << ~tolerancia << ") a " << nibble(0,parametres) << ":" << setw(8) << direccio << dec << endl;
			return true;
		}
	}
	logAccio << m_infoOrganismeActiu->descripcio() << "SentoQ " << hex << nibble(4,parametres) << "=???????? " << nibble(5,parametres) << "=???????? " << clauElement << "(" << ~tolerancia << ") a " << nibble(0,parametres) << ":" << setw(8) << direccio << dec << endl;
	// TODO: Be, com sempre aixo hauria de tenir un significat
	return true;
}

bool CBiosistema::organismeSensorPresencia(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 direccio        = (*m_organismeActiu)[nibble(0,parametres)];
	uint32 radi            = (*m_organismeActiu)[nibble(1,parametres)]&31;
	uint32 clauElement     = (*m_organismeActiu)[nibble(2,parametres)];
	uint32 tolerancia      = (*m_organismeActiu)[nibble(3,parametres)];
	uint32 & destiDireccio = (*m_organismeActiu)[nibble(4,parametres)];
	uint32 & destiTipus    = (*m_organismeActiu)[nibble(5,parametres)];
	uint32 registre        = nibble(7,parametres);
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posBase = m_biotop->desplacament(posOrigen, direccio);

	for (uint32 i=Config.get("Sensor/Presencia/Intents"); i--;) {
		uint32 posDesti = m_biotop->desplacamentAleatori (posBase, radi);
		if ((*m_biotop)[posDesti].esOcupat()) {
			uint32 ocupant=(*m_biotop)[posDesti].ocupant();
			if (sonCompatibles((*(*m_comunitat)[ocupant].cos())[registre],clauElement,tolerancia)) {
				// TODO: Aixo retorna true si hi arribes nomes amb el desplacament. Ho fem servir?
				m_biotop->unio(posOrigen,posDesti,destiDireccio);
				logAccio << m_infoOrganismeActiu->descripcio() << "SentoP " << hex << nibble(4,parametres) << "=" << setw(8) << destiDireccio << " " << nibble(5,parametres) << "=" << setw(8) << destiTipus << " dir(" << nibble(0,parametres) << ") tq " << nibble(7,parametres) << "==R" << nibble(2,parametres) << "(R" << nibble(3,parametres) << ")" << dec << endl; 
				return true;
			}
		}
	}
	logAccio << m_infoOrganismeActiu->descripcio() << "SentoP " << hex << nibble(4,parametres) << "=???????? " << nibble(5,parametres) << "=???????? dir(" << nibble(0,parametres) << ") tq " << nibble(7,parametres) << "==R" << nibble(2,parametres) << "(R" << nibble(3,parametres) << ")" << dec << endl; 

	// TODO: Be, com sempre aixo hauria de tenir un significat
	return true;
}

bool CBiosistema::organismeSensorIntern(uint32 parametres)
{
	// TODO: SensorIntern no esta implementada encara
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 clauElement     = (*m_organismeActiu)[nibble(2,parametres)]&m_mascaraQuimica;
	uint32 tolerancia      = (*m_organismeActiu)[nibble(3,parametres)]|~m_mascaraQuimica;
	uint32 & destiEnergia  = (*m_organismeActiu)[nibble(4,parametres)];
	uint32 & destiTipus    = (*m_organismeActiu)[nibble(5,parametres)];

	destiEnergia = m_organismeActiu->energia();

	for (uint32 i=Config.get("Sensor/Intern/Intents"); i--;) {
		if (m_organismeActiu->detecta(destiTipus, clauElement, tolerancia)) {
			logAccio << m_infoOrganismeActiu->descripcio() << "SentoI " << hex << nibble(4,parametres) << "=" << setw(8) << destiEnergia << " " << nibble(5,parametres) << "=" << setw(8) << destiTipus << " " << clauElement << "(" << ~tolerancia << ") " << dec << endl;
			return true;
		}
	}
	logAccio << m_infoOrganismeActiu->descripcio() << "SentoI " << hex << nibble(4,parametres) << "=" << setw(8) << destiEnergia << " " << nibble(5,parametres) << "=???????? " << clauElement << "(" << ~tolerancia << ") " << dec << endl;
	// TODO: Be, com sempre aixo hauria de tenir un significat
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
	biosistema.carregaOpCodes("Opcodes.ini", error);
	out << "Inicialitzant Biotop..." << endl;
	biosistema.biotop(new CTopologiaToroidal<CBiosistema::t_substrat>(Config.get("Biotop/CassellesAmplitud"),Config.get("Biotop/CassellesAltitud")));
	biosistema.comunitat(new CComunitat);
	biosistema.agents(CAgent::ParsejaArxiu("Agents.ini", *(biosistema.biotop()), error));

	uint32 pasVisualitzacio=0;
	uint32 pas=pasVisualitzacio;

	enum {Blanc, Mapa, MapaOrganismes, Organismes} mode = MapaOrganismes;
	bool modeCanviat=true;
	CComparativaOrganismes graf1(biosistema.comunitat());
	CComparativaOrganismes graf2(biosistema.comunitat());
	CComparativaOrganismes graf3(biosistema.comunitat());
	CComparativaOrganismes graf4(biosistema.comunitat());
	CComparativaOrganismes graf5(biosistema.comunitat());
	CMapa mapa1(&biosistema);

	mapa1.primeraPosicio(0);
	bool helpWanted=true;
	bool editWanted=false;
	bool redisplayWanted=false;
	bool congelat=false;
	uint32 id=0;
	out << endl << "Biosistema carregat. Prem return per continuar..." << endl;
	cin.get();

	while (true) {
		// Actualitzem el mode de visualitzacio
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
				graf1.tamany(1,2,80,8);
				graf1.primerOrganisme(0);
				graf2.tamany(1,12,80,8);
				graf2.primerOrganisme(80);
				graf3.tamany(1,22,80,8);
				graf3.primerOrganisme(160);
				graf4.tamany(1,32,80,8);
				graf4.primerOrganisme(240);
				graf5.tamany(1,42,80,7);
				graf5.primerOrganisme(320);
			}
			modeCanviat=false;
			redisplayWanted=true;
		}

		// Fem un pas del biosistema
		if (!congelat) {
			biosistema();
			if (!biosistema.temps() && !pas--) {
				pas=pasVisualitzacio;
				redisplayWanted=true;
			}
		}

		// Visualitzem
		if (redisplayWanted) {
			redisplayWanted=false;
			if (mode==Mapa||mode==MapaOrganismes)
				mapa1.visualitza(out);
			if (mode==MapaOrganismes||mode==Organismes) {
				graf1.visualitza(out);
				graf2.visualitza(out);
				graf3.visualitza(out);
				graf4.visualitza(out);
				graf5.visualitza(out);
			}
			uint32 pos = mapa1.primeraPosicio();
			out << setfill('0');
			switch (mode)
			{
			case Blanc:
			case Mapa:
				out << gotoxy(42,1) 
					<< "Coords: " << setw(3) << pos%Config.get("Biotop/CassellesAmplitud") 
					<< '@' << setw(3) << pos/Config.get("Biotop/CassellesAmplitud") 
					;
				break;
			case MapaOrganismes:
				out << gotoxy(1,42) 
					<< "Coords: " << setw(3) << pos%Config.get("Biotop/CassellesAmplitud") 
					<< '@' << setw(3) << pos/Config.get("Biotop/CassellesAmplitud") 
					;
				break;
			}
			out << gotoxy(1, 1) << "Poblacio: " << setw(6) << biosistema.comunitat()->tamany();
			out << gotoxy(26,1) << "Temps: " << setw(8) << biosistema.m_temps;
			out << setfill(' ') << endl;
		}
		char filename[300];
		// Obtenim l'entrada del teclat
		if (kbhit())
		{
			cout << gotoxy(1,48) << clrlin << "Comanda: " << flush;
			bool sortir=false;
			while (!sortir)
			{
				char a = cin.get();
				if (!cin) break;
				switch (a) 
				{
				case 'D': mapa1.scrollPageRight(1); redisplayWanted=true; break;
				case 'd': mapa1.scrollRight(1); redisplayWanted=true; break;
				case 'A': mapa1.scrollPageLeft(1); redisplayWanted=true; break;
				case 'a': mapa1.scrollLeft(1); redisplayWanted=true; break;
				case 'W': mapa1.scrollPageUp(1); redisplayWanted=true; break;
				case 'w': mapa1.scrollUp(1); redisplayWanted=true; break;
				case 'S': mapa1.scrollPageDown(1); redisplayWanted=true; break;
				case 's': mapa1.scrollDown(1); redisplayWanted=true; break;
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
				case 'H': 
				case 'h': helpWanted=true; break;
				case 'L': modeCanviat = true; logAccio.desactiva(); break;
				case 'l': modeCanviat = true; logAccio.activa(); break;
				case 'R': 
					cin.get(filename,30);
					if (!cin.get(filename,30)) {
						cin.clear();
						break;
					}
					cin.clear();
					out << clrscr << endl;
					biosistema.deleteAgents();
					biosistema.agents(CAgent::ParsejaArxiu(filename, *(biosistema.biotop()), error));
					// TODO: Recarregar configuracio 
					out << "Prem una tecla" << endl;
					cin.get();
					modeCanviat = true;
					break;
				case 'r': 
					out << clrscr << endl;
					biosistema.deleteAgents();
					biosistema.agents(CAgent::ParsejaArxiu("Agents.ini", *(biosistema.biotop()), error));
					// TODO: Recarregar configuracio 
					out << "Prem una tecla" << endl;
					cin.get();
					modeCanviat = true;
					break;
				case 'e': biosistema.m_calEspeciar=true; break;
				case 'E': biosistema.m_calEspeciar=false; break;
				case 'V':
				case 'v':
					cin >> oct >> id >> dec;
					if (cin) editWanted=true;
					cin.clear();
					break;
				case 'p': congelat=true; break;
				case 'P': congelat=false; break;
				case 'J': 
				case 'j':
					cin >> pasVisualitzacio;
					if (cin) pas = pasVisualitzacio;
					cin.clear();
					break;
				case '\0':
					warning << "Valla, el \\0!!!" << endl;
					cin.get();
				case '\n': sortir=true;
					break;
				}
				// out << gotoxy(20,48) << "Tecla: " << '\'' << (a?a:'#') << '\''<< endl;
			}
		}
		if (helpWanted) {
			helpWanted=false;
			cout << clrscr <<
				"Ajuda de consola de Bioscena\n" 
				"****************************\n\n" 
				"Aquestes tecles son disposables durant l'execucio del prototip.\n"
				"Si prems qualsevol tecla/comanda el programa es para fins que prems el retorn.\n"
				"\nGeneral:\n\n"
				"  H      Per tornar a veure aquesta pantalla d'ajuda\n"
				"  Q      Surt del programa\n"
				"\nVisualitzacio:\n\n" 
				"  M      Visualitza el mapa (amb Shift ho treu)\n" 
				"  O      Visualitza comparativa organismes (amb Shift la treu)\n"
				"  L      Visualitza les accions dels organismes una per una (Amb Shift ho treu)\n"
				"  V+Num  Visualitza l'interior de l'organisme especificat\n"
				"  J+Num  Es salta 'Num' pasos entre visualitzacio i visualitzacio\n"
				"\nNavegacio Mapa:\n\n"
				"  W      1 posicio adalt (amb Shift 1 pantalla sencera)\n"
				"  S      1 posicio abaix (amb Shift 1 pantalla sencera)\n"
				"  D      1 posicio a la dreta (amb Shift 1 pantalla sencera)\n"
				"  A      1 posicio a l'esquerra (amb Shift 1 pantalla sencera)\n"
				"\nActuacio sobre el biosistema:\n\n"
				"  R      Torna a carregar el fitxer de configuracio d'agents externs\n"
				"  E      Diferencia els colors dels organismes mutants (amb Shift es desactiva)\n"
				"  P      Congela el biosistema (amb Shift ho descongela)\n"
				"\n  Premi return per a continuar amb el programa\n"
				"\n  Copirait KKEPerians UNLTD\n"
				<< endl
				;
			cin.get();
			modeCanviat = true;
		}
		if (editWanted) {
			editWanted=false;
			cout << clrscr << endl;
			if (biosistema.comunitat()->esValid(id)) {
				COrganisme * org = (*biosistema.comunitat())[id].cos();
				uint32 pos = (*biosistema.comunitat())[id].posicio();
				out << (*biosistema.comunitat())[id].descripcio() << '\t' 
					<<  pos%Config.get("Biotop/CassellesAmplitud")<< '@' <<  pos/Config.get("Biotop/CassellesAmplitud") 
					<< " Edat:" << (*biosistema.comunitat())[id].cos()->edat()
					<< " Energia: " << (*biosistema.comunitat())[id].cos()->energia()
					<< endl;
				org->dump(out);
			}
			else cout << "L'organisme ha mort o mai ha existit" << endl;
			cin.get();
			modeCanviat = true;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Implementacio
//////////////////////////////////////////////////////////////////////

uint32 CBiosistema::temps()
{
	return m_tempsPerAccionarAgents;

}
