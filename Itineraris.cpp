// Itineraris.cpp: CPosicionador & CDireccionador subclasses' implementation.
//
//////////////////////////////////////////////////////////////////////

#include "Itineraris.h"
#include "Actuadors.h"
#include "Aleaturitzador.h"
#include "Temporitzador.h"
#include "Iterador.h"
#include "TopologiaToroidal.h"
#include "Color.h"

using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Construccio/Destruccio
//////////////////////////////////////////////////////////////////////

CItinerari::CItinerari(tipus_biotop & biotop)
	:CPosicionador(biotop)
{
	m_tipus+="/Direccional";
	m_direccionador=NULL;
	m_radi=1;
}

CPosicionadorZonal::CPosicionadorZonal(tipus_biotop & biotop)
	:CPosicionador(biotop)
{
	m_tipus+="/Zonal";
	m_posicionador=NULL;
	m_radi=1;
}

CPosicionadorAleatori::CPosicionadorAleatori(tipus_biotop & biotop)
	:CPosicionador(biotop) 
{
	m_tipus+="/Aleatori";
}

CDireccionadorAleatori::CDireccionadorAleatori(tipus_biotop & biotop)
	:CDireccionador(biotop) 
{
	m_tipus+="/Aleatori";
};

//////////////////////////////////////////////////////////////////////
// Redefinibles (Itinerari)
//////////////////////////////////////////////////////////////////////

void CItinerari::operator() (void) 
{
	if (!m_direccionador)
		warning << "CItinerari " << nom() << " accionat sense posicionador" << endl;
	else 
		for (int i=m_radi; i--;)
			m_pos = m_biotop.desplacament(m_pos, m_direccionador->dir());
}

void CItinerari::dump(CMissatger & msg)
{
	CPosicionador::dump(msg);
	msg << "- Radi " << m_radi << endl;
	if (m_direccionador) 
		msg << "- Direccionador " << m_direccionador->nom() << endl; 
}

bool CItinerari::configura(string parametre, istream & valor, t_diccionariAgents & diccionari, CMissatger & errors)
{
	if (parametre=="Radi") {
		uint32 rad;
		if (!(valor>>rad))
			errors << "Format invalid pel radi de '" << nom() << "'" << endl;
		else 
			radi(rad);
		return true;
	}
	if (parametre=="Direccionador") {
		string tipusBasic("Agent/Direccionador");
		string nomDependencia;
		t_diccionariAgents::iterator it;
		if (!(valor>>nomDependencia))
			errors << "Format invalid per a l'especificacio del posicionador de '" << nom() << "'" << endl;
		else if ((it=diccionari.find(nomDependencia))==diccionari.end())
			errors << "El direccionador '" << nomDependencia << "' de l'agent '" << nom() << "' no esta definit al fitxer." << endl;
		else if (it->second->tipus().find(tipusBasic)==string::npos)
			errors << "L'agent '" << nomDependencia << "' no es un subtipus de '" << tipusBasic << "' com necessita l'agent '" << nom() << "'." << endl;
		else if (false) // TODO: Ha de ser del tipus 
			errors << "L'agent '" << nomDependencia << "' no es un direccionador com necessita l'agent '" << nom() << "'." << endl;
		else 
			direccionador((CDireccionador*)it->second);
		return true; // Parametre interceptat
	}
	// Li deixem a la superclasse que l'intercepti si vol
	return inherited::configura(parametre, valor, diccionari, errors);
}

list<CAgent*> CItinerari::dependencies() {
	list<CAgent*> l=CPosicionador::dependencies();
	if (m_direccionador) l.push_back(m_direccionador); 
	return l;
}

//////////////////////////////////////////////////////////////////////
// Redefinibles (PosicionadorZonal)
//////////////////////////////////////////////////////////////////////

void CPosicionadorZonal::operator() (void) 
{
	if (!m_posicionador) 
	{
		warning << "PosicionadorZonal sense Posicionador central" << endl;
		return;
	}
	m_pos = m_biotop.desplacamentAleatori(m_posicionador->pos(),m_radi);
	/* 
	// Veure el ChangeLog 19991130.VoK Aixo es el que hi havia abans
	m_pos=m_posicionador->pos();
	for (int i=m_radi; i--;)
		m_pos = m_biotop.desplacament(m_pos, rnd.get());
	*/
}

void CPosicionadorZonal::dump(CMissatger & msg)
{
	CPosicionador::dump(msg);
	msg << "- Radi " << m_radi << endl;
	if (m_posicionador) 
		msg << "- Posicionador " << m_posicionador->nom() << endl; 
}

