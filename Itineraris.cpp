// Cumulador.cpp: implementation of the CCumulador class.
//
//////////////////////////////////////////////////////////////////////

#include "Itineraris.h"
#include "Cumulador.h"
#include "Temporitzador.h"

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CPosicionadorZonal::ProvaClasse()
{
	out << "Provant Posicionador Zonal" << endl;
	CTopologiaToroidal<CSubstrat> biotop(70,35);
	CPosicionador* posicioCentral = new CPosicionador(biotop);
	CPosicionador* posicio = new CPosicionadorZonal(biotop, posicioCentral);
	posicioCentral->pos(1085);
	((CPosicionadorZonal*)posicio)->radi(20);
	CCumulador cum (biotop, posicio);
	cum.distribucio (5, 1);
	cum.composicio (31, 0);
	for (int i=300; i--;)
	{
		(*posicioCentral)();
		(*posicio)();
		(cum)();
		if (!(i%5)) biotop.debugPresenta(out);
		out << blanc << i <<endl;
	}
}

void CDireccionadorAleatori::ProvaClasse()
{
	out << "Provant Direccionador Aleatori" << endl;
	CTopologiaToroidal<CSubstrat> biotop(70,22);
	CDireccionador* direccio = new CDireccionadorAleatori(biotop);
	CPosicionador* posicio = new CItinerari(biotop, direccio);
	CCumulador cum (biotop, posicio);
	cum.distribucio (1, 0);
	cum.composicio (31, 0);
	CTemporitzador timer1;
	timer1.accio(&cum);
	timer1.cicleActiu(6);
	timer1.cicleDesactiu(1);
	CTemporitzador timer2;
	timer2.accio(posicio);
	timer2.cicleActiu(10);
	timer2.cicleDesactiu(1);
	CTemporitzador timer3;
	timer3.accio(direccio);
	timer3.cicleActiu(1);
	timer3.cicleDesactiu(32);
	for (int i=100; i--;)
	{
		timer3();
		timer2();
		timer1();
		out << blanc << i <<endl;
		if (!(i%5)) biotop.debugPresenta(out);
	}
}

void CPosicionadorAleatori::ProvaClasse()
{
	out << "Provant Posicionador Aleatori" << endl;
	CTopologiaToroidal<CSubstrat> biotop(70,22);
	CPosicionador* posicio = new CPosicionadorAleatori(biotop);
	CCumulador cum (biotop, posicio);
	cum.distribucio (1, 0);
	cum.composicio (31, 0);
	CTemporitzador timer1;
	timer1.accio(&cum);
	timer1.cicleActiu(6);
	timer1.cicleDesactiu(1);
	CTemporitzador timer2;
	timer2.accio(posicio);
	timer2.cicleActiu(10);
	timer2.cicleDesactiu(1);
	for (int i=100; i--;)
	{
		timer2();
		timer1();
		out << blanc << i <<endl;
		if (!(i%5)) biotop.debugPresenta(out);
	}
}
