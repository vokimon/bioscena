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
// El namespace es per solucionar un conflicte amb el gotoxy de AnsiCodes.h
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
// TODO: Cal canviar aixo, quan tinguem taxonomista
static uint32 ultimTaxo=0;

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

	m_bitsCodiOperacio=Config.get("Biosistema/OpCodes/BitsOperacio");
	m_nCodisOperacio=1<<m_bitsCodiOperacio;
	m_mascaraCodis = m_nCodisOperacio-1;
	m_opcodes = NULL;
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
	// Obrim l'arxiu
	ifstream entrada(nomArxiu);
	string prefetch;
	string mnemonicOperacio;
	uint32 valor;

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
		if      (mnemonicOperacio=="Avanca")
			m_opcodes[valor]=&CBiosistema::organismeAvanca;
		else if (mnemonicOperacio=="Mitosi")
			m_opcodes[valor]=&CBiosistema::organismeMitosi;
		else if (mnemonicOperacio=="Ataca")
			m_opcodes[valor]=&CBiosistema::organismeAtaca;
		else if (mnemonicOperacio=="Engoleix")
			m_opcodes[valor]=&CBiosistema::organismeEngoleix;
		else if (mnemonicOperacio=="Excreta")
			m_opcodes[valor]=&CBiosistema::organismeExcreta;
		else if (mnemonicOperacio=="NoOperacio")
			m_opcodes[valor]=&CBiosistema::organismeNoOperacio;
		else if (mnemonicOperacio=="And")
			m_opcodes[valor]=&CBiosistema::organismeAnd;
		else if (mnemonicOperacio=="Or")
			m_opcodes[valor]=&CBiosistema::organismeOr;
		else if (mnemonicOperacio=="Xor")
			m_opcodes[valor]=&CBiosistema::organismeXor;
		else if (mnemonicOperacio=="Not")
			m_opcodes[valor]=&CBiosistema::organismeNot;
		else if (mnemonicOperacio=="Oposa")
			m_opcodes[valor]=&CBiosistema::organismeOposa;
		else if (mnemonicOperacio=="Random")
			m_opcodes[valor]=&CBiosistema::organismeRandom;
		else if (mnemonicOperacio=="Copia")
			m_opcodes[valor]=&CBiosistema::organismeCopia;
		else if (mnemonicOperacio=="Carrega")
			m_opcodes[valor]=&CBiosistema::organismeCarrega;
		else if (mnemonicOperacio=="Localitza")
			m_opcodes[valor]=&CBiosistema::organismeLocalitza;
		else if (mnemonicOperacio=="Identifica")
			m_opcodes[valor]=&CBiosistema::organismeIdentifica;
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
	bool bUtil = m_opcodes && (this->*m_opcodes[instr&m_mascaraCodis] )(instr>>m_bitsCodiOperacio);

	// Actualitzacio del quantum i l'energia (l'energia tb es modifica en altres llocs)
	if (bUtil) m_instruccionsUtilsRestants--;
	else m_organismeActiu->consumeix(Config.get("Biosistema/Energia/AdicionalInutil"));
	m_instruccionsRestants--;
	m_organismeActiu->consumeix(Config.get("Biosistema/Energia/FixeInstruccio"));

	// TODO: Treure modificacio del foo. 
	// Es per asegurar-nos de que la memoria de l'organisme es nostra. Si no ho es, casca.
	m_organismeActiu->m_foo++;
}

void CBiosistema::canviaOrganismeActiu()
// Post: Agafa un organisme valid com a actiu. (Si no n'hi ha el crea)
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
	// Ho introduim en societat (a la comunitat i al biotop)
	uint32 id = m_comunitat->introdueix(org, pos, taxo);
	(*m_biotop)[pos].ocupa(id);
	// Logging
	logAccio << verd.brillant() << "Generat expontaneament" << blanc.fosc() << endl;
	// Registrem el nou 'primigeni'
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

//////////////////////////////////////////////////////////////////////
// Operacions (Execucio d'intruccions)
//////////////////////////////////////////////////////////////////////