bool CPosicionadorZonal::configura(string parametre, istream & valor, t_diccionariAgents & diccionari, CMissatger & errors)
{
	if (parametre=="Radi") {
		uint32 rad;
		if (!(valor>>rad))
			errors << "Format invalid pel radi de '" << nom() << "'" << endl;
		else 
			radi(rad);
		return true;
	}
	if (parametre=="Posicionador") {
		string tipusBasic("Agent/Posicionador");
		string nomDependencia;
		t_diccionariAgents::iterator it;
		if (!(valor>>nomDependencia))
			errors << "Format invalid per a l'especificacio del posicionador de '" << nom() << "'" << endl;
		else if ((it=diccionari.find(nomDependencia))==diccionari.end())
			errors << "El posicionador '" << nomDependencia << "' de l'agent '" << nom() << "' no esta definit al fitxer." << endl;
		else if (it->second->tipus().find(tipusBasic)==string::npos)
			errors << "L'agent '" << nomDependencia << "' no es un subtipus de '" << tipusBasic << "' com necessita l'agent '" << nom() << "'." << endl;
		else 
			posicionador((CPosicionador*)it->second);
		return true; // Parametre interceptat
	}
	// Li deixem a la superclasse que l'intercepti si vol
	return inherited::configura(parametre, valor, diccionari, errors);
}

list<CAgent*> CPosicionadorZonal::dependencies() {
	list<CAgent*> l=CPosicionador::dependencies();
	if (m_posicionador) l.push_back(m_posicionador); 
	return l;
}

//////////////////////////////////////////////////////////////////////
// Redefinibles (PosicionadorAleatori)
//////////////////////////////////////////////////////////////////////

void CPosicionadorAleatori::operator() (void) 
{
	m_pos=m_biotop.posicioAleatoria();
}

//////////////////////////////////////////////////////////////////////
// Redefinibles (DireccionadorAleatori)
//////////////////////////////////////////////////////////////////////

void CDireccionadorAleatori::operator() (void) 
{
	m_dir = rnd.get();
}

//////////////////////////////////////////////////////////////////////
// Operacions (Itinerari)
//////////////////////////////////////////////////////////////////////

void CItinerari::direccionador(CDireccionador * dir) 
{
	m_direccionador=dir;
}

CDireccionador * CItinerari::direccionador() 
{
	return m_direccionador;
}

uint32 CItinerari::radi() 
{
	return m_radi;
}

void CItinerari::radi(uint32 rad) 
{
	m_radi=rad;
}

//////////////////////////////////////////////////////////////////////
// Operacions (PosicionadorZonal)
//////////////////////////////////////////////////////////////////////

void CPosicionadorZonal::posicionador(CPosicionador * pos) 
{
	m_posicionador=pos;
}

CPosicionador * CPosicionadorZonal::posicionador() 
{
	return m_posicionador;
}

uint32 CPosicionadorZonal::radi() 
{
	return m_radi;
}

