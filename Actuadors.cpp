// Actuadors.cpp: implementation of the CActuador subclasses.
//
//////////////////////////////////////////////////////////////////////

#include "Actuadors.h"
#include "Itineraris.h"
#include "Temporitzador.h"
#include "TopologiaToroidal.h"
#include "Iterador.h"
#include "MultiAgent.h"
#include "Color.h"

using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNutridor::CNutridor()
{
	m_tipus+="/Nutridor";
	m_variabilitat=0;
	m_element=0;
}

CNutridor::~CNutridor()
{

}

CDesnutridor::CDesnutridor()
{
	m_tipus+="/Invers";
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

void CNutridor::dump(CMissatger & msg)
{
	CActuador::dump(msg);
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
	return CActuador::configura(parametre, valor, diccionari, errors);
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
	out << clrscr;
	out << blanc.brillant() << "Provant Agent Nutridor" << endl;
	
	CTopologiaToroidal<CSubstrat> biotop(70,22);

	CMultiAgent agents;

	CDireccionador* direccio = new CDireccionadorAleatori(biotop);

	CPosicionador* posicioCentral = new CItinerari(biotop);
	posicioCentral->pos(123);
	((CItinerari*)posicioCentral)->direccionador(direccio);

	CPosicionador* posicio = new CPosicionadorZonal(biotop);
	((CPosicionadorZonal*)posicio)->posicionador(posicioCentral);

	CActuador * nutridor = new CNutridor;
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
			biotop.debugPresenta(out);
			out << setw(4) << blanc.fosc() << i <<endl;
		}
	}
	agents.dumpAll(out);
	cin.get();
}

void CDesnutridor::ProvaClasse()
{
	out << clrscr;
	out << blanc.brillant() << "Provant Agent Desnutridor" << endl;

	CTopologiaToroidal<CSubstrat> biotop(70,22);

	CMultiAgent agents;

	CPosicionador* posicioCentral = new CPosicionador(biotop);
	posicioCentral->pos(735);

	CPosicionador* posicio = new CPosicionadorZonal(biotop);
	((CPosicionadorZonal*)posicio)->posicionador(posicioCentral);
	((CPosicionadorZonal*)posicio)->radi(4);

	CActuador * cum = new CNutridor;
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
			biotop.debugPresenta(out);
			out << blanc.fosc() << setw(4) << i <<endl;
		}
	}
	CPosicionador* posicio2 = new CPosicionadorZonal(biotop);
	((CPosicionadorZonal*)posicio2)->posicionador(posicioCentral);
	((CPosicionadorZonal*)posicio2)->radi(1);

	CActuador * cla = new CDesnutridor;
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
			biotop.debugPresenta(out);
			out << blanc.fosc() << setw(4) << i <<endl;
		}
	}
	agents.dumpAll(out);
	cin.get();
}