bool CBiosistema::organismeMitosi(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 desp =    (*m_organismeActiu)[(parametres>>0)&31];
	uint32 energia = (*m_organismeActiu)[(parametres>>5)&31];
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti  = m_biotop->desplacament(posOrigen, desp);
	CSubstrat & substratOrigen=(*m_biotop)[posOrigen];
	CSubstrat & substratDesti=(*m_biotop)[posDesti];
	logAccio << m_infoOrganismeActiu->descripcio() << "Mitosi " << setw(2) << ((parametres>>0)&31) << ":" << hex << setw(8) << desp << dec << ": ";
	if (substratDesti.esOcupat()) {
		logAccio << vermell.brillant() << "Ocupat " << blanc.fosc() << (*m_comunitat)[substratDesti.ocupant()].descripcio() << endl;
		return false; // Error: Ja hi ha penya a la posicio, no la podem ocupar
	}
	COrganisme * nouOrganisme = new COrganisme(m_organismeActiu->m_cariotip);
	if (!nouOrganisme) {
		logAccio << verd.brillant() << "Falta memoria" << blanc.fosc() << endl;
		cin.get();
		return false;
	}
	// TODO: Cal canviar aixo, quan tinguem taxonomista
	uint32 taxo;
	if (nouOrganisme->m_mutat)
	{
		if (!(ultimTaxo&070)) ultimTaxo+=010;
		taxo = ultimTaxo++;
	}
	else 
		taxo = m_infoOrganismeActiu->taxo();
	uint32 id = m_comunitat->introdueix(nouOrganisme, posDesti, taxo);
	substratDesti.ocupa(id);
	m_organismeActiu->consumeix(Config.get("Biosistema/Energia/Mitosi"));

	// Restaurem el punter que probablement haura quedat invalidat
	m_infoOrganismeActiu = &((*m_comunitat)[m_idOrganismeActiu]);

	// Logging
	logAccio << verd.brillant() << "Nascut " << blanc.fosc() << setw(2) << (*m_comunitat)[id].descripcio() << endl;
	if (taxo!=m_infoOrganismeActiu->taxo()) {
		logAccio << "Eps! Mutacio " /*<< m_infoOrganismeActiu->taxo() << "->" << taxo*/ << endl;
	}
	// TODO: Logs
	return true;
}