void CPosicionadorZonal::radi(uint32 rad) 
{
	m_radi=rad;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CItinerari::ProvaClasse()
{
	out << clrscr;
	out << blanc.brillant() << "Provant Itinerari" << endl;
	CTopologiaToroidal<CSubstrat> biotop(70,21);
/*
	CDireccionador* direccio = new CDireccionador(biotop);
//	direccio->dir(000177777777L);
//	direccio->dir(037701234567L); // Aixi aniria amb els selectors (377) davant (en octal)
	direccio->dir(0x37777777777L); // Aixi va amb els selectors

	CPosicionador* posicio = new CItinerari(biotop);
//	posicio->pos(20);
	((CItinerari*)posicio)->direccionador(direccio);

	CActuador * nutridor = new CNutridor;
	((CNutridor*)nutridor)->composicio (31, 0);
	((CNutridor*)nutridor)->posicionador(posicio);

	CAleaturitzador * ruleta = new CAleaturitzador;
	ruleta->probabilitat(20, 1);
	ruleta->accio(direccio);

	CMultiAgent agents;
	agents.accio(nutridor);
	agents.accio(ruleta);
	agents.accio(posicio);
*/
	CPosicionador* posicioRef = new CPosicionadorAleatori(biotop);
	posicioRef->pos(20);

	CDireccionador* direccio = new CDireccionador(biotop);
	direccio->dir(0x00000000L); // Aixi va amb els selectors

	CPosicionador* posicio = new CItinerari(biotop);
	posicio->pos(300);
	((CItinerari*)posicio)->direccionador(direccio);

	CActuador * nutridor = new CNutridor;
	((CNutridor*)nutridor)->composicio (31, 0);
	((CNutridor*)nutridor)->posicionador(posicio);

	CActuador * desnutridor = new CDesnutridor;
	((CNutridor*)desnutridor)->composicio (31, ~0);
	((CNutridor*)desnutridor)->posicionador(posicio);

	CActuador * nutridorRef = new CNutridor;
	((CNutridor*)nutridorRef)->composicio (0, 0);
	((CNutridor*)nutridorRef)->posicionador(posicioRef);

	CActuador * desnutridorRef = new CDesnutridor;
	((CNutridor*)desnutridorRef)->composicio (0, ~0);
	((CNutridor*)desnutridorRef)->posicionador(posicioRef);

	CIterador * iterador1 = new CIterador;
	iterador1->iteracions(4);
	iterador1->accio(desnutridor);

	CIterador * iterador2 = new CIterador;
	iterador2->iteracions(4);
	iterador2->accio(nutridor);

	CAleaturitzador * ruletaRef = new CAleaturitzador;
	ruletaRef->probabilitat(5, 1);
	ruletaRef->accio(desnutridorRef);
	ruletaRef->accio(posicioRef);
	ruletaRef->accio(nutridorRef);

	CMultiAgent agents;
	agents.accio(iterador1);
	agents.accio(posicio);
	agents.accio(iterador2);
	agents.accio(ruletaRef);
	agents.accio(direccio);

	for (int i=50; i--;)
	{
		uint32 direccioCerca;
		bool alcancat = biotop.unio(posicio->pos(),posicioRef->pos(),direccioCerca);
		direccio->dir(direccioCerca&0xFFFF0000L);
		agents();
//		if (!(i%5)) {
			biotop.debugPresenta(out);
			out 
				<< blanc.fosc() 
				<< " Posicio: " << setw(4) << posicio->pos() << ". Direccio: " << hex << setw(8) << direccio->dir() << dec << endl
				<< " Posicio Desti: " << setw(4) << posicioRef->pos() << " " << (alcancat?"Alcancat!!":"Cercant...") << endl
				<< " Quenden " << setw(4) << i << " iteracios." << endl;
//			cin.get();
//		}
	}

	agents.dumpAll(out);
	cin.get();

}

void CPosicionadorZonal::ProvaClasse()
{
	out << clrscr;
	out << blanc.brillant() << "Provant Posicionador Zonal" << endl;
	CTopologiaToroidal<CSubstrat> biotop(70,35);
	CPosicionador* posicioCentral = new CPosicionador(biotop);
	CPosicionador* posicio = new CPosicionadorZonal(biotop);
	posicioCentral->pos(1085);
	((CPosicionadorZonal*)posicio)->posicionador(posicioCentral);
	((CPosicionadorZonal*)posicio)->radi(20);
	CActuador * nutridor = new CNutridor;
	((CNutridor*)nutridor)->posicionador(posicio);
//	nutridor.distribucio (5, 1);
	((CNutridor*)nutridor)->composicio (31, 0);
	for (int i=300; i--;)
	{
		(*posicioCentral)();
		(*posicio)();
		(*nutridor)();
		if (!(i%5)) {
			biotop.debugPresenta(out);
			out << blanc.fosc() << setw(4) << i <<endl;
		}
	}
//	X.dumpAll(out);
	cin.get();
}

void CDireccionadorAleatori::ProvaClasse()
{
	out << clrscr;
	out << blanc.brillant() << "Provant Direccionador Aleatori" << endl;
	CTopologiaToroidal<CSubstrat> biotop(70,22);

	CDireccionador* direccio = new CDireccionadorAleatori(biotop);

	CPosicionador* posicio = new CItinerari(biotop);
	((CItinerari*)posicio)->direccionador(direccio);

	CActuador * nutridor = new CNutridor;
	((CNutridor*)nutridor)->composicio (31, 0);
	((CNutridor*)nutridor)->posicionador(posicio);

	CAleaturitzador * ruleta = new CAleaturitzador;
	ruleta->probabilitat(20, 1);
	ruleta->accio(direccio);

	CMultiAgent agents;
	agents.accio(ruleta);
	agents.accio(posicio);
	agents.accio(nutridor);

	for (int i=200; i--;)
	{
		agents();
		if (!(i%5)) {
			biotop.debugPresenta(out);
			out << blanc.fosc() << setw(4) << i <<endl;
		}
	}
	agents.dumpAll(out);
	cin.get();
}

void CPosicionadorAleatori::ProvaClasse()
{
	out << clrscr;
	out << blanc.brillant() << "Provant Posicionador Aleatori" << blanc.fosc() << endl;
	CTopologiaToroidal<CSubstrat> biotop(70,22);
	CPosicionador* posicio = new CPosicionadorAleatori(biotop);
	CNutridor *nutridor = new CNutridor;
	nutridor->posicionador(posicio);
	nutridor->composicio (31, 0);
	CTemporitzador timer1;
	timer1.accio(nutridor);
	timer1.cicleActiu(2);
	timer1.cicleInactiu(0);
	CTemporitzador timer2;
	timer2.accio(posicio);
	timer2.cicleActiu(10);
	timer2.cicleInactiu(1);
	for (int i=100; i--;)
	{
		timer2();
		timer1();
		if (!(i%5)) {
			biotop.debugPresenta(out);
			out << blanc.fosc() << setw(4) << i <<endl;
		}
	}
//	X.dumpAll(out);
	cin.get();
}

