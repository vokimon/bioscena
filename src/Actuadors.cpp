// Actuadors.cpp: implementation of the CActuador subclasses.
//
//////////////////////////////////////////////////////////////////////

#include "Actuadors.h"
#include "Itineraris.h"
#include "Temporitzador.h"
#include "TopologiaToroidal.h"
#include "Iterador.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNutridor::CNutridor()
{
	m_variabilitat=0;
	m_element=0;
	m_tipus+="/Nutridor";
}

CNutridor::~CNutridor()
{

}

CDesnutridor::CDesnutridor()
{
	m_tipus+="/Invers";
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
	CTopologiaToroidal<CSubstrat> biotop(70,22);

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
	iterador->iteracions(6);

	CTemporitzador * timer1 = new CTemporitzador;
	timer1->accio(iterador);
	timer1->accio(posicioCentral);
	timer1->cicleActiu(6);
	timer1->cicleInactiu(2);

	CTemporitzador * timer2 = new CTemporitzador;
	timer2->accio(direccio);
	timer2->cicleActiu(1);
	timer2->cicleInactiu(5);

	for (int i=30; i--;)
	{
		(*posicio)();
		(*timer2)();
		(*timer1)();
		biotop.debugPresenta(out);
		out << blanc << i <<endl;
	}
}

void CDesnutridor::ProvaClasse()
{
	CTopologiaToroidal<CSubstrat> biotop(70,22);

	CPosicionador* posicioCentral = new CPosicionador(biotop);
	posicioCentral->pos(735);

	CPosicionador* posicio = new CPosicionadorZonal(biotop);
	((CPosicionadorZonal*)posicio)->posicionador(posicioCentral);

	CActuador * cum = new CNutridor;
	((CNutridor*)cum)->posicionador(posicio);
//	cum->distribucio (10, 4);
	((CNutridor*)cum)->composicio (31, 0);
	int i;
	for (i=20; i--;)
	{
		(*cum)();
		biotop.debugPresenta(out);
		out << blanc << i <<endl;
	}
	CActuador * cla = new CDesnutridor;
	cla->posicionador(posicio);
//	cla->distribucio (20, 1);
	((CDesnutridor*)cla)->composicio (31, 0xFFFFFFFF);
	for (i=40; i--;)
	{
		(*cum)();
		(*cla)();
		biotop.debugPresenta(out);
		out << blanc << i <<endl;
	}
}