bool CBiosistema::organismeAvanca(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 desp =         (*m_organismeActiu)[(parametres>> 0)&31];
	uint32 energia =      (*m_organismeActiu)[(parametres>> 5)&31];
	uint32 clauMoviment = (*m_organismeActiu)[(parametres>>10)&31];
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti  = m_biotop->desplacament(posOrigen, desp);
	logAccio << m_infoOrganismeActiu->descripcio() << "Em moc " << setw(2) << ((parametres>>0)&31) << ":" << hex << setw(8) << desp << dec << ": ";
	CSubstrat & substratOrigen=(*m_biotop)[posOrigen];
	CSubstrat & substratDesti=(*m_biotop)[posDesti];
	if (substratDesti.esOcupat()) {
		logAccio << vermell.brillant() << "Ocupat " << blanc.fosc() << (*m_comunitat)[substratDesti.ocupant()].descripcio() << endl;
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


bool CBiosistema::organismeAtaca(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 desp=       (*m_organismeActiu)[(parametres>> 0)&31];
	uint32 elementBase=(*m_organismeActiu)[(parametres>> 5)&31]&3;
	uint32 tolerancia= (*m_organismeActiu)[(parametres>>10)&31]&3;
	uint32 energia=    (*m_organismeActiu)[(parametres>>15)&31]&7;
	uint32 clauAtac=   (*m_organismeActiu)[(parametres>>20)&31];
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti = m_biotop->desplacament(posOrigen, desp);
	logAccio << m_infoOrganismeActiu->descripcio() << "Ataco " << setw(2) << ((parametres>>0)&31) << ":" << hex << setw(8) << desp << dec /*<< setw(4) << posDesti*/ << ": ";
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

bool CBiosistema::organismeEngoleix(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 desp=	   rnd.get()  ;//(*m_organismeActiu)[(parametres>> 0)&31];
	uint32 elementBase=(parametres>> 5)&3;
	uint32 tolerancia= (parametres>>10)&3;
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti = m_biotop->desplacament(posOrigen, desp);
	logAccio << m_infoOrganismeActiu->descripcio() << "Engolo " << setw(2) << ((parametres>>0)&31) << ":" << hex << setw(8) << desp << dec /*<< setw(4) << posDesti*/ << ": ";
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

bool CBiosistema::organismeExcreta(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 desp=	   (*m_organismeActiu)[(parametres>> 0)&31];
	uint32 elementBase=(*m_organismeActiu)[(parametres>> 5)&31]&3;
	uint32 tolerancia= (*m_organismeActiu)[(parametres>>10)&31]&3;
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posDesti = m_biotop->desplacament(posOrigen, desp);
	logAccio << m_infoOrganismeActiu->descripcio() << "Excreto " << setw(2) << ((parametres>>0)&31) << ":" << hex << setw(8) << desp << dec /*<< setw(4) << posDesti*/ << ": ";
	CSubstrat & substratDesti = (*m_biotop)[posDesti];
	uint32 element;
	if (!(*m_comunitat)[m_idOrganismeActiu].cos()->excreta(element, elementBase, tolerancia)) {
		logAccio << vermell.brillant() << "No tinc " << hex << elementBase << "(" << tolerancia << ")" << dec << blanc.fosc() << endl;
		return false; // Error: No hi ha res que excretar
	}
	logAccio << groc.brillant() << "Deposito un " << element << blanc.fosc() << endl;
	substratDesti.deposita(element);
//	logAccio << "Ja he excretat" << endl;
	return true;
}	

bool CBiosistema::organismeAnd(uint32 parametres)
{
	(*m_organismeActiu)[(parametres>> 0)&31] =
	(*m_organismeActiu)[(parametres>> 5)&31] &
	(*m_organismeActiu)[(parametres>>10)&31] ;
	logAccio << m_infoOrganismeActiu->descripcio() << "And    " << setw(2) << ((parametres>> 0)&31) << "=" << hex << setw(8) << (*m_organismeActiu)[(parametres>> 0)&31] << dec << endl;
	return true;
}

bool CBiosistema::organismeOr(uint32 parametres)
{
	(*m_organismeActiu)[(parametres>> 0)&31] =
	(*m_organismeActiu)[(parametres>> 5)&31] |
	(*m_organismeActiu)[(parametres>>10)&31] ;
	logAccio << m_infoOrganismeActiu->descripcio() << "Or     " << setw(2) << ((parametres>> 0)&31) << "=" << hex << setw(8) << (*m_organismeActiu)[(parametres>> 0)&31] << dec << endl;
	return true;
}

bool CBiosistema::organismeXor(uint32 parametres)
{
	(*m_organismeActiu)[(parametres>> 0)&31] =
	(*m_organismeActiu)[(parametres>> 5)&31] ^
	(*m_organismeActiu)[(parametres>>10)&31] ;
	logAccio << m_infoOrganismeActiu->descripcio() << "Xor    " << setw(2) << ((parametres>> 0)&31) << "=" << hex << setw(8) << (*m_organismeActiu)[(parametres>> 0)&31] << dec << endl;
	return true;
}

bool CBiosistema::organismeNot(uint32 parametres)
{
	uint32 & registre = (*m_organismeActiu)[(parametres>> 0)&31];
	registre = !registre;
	logAccio << m_infoOrganismeActiu->descripcio() << "Negat  " << setw(2) << ((parametres>> 0)&31) << "=" << hex << setw(8) << (*m_organismeActiu)[(parametres>> 0)&31] << dec << endl;
	return true;
}

bool CBiosistema::organismeOposa(uint32 parametres)
// Serveix per obtenir la direccio oposada
{
	uint32 & registre = (*m_organismeActiu)[(parametres>> 0)&31];
	registre ^= 0x77777777;
	logAccio << m_infoOrganismeActiu->descripcio() << "Oposa  " << setw(2) << ((parametres>> 0)&31) << "=" << hex << setw(8) << (*m_organismeActiu)[(parametres>> 0)&31] << dec << endl;
	return true;
}

bool CBiosistema::organismeRandom(uint32 parametres)
{
	(*m_organismeActiu)[(parametres>> 0)&31] = rnd.get();
	logAccio << m_infoOrganismeActiu->descripcio() << "Random " << setw(2) << ((parametres>> 0)&31) << "=" << hex << setw(8) << (*m_organismeActiu)[(parametres>> 0)&31] << dec << endl;
	return true;
}

bool CBiosistema::organismeCopia(uint32 parametres)
{
	(*m_organismeActiu)[(parametres>> 0)&31] =
	(*m_organismeActiu)[(parametres>> 5)&31];
	logAccio << m_infoOrganismeActiu->descripcio() << "Copy   " << setw(2) << ((parametres>> 0)&31) << "=" << hex << setw(8) << (*m_organismeActiu)[(parametres>> 0)&31] << dec << endl;
	return true;
}

bool CBiosistema::organismeCarrega(uint32 parametres)
{
	(*m_organismeActiu)[(parametres>> 0)&31] =
	m_organismeActiu->seguentInstruccio();
	logAccio << m_infoOrganismeActiu->descripcio() << "Load   " << setw(2) << ((parametres>> 0)&31) << "=" << hex << setw(8) << (*m_organismeActiu)[(parametres>> 0)&31] << dec << endl;
	return true;
}

bool CBiosistema::organismeNoOperacio(uint32 parametres)
{
	logAccio << m_infoOrganismeActiu->descripcio() << "No Operacio ..." << endl;
//	error << "Eps, accio no valida!!" << endl;
//	cin.get();
	return false;
}

bool CBiosistema::organismeLocalitza(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 direccio        = (*m_organismeActiu)[(parametres>> 0)&31];
	uint32 radi            = (*m_organismeActiu)[(parametres>> 5)&31]&31;
	uint32 clauElement     = (*m_organismeActiu)[(parametres>>10)&31]&3;
	uint32 tolerancia      = (*m_organismeActiu)[(parametres>>15)&31]&3;
	uint32 & registreDesti = (*m_organismeActiu)[(parametres>>20)&31];
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posBase = m_biotop->desplacament(posOrigen, direccio);

	for (uint32 i=Config.get("Sensors/Localitzacio/Intents")/*10*/; i--;) {
		uint32 posDesti = m_biotop->desplacamentAleatori (posBase, radi);
		uint32 elementTrobat;
		if ((*m_biotop)[posDesti].rastreja(elementTrobat, clauElement, tolerancia)) {
			// TODO: Aixo retorna si hi arribes nomes amb el desplacament
			m_biotop->unio(posOrigen,posDesti,registreDesti);
			logAccio << m_infoOrganismeActiu->descripcio() << "LocalQ " << setw(2) << ((parametres>>20)&31) << "=" << hex << setw(8) << registreDesti << dec << " " << clauElement << "(" << tolerancia << ")" << " a " << setw(2) << ((parametres>> 0)&31) << ":" << setw(8) << direccio << dec << endl;
			return true;
		}
	}
	// TODO: Be, com sempre aixo hauria de tenir un significat
	return true;
}

bool CBiosistema::organismeIdentifica(uint32 parametres)
{
	// Extreiem els parametres del codi d'operacio i el fenotip de l'organisme
	uint32 direccio        = (*m_organismeActiu)[(parametres>> 0)&31];
	uint32 radi            = (*m_organismeActiu)[(parametres>> 5)&31]&31;
	uint32 clauElement     = (*m_organismeActiu)[(parametres>>10)&31]&3;
	uint32 tolerancia      = (*m_organismeActiu)[(parametres>>15)&31]&3;
	uint32 & registreDesti = (*m_organismeActiu)[(parametres>>20)&31];
	// Precalculem tot el que utilitzem mutliples vegades
	uint32 posOrigen = m_infoOrganismeActiu->posicio();
	uint32 posBase = m_biotop->desplacament(posOrigen, direccio);

	for (uint32 i=Config.get("Sensors/Identificacio/Intents")/*10*/; i--;) {
		uint32 posDesti = m_biotop->desplacamentAleatori (posBase, radi);
		if ((*m_biotop)[posDesti].rastreja(registreDesti, clauElement, tolerancia)) {
			logAccio << m_infoOrganismeActiu->descripcio() << "IdentQ " << setw(2) << ((parametres>>20)&31) << "=" << hex << setw(8) << registreDesti << " a " << setw(2) << ((parametres>> 0)&31) << ":" << setw(8) << direccio << dec << endl;
			return true;
		}
	}
	return false;
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
	biosistema.agents(CAgent::ParsejaArxiu("Agents2.ini", *(biosistema.biotop()), error));
	biosistema.comunitat(new CComunitat);

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
				out << gotoxy(1,42) 
					<< "Coords: " << setw(3) << pos%Config.get("Biotop/CassellesAmplitud") 
					<< '@' << setw(3) << pos/Config.get("Biotop/CassellesAmplitud") 
					;
				break;
			case MapaOrganismes:
				out << gotoxy(42,1) 
					<< "Coords: " << setw(3) << pos%Config.get("Biotop/CassellesAmplitud") 
					<< '@' << setw(3) << pos/Config.get("Biotop/CassellesAmplitud") 
					;
				break;
			}
			out << gotoxy(1, 1) << "Poblacio: " << setw(6) << biosistema.comunitat()->tamany();
			out << gotoxy(1,26) << "Temps: " << setw(8) << biosistema.m_temps;
			out << setfill(' ') << endl;
		}

		// Obtenim l'entrada del teclat
//		cin.rdbuf()->sync();
		if (kbhit())// || cin.rdbuf()->in_avail()) 
		{
			cout << gotoxy(48,1) << clrlin << "Comanda: " << flush;
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
				case 'L': logAccio.desactiva(); break;
				case 'l': logAccio.activa(); break;
				case 'R': 
				case 'r': 
					// TODO: Recarregar configuracio 
					break;
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
				// out << gotoxy(48,20) << "Tecla: " << '\'' << (a?a:'#') << '\''<< endl;
			}
		}
		if (helpWanted) {
			helpWanted=false;
			cout << clrscr <<
				"Ajuda de consola de Bioscena\n" 
				"****************************\n\n" 
				"\nVisualitzacio:\n\n" 
				"  M      Visualitza el mapa (amb Shift ho treu)\n" 
				"  O      Visualitza comparativa organismes (amb Shift la treu)\n"
				"\nNavegacio Mapa:\n\n"
				"  W      1 posicio adalt (amb Shift 1 pantalla sencera)\n"
				"  S      1 posicio abaix (amb Shift 1 pantalla sencera)\n"
				"  D      1 posicio a la dreta (amb Shift 1 pantalla sencera)\n"
				"  A      1 posicio a l'esquerra (amb Shift 1 pantalla sencera)\n"
				"\nAltres:\n\n"
				"  Q      Surt del programa\n"
				"  V      Visualitza l'interior d'un organisme\n"
				"  J+Num  Es salta 'Num' pasos entre visualitzacio i visualitzacio\n"
				"  H      Aquesta pantalla d'ajuda\n"
				"  P      Congela el biosistema (amb Shift ho descongela)\n"
				"\n  Premi return per a continuar amb el programa\n"
				<< endl
				;
			cin.get();
			modeCanviat = true;
		}
		if (editWanted) {
			editWanted=false;
			cout << clrscr << endl;
			COrganisme * org = (*biosistema.comunitat())[id].cos();
			if (org) {
				uint32 pos = (*biosistema.comunitat())[id].posicio();
				out << (*biosistema.comunitat())[id].descripcio() << '\t' <<  pos%Config.get("Biotop/CassellesAmplitud")<< '@' <<  pos/Config.get("Biotop/CassellesAmplitud") << endl;
				org->dump(out);
			}
			else cout << "Ops! ha devido palmar" << endl;
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
