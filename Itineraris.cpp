// Cumulador.cpp: implementation of the CCumulador class.
//
//////////////////////////////////////////////////////////////////////

#include "Itineraris.h"
#include "Actuadors.h"
#include "Aleaturitzador.h"
#include "Temporitzador.h"
#include "TopologiaToroidal.h"

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CPosicionadorZonal::ProvaClasse()
{
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
		if (!(i%5)) biotop.debugPresenta(out);
		out << blanc << i <<endl;
	}
}

void CDireccionadorAleatori::ProvaClasse()
{
	out << "\033[J";
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

	for (int i=400; i--;)
	{
		agents();
		if (!(i%5)) {
			biotop.debugPresenta(out);
			out << blanc << i <<endl;
		}
	}
}

void CPosicionadorAleatori::ProvaClasse()
{
	out << blanc.brillant() << "Provant Posicionador Aleatori" << endl;
	CTopologiaToroidal<CSubstrat> biotop(70,22);
	CPosicionador* posicio = new CPosicionadorAleatori(biotop);
	CNutridor cum;
	cum.posicionador(posicio);
//	cum.distribucio (1, 0);
	cum.composicio (31, 0);
	CTemporitzador timer1;
	timer1.accio(&cum);
	timer1.cicleActiu(6);
	timer1.cicleInactiu(1);
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
			out << blanc << i <<endl;
		}
	}
}
