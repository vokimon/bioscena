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
// 20000104 VoK - Impronta: el pare modifica un registre de fenotip del fill
// 20000215 VoK - La feina de taxonomista es fa amb l'objecte extern
// 20000527 VoK - Factor de carrega pel moviment
// 20000708 VoK - Es pot introduir un organisme a una posicio concreta
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
#include "Biosistema.h"
#include "Missatger.h"
#include "Configuracio.h"
#include "Compatibilitat.h"

using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Variables estatiques
//////////////////////////////////////////////////////////////////////

static CMemoryOutputer unLog(clog, 40);
//static CColorOutputer unLog(clog);
static CMissatger logAccio("Accions realitzades", NULL, unLog);

//////////////////////////////////////////////////////////////////////
// Funcions Auxiliars (Locals al modul)
//////////////////////////////////////////////////////////////////////

struct COpcodeInfo {
	const char *mnemonic;
	CBiosistema::t_accioOrganisme fn;
	const char *param[8];
	};

COpcodeInfo opcodes [] = {
	{"NoOperacio", &CBiosistema::organismeNoOperacio, {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}},
	{"Avanca", &CBiosistema::organismeAvanca, {"dir","ener","clau",NULL,NULL,NULL,NULL,NULL}},
	{"Mitosi", &CBiosistema::organismeMitosi, {"dir","base","tol","ener","nutr","regH",NULL,NULL}},
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
	{"Catabol", &CBiosistema::organismeCatabolitza, {"patA","tolA","clau","desB","desC",NULL,NULL,NULL}},
	{NULL, NULL, {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}},
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBiosistema::CBiosistema()
	: m_opcodes(NULL)
	, _opcodeIndexes(0)
{
	m_biotop = NULL;
	m_comunitat = NULL;
	m_agents = NULL;
	m_taxonomista = NULL;
	logAccio.desactiva();

	m_maxInstruccionsUtils=Config.get("Biosistema/Quantum/Utils");
	m_maxInstruccions=Config.get("Biosistema/Quantum/Maxim");
	m_instruccionsUtilsRestants=0;
	m_instruccionsRestants=0;
	m_organismeActiu=NULL;
	m_tempsPerAccionarAgents=0;
	m_temps=0;

	m_probabilitatGeneracioExpontanea.set(
		Config.get("Comunitat/ProbabilitatGeneracioExpontanea/Encerts"),
		Config.get("Comunitat/ProbabilitatGeneracioExpontanea/Mostra")
		);

	m_bitsCodiOperacio=Config.get("Biosistema/OpCodes/BitsOperacio");
	m_nCodisOperacio=1<<m_bitsCodiOperacio;
	m_mascaraCodis = m_nCodisOperacio-1;

	m_mascaraQuimica = Config.get("Biosistema/Metabolisme/BitsSignificatius");
}

CBiosistema::~CBiosistema()
{
	if (m_biotop) delete m_biotop;
	if (m_comunitat) delete m_comunitat;
	if (m_agents) delete m_agents;
	if (m_taxonomista) delete m_taxonomista;
	if (m_opcodes==NULL) delete [] m_opcodes;
}

//////////////////////////////////////////////////////////////////////
// Activacio/Pasivacio
//////////////////////////////////////////////////////////////////////

istream & CBiosistema::load(istream & str)
{
	m_biotop->load(str);
	m_comunitat->load(str);
	m_taxonomista->load(str);
	return str;
}

ostream & CBiosistema::store(ostream & str)
{
	m_biotop->store(str);
	m_comunitat->store(str);
	m_taxonomista->store(str);
	return str;
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

CBiosistema::t_taxonomista * CBiosistema::taxonomista() const
{
	return m_taxonomista;
}

void CBiosistema::taxonomista(t_taxonomista * taxonomista)
{
	if (!m_taxonomista)
		m_taxonomista=taxonomista;
}

void CBiosistema::deleteTaxonomista()
{
	if (m_taxonomista)
		delete m_taxonomista;
	m_taxonomista=NULL;
}

//////////////////////////////////////////////////////////////////////
// Operacions (Inicialitzacio)
//////////////////////////////////////////////////////////////////////

std::string CBiosistema::operationDescriptor(uint32 operation)
{
	std::cout <<"operation: "  << operation << " " << _opcodeIndexes.get() << std::endl;
	uint32 opcodeIdx = _opcodeIndexes.get()? _opcodeIndexes[operation]:0;
	std::cout << "opcode idx: " << opcodeIdx << std::endl;
	return opcodes[opcodeIdx].mnemonic;
}

void CBiosistema::addOperation(uint32 opcode, const std::string & mnemonic)
{
	if (opcode>=m_nCodisOperacio) {
		std::ostringstream message;
		message
			<< "Operation opcode 0x" << hex << opcode << dec
			<< " cannot be represented with " << m_bitsCodiOperacio
			<< " bits" << ends;
		throw OpcodeConfigError(message.str());
	}

	uint32 i;
	for (i=0; opcodes[i].mnemonic && mnemonic!=opcodes[i].mnemonic; i++);
	if (!opcodes[i].mnemonic) {
		throw OpcodeConfigError(
			"Mnemonic '"+mnemonic+"' does not exist");
	}
	m_opcodes[opcode]=opcodes[i].fn;
	_opcodeIndexes[opcode]=i;
}

void CBiosistema::clearOpcodes() {
	if (m_opcodes) delete [] m_opcodes;
	m_opcodes = new t_accioOrganisme[m_nCodisOperacio];
	_opcodeIndexes.reset(new uint32[m_nCodisOperacio]);
	for (uint32 i=m_nCodisOperacio; i--;) {
		m_opcodes[i]= (t_accioOrganisme) NULL;
		_opcodeIndexes[i]= 0u;
	}
}

bool CBiosistema::carregaOpCodes(const std::string & nomArxiu, CMissatger & errors)
{
	// Obrim l'arxiu
	std::ifstream entrada(nomArxiu);
	out << "Carregant la taula de codis d'operacions [" << nomArxiu << "]" << endl;
	if (!entrada) {
		errors 
			<< "Error obrint '" << nomArxiu << '\'' << endl;
		return false;
	}
	bool result = carregaOpCodes(entrada, errors);
	out << "Taula codis d'operacio [" << nomArxiu << "] carregada" << endl;
	return result;
}

bool CBiosistema::carregaOpCodes(std::istream & entrada, CMissatger & errors)
{
	clearOpcodes();

	string prefetch;
	entrada >> prefetch;
	while (entrada && prefetch=="*") {	
		uint32 valor;
		string mnemonicOperacio;
		entrada >> hex >> valor >> dec >> mnemonicOperacio >> prefetch;
		// TODO: Esborrar aquesta traca (o no)
		out << '\t' << hex << valor << dec << '\t' << mnemonicOperacio << endl;
		addOperation(valor, mnemonicOperacio);
	}
	if (entrada) throw OpcodeConfigError(
		"Unexpected '"+prefetch+"' in opcode configuration");

	// Comprovacions a posteriori
	for (uint32 i=0; i<m_nCodisOperacio; i++)
		if (m_opcodes[i]==NULL) {
			warning <<
				"Codi " << i << " no assignat a cap operacio; per defecte: NoOperacio" << endl;
			m_opcodes[i]=&CBiosistema::organismeNoOperacio;
		}

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
		for (uint32 i=300; --i; ) {
			bool wasAdded = organismeExpontani();
			if (not wasAdded) continue;
			if (not m_comunitat->tamany()) continue;
			break;
		}
		logAccio << magenta.brillant() << "Biosistema: " << blanc.fosc() << "Repoblat" << endl;
	}
	else if (m_probabilitatGeneracioExpontanea.chance())
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
	uint32 pos = m_biotop->randomPosition();
	if ((*m_biotop)[pos].esOcupat()) {
		/*
		error << "Deixant un organisme en una posicio ocupada " << pos 
			<< " hi ha " << m_comunitat->tamany()
			<< " de " << m_biotop->size() << "."  << std::endl;
		*/
		return false;
	}
	// Creem un taxo per l'ancestre
	uint32 taxo = m_taxonomista ? m_taxonomista->nouTaxoIndependent(): ~uint32(0);
	// 
	COrganisme * org = new COrganisme("ControlOrganisme/ExpressioGenica");
	if (!org) {
		error << "CBiosistema: Fallo la memoria ocupando un organismo expontaneo" << endl;
		cin.get();
		return false;
	}
	// Li posem l'energia inicial
	org->guanyaEnergia(Config.get("Biosistema/Energia/InicialExpontani"));
	// Ho introduim en societat (a la comunitat i al biotop)
	uint32 id = m_comunitat->introdueix(org, pos, taxo);
	(*m_biotop)[pos].ocupa(id);
	// Logging
	logAccio << verd.brillant() << "Generat expontaneament" << blanc.fosc() << (*m_comunitat)[id].descripcio() << endl;
	return true;
}

bool CBiosistema::eliminaOrganismeActiu()
{
	logAccio << m_infoOrganismeActiu->descripcio() << flush << blau.brillant() << "Defuncio" << blanc.fosc() << endl;

	// Alliberem la posicio
	uint32 pos = m_infoOrganismeActiu->posicio();
	(*m_biotop)[pos].desocupa();

	// Informem al taxonomista
	if (m_taxonomista) {
		uint32 taxo = m_infoOrganismeActiu->taxo();
		m_taxonomista->defuncio(taxo);
	}

	// El desterrem de la comunitat
	m_comunitat->extreu(m_idOrganismeActiu);
	(*m_comunitat)[m_idOrganismeActiu].taxo(0); // Aixo indica que ha estat borrat adecuadament
	// TODO: Pensar-se si posar el taxo a 0 es la forma mes elegant de declarar el InfoOrg esborrat
	return true;
}

bool CBiosistema::introdueix(COrganisme * org, uint32 pos /* =-1 */)
// Pre: org es un punter vàlid
{
	if (!m_biotop->isValidPosition(pos)) {
		pos = m_biotop->randomPosition();
		}
	// Escollim una posicio aleatoria, si esta lliure continuem
	if ((*m_biotop)[pos].esOcupat()) {
		delete org;
		return false;
		}
	// Creem un taxo per l'ancestre
	uint32 taxo = m_taxonomista ? m_taxonomista->nouTaxoIndependent(): ~uint32(0);
	// Ho introduim en societat (a la comunitat i al biotop)
	uint32 id = m_comunitat->introdueix(org, pos, taxo);
	(*m_biotop)[pos].ocupa(id);
	// Logging
	logAccio << verd.brillant() << "Introduint " << blanc.fosc() << (*m_comunitat)[id].descripcio() << endl;
	return true;
}

//////////////////////////////////////////////////////////////////////
// Operacions (Execucio d'intruccions)
//////////////////////////////////////////////////////////////////////

bool CBiosistema::organismeMitosi(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 desp        = fenotip(parametres,0);
	uint32 elementBase = fenotip(parametres,1)&m_mascaraQuimica;
	uint32 tolerancia  = fenotip(parametres,2)|~m_mascaraQuimica;
	uint32 energia     = fenotip(parametres,3)&0x00ff;
	uint32 nutrients   = fenotip(parametres,4)&0x00ff;
	uint32 regHeredat  = nibble(5,parametres);
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti  = m_biotop->displace(posOrigen, desp);
	CSubstrat & substratDesti=(*m_biotop)[posDesti];

	// Fem pagar al futur pare la penalitzacio per cromosoma llarg
	uint32 tamanyPenat = Config.get("Organisme/Cariotip/PenalitzacioLlarg/BitsTamanyMaxim");
	uint32 factorPenat = Config.get("Organisme/Cariotip/PenalitzacioLlarg/Factor");
	m_organismeActiu->consumeixEnergia((m_organismeActiu->m_cariotip.tamanyCodons()>>tamanyPenat)*factorPenat);

	// Inici del log
	logAccio << m_infoOrganismeActiu->descripcio() << "Mitosi " << hex << nibble(0,parametres) << ":" << setw(8) << desp << dec << ": ";

	// Comprovem el minim d'energia requerit
	if (m_organismeActiu->energia()<Config.get("Biosistema/Energia/Mitosi/Minima")) {
		m_organismeActiu->consumeixEnergia(Config.get("Biosistema/Energia/Mitosi/CostNoMinima"));
		logAccio << vermell << "No tinc l'energia minima" << blanc.fosc() << endl;
		return false;
		}
	// Mirem si encara tenim energia per fer la mitosi
	if (!m_organismeActiu->consumeixEnergia(energia/*Config.get("Biosistema/Energia/Mitosi/Cedida")*/ *Config.get("Biosistema/Energia/Mitosi/Factor"))) {
		logAccio << vermell << "No tinc energia" << blanc.fosc() << endl;
		return false;
		}
	// Comprovem que la posició del futur nado està buida
	if (substratDesti.esOcupat()) {
		logAccio << vermell.brillant() << "Ocupat " << blanc.fosc() << (*m_comunitat)[substratDesti.ocupant()].descripcio() << endl;
		return false; // Error: Ja hi ha penya a la posicio, no la podem ocupar
	}
	// Benvingut al mon!!!
	COrganisme * nouOrganisme = new COrganisme(m_organismeActiu->m_genotip->tipus(), m_organismeActiu->m_cariotip);
	if (!nouOrganisme) {
		logAccio << vermell.brillant() << "Falta memoria" << blanc.fosc() << endl;
		cin.get();
		return false;
	}
	// L'inscribim al registre de naixements (taxonomista)
	uint32 taxo = m_taxonomista ? m_taxonomista->naixement(m_infoOrganismeActiu->taxo(),nouOrganisme->m_mutat) : ~uint32(0);
	
	// Omplim el nado amb energia i nutrients
	nouOrganisme->guanyaEnergia(energia); // Config.get("Biosistema/Energia/Mitosi/Cedida")
	while (nutrients--) {
		uint32 element;
		if (m_organismeActiu->excreta(element, elementBase, tolerancia))
			nouOrganisme->engoleix(element);
	}
	// Copiem el registre heredat
	uint32 valHeredat = (*nouOrganisme)[regHeredat] = (*m_organismeActiu)[regHeredat];
	// Introduim el nado en comunitat
	uint32 id = m_comunitat->introdueix(nouOrganisme, posDesti, taxo);
	// Restaurem el punter que probablement haura quedat invalidat en introduir el nado
	m_infoOrganismeActiu = &((*m_comunitat)[m_idOrganismeActiu]);
	substratDesti.ocupa(id);
	logAccio << verd.brillant() << "Nascut " << blanc.fosc() << (*m_comunitat)[id].descripcio() << ' ' << hex << regHeredat << '(' << valHeredat << dec << ')' << endl;
	// Logging adicional (i provisional)
	if (taxo!=m_infoOrganismeActiu->taxo()) {
		logAccio << "Eps! Mutacio " << m_infoOrganismeActiu->taxo() << "->" << taxo << endl;
	}
	// TODO: Logs
	return true;
}

bool CBiosistema::organismeAvanca(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 desp =         fenotip(parametres,0);
	uint32 energia =      fenotip(parametres,1);
	uint32 clauMoviment = fenotip(parametres,2);
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti  = m_biotop->displace(posOrigen, desp);
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
	uint32 energiaGastada = Config.get("Biosistema/Energia/Moviment/Fixe") +
		(m_organismeActiu->carrega()/Config.get("Biosistema/Energia/Moviment/UnitatDeCarrega"))*
		m_organismeActiu->carrega()/Config.get("Biosistema/Energia/Moviment/CostUnitatDeCarrega");
	m_organismeActiu->consumeixEnergia(energiaGastada);
	(*m_comunitat)[m_idOrganismeActiu].posicio(posDesti);
	logAccio << groc.brillant() << "Desti " << setw(4) << posDesti << " Ok" << blanc.fosc() << endl;
	// TODO: Complicar la logica: Cost de displace, camí lliure, corrents...
	// TODO: Logs
	return true;
}


bool CBiosistema::organismeAtaca(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 desp=       fenotip(parametres,0);
	uint32 elementBase=fenotip(parametres,1)&m_mascaraQuimica;
	uint32 tolerancia= fenotip(parametres,2)|~m_mascaraQuimica;
	uint32 clauAtac=   fenotip(parametres,3);
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti = m_biotop->displace(posOrigen, desp);
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
	uint32 desp=	   fenotip(parametres,0);
	uint32 elementBase=fenotip(parametres,1)&m_mascaraQuimica;
	uint32 tolerancia= fenotip(parametres,2)|~m_mascaraQuimica;
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti = m_biotop->displace(posOrigen, desp);
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
	uint32 desp=	   fenotip(parametres,0);
	uint32 elementBase=fenotip(parametres,1)&m_mascaraQuimica;
	uint32 tolerancia= fenotip(parametres,2)|~m_mascaraQuimica;
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti = m_biotop->displace(posOrigen, desp);
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

/// Realitza la reaccio A + B + energia -> C
bool CBiosistema::organismeAnabolitza(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 patroA      = fenotip(parametres,0)&m_mascaraQuimica;
	uint32 patroB      = fenotip(parametres,1)&m_mascaraQuimica;
	uint32 toleranciaA = fenotip(parametres,2)|~m_mascaraQuimica;
	uint32 toleranciaB = fenotip(parametres,3)|~m_mascaraQuimica;
	uint32 &C          = fenotip(parametres,4);


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

/// Realitza la reaccio A -> B + C + energia
bool CBiosistema::organismeCatabolitza(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 patroA         = fenotip(parametres,0)&m_mascaraQuimica;
	uint32 toleranciaA    = fenotip(parametres,1)|~m_mascaraQuimica;
	uint32 clauCatabolica = fenotip(parametres,2)|~m_mascaraQuimica;
	uint32 &B  = fenotip(parametres,3);
	uint32 &C  = fenotip(parametres,4);

	logAccio << m_infoOrganismeActiu->descripcio() << "Catab  " << hex << nibble(3,parametres) << "," << nibble(4,parametres) << "=" << setw(8) << patroA << ">" << nibble(2,parametres) << ":" << clauCatabolica << dec << ": ";

	uint32 A;
	if (!m_organismeActiu->excreta(A, patroA, toleranciaA)) {
		logAccio << vermell.brillant() << "No tinc " << hex << patroA << "(" << ~toleranciaA << ")" << dec << blanc.fosc() << endl;
		return false; // No he pogut trobar el primer reactiu
	}
	
	B=A&clauCatabolica;
	C=A&~clauCatabolica;
	m_organismeActiu->engoleix(C);
	m_organismeActiu->engoleix(C);
	uint32 energia=comptaUns(A)- max(comptaUns(B),comptaUns(C));
	logAccio << groc.brillant() << hex << A << "->" << B << "+" << C << dec << "+E(" << energia << ")" << blanc.fosc() << endl;
	m_organismeActiu->guanyaEnergia(energia);

	return true;
}

bool CBiosistema::organismeAnd(uint32 parametres)
{
	fenotip(parametres,0) =
	fenotip(parametres,1) &
	fenotip(parametres,2) ;
	logAccio << m_infoOrganismeActiu->descripcio() << "And    " << hex << nibble(0,parametres) << "=" << setw(8) << fenotip(parametres,0) << " " << nibble(1,parametres) << "&" << nibble(2,parametres) << dec << endl;
	return true;
}

bool CBiosistema::organismeOr(uint32 parametres)
{
	fenotip(parametres,0) =
	fenotip(parametres,1) |
	fenotip(parametres,2) ;
	logAccio << m_infoOrganismeActiu->descripcio() << "Or     " << hex << nibble(0,parametres) << "=" << setw(8) << fenotip(parametres,0) << " " << nibble(1,parametres) << "|" << nibble(2,parametres) << dec << endl;
	return true;
}

bool CBiosistema::organismeXor(uint32 parametres)
{
	fenotip(parametres,0) =
	fenotip(parametres,1) ^
	fenotip(parametres,2) ;
	logAccio << m_infoOrganismeActiu->descripcio() << "Xor    " << hex << nibble(0,parametres) << "=" << setw(8) << fenotip(parametres,0) << " " << nibble(1,parametres) << "^" << nibble(2,parametres) << dec << endl;
	return true;
}

bool CBiosistema::organismeShiftRight(uint32 parametres)
{
	fenotip(parametres,0) =
	fenotip(parametres,1) >>
	nibble(2,parametres);
	logAccio << m_infoOrganismeActiu->descripcio() << "ShiftR " << hex << nibble(0,parametres) << "=" << setw(8) << fenotip(parametres,0) << " " << nibble(1,parametres) << ">>" << dec << setw(2) << nibble(2,parametres) << endl;
	return true;
}

bool CBiosistema::organismeShiftLeft(uint32 parametres)
{
	fenotip(parametres,0) =
	fenotip(parametres,1) <<
	nibble(2,parametres);
	logAccio << m_infoOrganismeActiu->descripcio() << "ShiftL " << hex << nibble(0,parametres) << "=" << setw(8) << fenotip(parametres,0) << " " << nibble(1,parametres) << "<<"<< dec << setw(2) << nibble(2,parametres) << endl; 
	return true;
}

bool CBiosistema::organismeNot(uint32 parametres)
{
	fenotip(parametres,0) =
	~ fenotip(parametres,1) ;
	logAccio << m_infoOrganismeActiu->descripcio() << "Negat  " << hex << nibble(0,parametres) << "=" << setw(8) << fenotip(parametres,0) << " ~" << nibble(1,parametres) << dec << endl;
	return true;
}

bool CBiosistema::organismeOposa(uint32 parametres)
// Serveix per obtenir la direccio oposada
{
	fenotip(parametres,0) =
	fenotip(parametres,1) ^ 0x77777777;
	logAccio << m_infoOrganismeActiu->descripcio() << "Oposa  " << hex << nibble(0,parametres) << "=" << setw(8) << fenotip(parametres,0) << " " << nibble(1,parametres) << "^77777777" << dec << endl;
	return true;
}

bool CBiosistema::organismeRandom(uint32 parametres)
{
	fenotip(parametres,0) = rnd.get();
	logAccio << m_infoOrganismeActiu->descripcio() << "Random " << hex << nibble(0,parametres) << "=" << setw(8) << fenotip(parametres,0) << dec << endl;
	return true;
}

bool CBiosistema::organismeCopia(uint32 parametres)
{
	fenotip(parametres,0) =
	fenotip(parametres,1);
	logAccio << m_infoOrganismeActiu->descripcio() << "Copy   " << hex << nibble(0,parametres) << "=" << setw(8) << fenotip(parametres,0) << " " << nibble(1,parametres) << dec << endl;
	return true;
}

bool CBiosistema::organismeCarrega(uint32 parametres)
{
	fenotip(parametres,0) =
	m_organismeActiu->seguentInstruccio();
	logAccio << m_infoOrganismeActiu->descripcio() << "Load   " << hex << nibble(0,parametres) << "=" << setw(8) << fenotip(parametres,0) << dec << endl;
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
	uint32 direccio        = fenotip(parametres,0);
	uint32 radi            = fenotip(parametres,1)&31;
	uint32 clauElement     = fenotip(parametres,2)&m_mascaraQuimica;
	uint32 tolerancia      = fenotip(parametres,3)|~m_mascaraQuimica;
	uint32 & destiDireccio = fenotip(parametres,4);
	uint32 & destiTipus    = fenotip(parametres,5);
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posBase = m_biotop->displace(posOrigen, direccio);

	for (uint32 i=Config.get("Sensor/Quimic/Intents"); i--;) {
		uint32 posDesti = m_biotop->displace (posBase, radi);
		if ((*m_biotop)[posDesti].rastreja(destiTipus, clauElement, tolerancia)) {
			// TODO: Aixo retorna true si hi arribes nomes amb el displace. Ho fem servir?
			m_biotop->wayTo(posOrigen,posDesti,destiDireccio);
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
	uint32 direccio        = fenotip(parametres,0);
	uint32 radi            = fenotip(parametres,1)&31;
	uint32 clauElement     = fenotip(parametres,2);
	uint32 tolerancia      = fenotip(parametres,3);
	uint32 & destiDireccio = fenotip(parametres,4);
	uint32 & destiTipus    = fenotip(parametres,5);
	uint32 registre        = nibble(7,parametres);
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posBase = m_biotop->displace(posOrigen, direccio);

	for (uint32 i=Config.get("Sensor/Presencia/Intents"); i--;) {
		uint32 posDesti = m_biotop->displace (posBase, radi);
		if ((*m_biotop)[posDesti].esOcupat()) {
			uint32 ocupant=(*m_biotop)[posDesti].ocupant();
			if (sonCompatibles((*(*m_comunitat)[ocupant].cos())[registre],clauElement,tolerancia)) {
				// TODO: Aixo retorna true si hi arribes nomes amb el displace. Ho fem servir?
				m_biotop->wayTo(posOrigen,posDesti,destiDireccio);
				logAccio << m_infoOrganismeActiu->descripcio() << "SentoP " << hex << nibble(4,parametres) << "=" << setw(8) << destiDireccio << " " << nibble(5,parametres) << "=" << setw(8) << destiTipus << " dir(" << nibble(0,parametres) << ") tq " << nibble(7,parametres) << "==R" << nibble(2,parametres) << "(R" << nibble(3,parametres) << ")" << dec << endl; 
				return true;
			}
		}
	}
	logAccio << m_infoOrganismeActiu->descripcio() << "SentoP " << hex << nibble(4,parametres) << "=???????? " << nibble(5,parametres) << "=???????? dir(" << nibble(0,parametres) << ") tq " << nibble(7,parametres) << "==R" << nibble(2,parametres) << "(R" << nibble(3,parametres) << ")" << dec << endl; 

	// TODO: Be, com sempre aixo hauria de tenir un significat
	return true;
}

uint32 & CBiosistema::fenotip(uint32 params, uint32 n) {
	return (*m_organismeActiu)[nibble(n,params)];
}

bool CBiosistema::organismeSensorIntern(uint32 parametres)
{
	// TODO: SensorIntern no esta implementada encara
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 clauElement     = fenotip(parametres,2)&m_mascaraQuimica;
	uint32 tolerancia      = fenotip(parametres,3)|~m_mascaraQuimica;
	uint32 & destiEnergia  = fenotip(parametres,4);
	uint32 & destiTipus    = fenotip(parametres,5);

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
// Implementacio
//////////////////////////////////////////////////////////////////////

uint32 CBiosistema::temps()
{
	return m_tempsPerAccionarAgents;

}

void CBiosistema::activaLog(bool actiu)
{
	if (actiu) logAccio.activa();
	else logAccio.desactiva();
}

