// ActuadorsNutrients.cpp: implementation of some CActuador subclasses.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990820 VoK - Creat (a partir de l'antic Cumulador)
// 19990822 VoK - Funcions de dump
// 19990823 VoK - Reordenat els fitxers
// 19990824 VoK - dump ja no posa ':' despres de nom del parametre
// 19990824 VoK - Funcions de configuracio amb parametres
// 20000708 VoK - Nutridor inicialitza el biotop i no Actuador
// 20000708 VoK - Ja no fem servir la intermitja operator()(CSubstrat)
//////////////////////////////////////////////////////////////////////

#include "Actuadors.h"
#include "Itineraris.h"
#include "Temporitzador.h"
#include "TopologiaToroidal.h"
#include "Iterador.h"
#include "Biotop.h"
#include "MultiAgent.h"
#include "Color.h"
#include "Organisme.h"

using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNutridor::CNutridor(t_biotop & biotop)
	: m_biotop(biotop)
{
	m_tipus+="/Nutridor";
	m_variabilitat=0;
	m_element=0;
}

CNutridor::~CNutridor()
{

}

CDesnutridor::CDesnutridor(t_biotop & biotop)
	: CNutridor(biotop)
{
	m_tipus+="/Invers";
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

void CNutridor::operator () () {
	if (!m_posicionador)
		warning << "CActuador " << nom() << " accionat sense posicionador" << endl;
	CSubstrat & s = m_biotop[m_posicionador->pos()];
	uint32 nutrient = m_element^(m_variabilitat&rnd.get());
	s.deposita(nutrient);
}

void CNutridor::dump(CMissatger & msg)
{
	inherited::dump(msg);
	msg << "- Composicio " << m_element << " " << m_variabilitat << endl;
}

bool CNutridor::configura(string parametre, istream & valor, t_diccionariAgents & diccionari, CMissatger & errors)
{
	if (parametre=="Composicio") {
		uint32 element, variabilitat;
		if (!(valor>>element))
			errors << "Format invalid per la composicio de '" << nom() << "'" << endl;
		else if (!(valor>>variabilitat))
			composicio(element);
		else 
			composicio(element, variabilitat);
		return true; // Parametre interceptat
	}
	// Li deixem a la superclasse que l'intercepti si vol
	return inherited::configura(parametre, valor, diccionari, errors);
}
void CDesnutridor::operator() () {
	if (!m_posicionador)
		warning << "CActuador " << nom() << " accionat sense posicionador" << endl;
	CSubstrat & s = m_biotop[m_posicionador->pos()];
	uint32 nutrient;
	s.extreu(nutrient, m_element, m_variabilitat);
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

void CNutridor::composicio(uint32 element, uint32 variabilitat)
{
	m_variabilitat=variabilitat;
	m_element=element;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CNutridor::ProvaClasse()
{
	//TODO: Treure els comentaris dels debug presenta
	out << clrscr;
	out << blanc.brillant() << "Provant Agent Nutridor" << endl;
	
	CTopologiaToroidal topo(70,22);
	CBiotop<CSubstrat> biotop(&topo);

	CMultiAgent agents;

	CDireccionador* direccio = new CDireccionadorAleatori(biotop);

	CPosicionador* posicioCentral = new CItinerari(biotop);
	posicioCentral->pos(123);
	((CItinerari*)posicioCentral)->direccionador(direccio);

	CPosicionador* posicio = new CPosicionadorZonal(biotop);
	((CPosicionadorZonal*)posicio)->posicionador(posicioCentral);

	CActuador * nutridor = new CNutridor(biotop);
	nutridor->posicionador(posicio);
	((CNutridor*)nutridor)->composicio(31, 0);

	CIterador * iterador = new CIterador;
	iterador->accio(nutridor);
	iterador->accio(posicio);
	iterador->iteracions(20);

	CTemporitzador * timer = new CTemporitzador;
	timer->accio(direccio);
	timer->cicleActiu(1);
	timer->cicleInactiu(5);

	agents.accio(posicioCentral);
	agents.accio(iterador);
	agents.accio(timer);

	for (int i=200; i--;)
	{
		agents();
		if (!(i%5)) {
//			biotop.debugPresenta(out);
			out << setw(4) << blanc.fosc() << i <<endl;
		}
	}
	agents.dumpAll(out);
	cin.get();
}

void CDesnutridor::ProvaClasse()
{
	//TODO: Treure els comentaris dels debug presenta
	out << clrscr;
	out << blanc.brillant() << "Provant Agent Desnutridor" << endl;

	CTopologiaToroidal topo(70,22);
	CBiotop<CSubstrat> biotop(&topo);

	CMultiAgent agents;

	CPosicionador* posicioCentral = new CPosicionador(biotop);
	posicioCentral->pos(735);

	CPosicionador* posicio = new CPosicionadorZonal(biotop);
	((CPosicionadorZonal*)posicio)->posicionador(posicioCentral);
	((CPosicionadorZonal*)posicio)->radi(4);

	CActuador * cum = new CNutridor(biotop);
	((CNutridor*)cum)->posicionador(posicio);
//	cum->distribucio (10, 4);
	((CNutridor*)cum)->composicio (31, 0);

	agents.accio(cum);
	agents.accio(posicio);
	agents.accio(posicioCentral);
	int i;
	for (i=100; i--;)
	{
		agents();
		if (!(i%5)) {
//			biotop.debugPresenta(out);
			out << blanc.fosc() << setw(4) << i <<endl;
		}
	}
	CPosicionador* posicio2 = new CPosicionadorZonal(biotop);
	((CPosicionadorZonal*)posicio2)->posicionador(posicioCentral);
	((CPosicionadorZonal*)posicio2)->radi(1);

	CActuador * cla = new CDesnutridor(biotop);
	cla->posicionador(posicio2);
	((CDesnutridor*)cla)->composicio (31, 0xFFFFFFFF);

	CIterador *iterador = new CIterador;
	iterador->iteracions(10);
	iterador->accio(cla);

	agents.accio(iterador);
	agents.accio(posicio2);
	for (i=200; i--;)
	{
		agents();
		if (!(i%5)) {
//			biotop.debugPresenta(out);
			out << blanc.fosc() << setw(4) << i <<endl;
		}
	}
	agents.dumpAll(out);
	cin.get();
}


