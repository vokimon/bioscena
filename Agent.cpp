// Agent.cpp: implementation of the CAgent class.
//
//////////////////////////////////////////////////////////////////////

#include "Agent.h"
#include "Missatger.h"
#include "TopologiaToroidal.h"
#include "Substrat.h"
#include "Itineraris.h"
#include "Cumulador.h"
#include "Temporitzador.h"

void hola(){out << "hola "<<endl;}

void CMultiAgent::ProvaClasse()
{
	out << "Provant Direccionador Aleatori" << endl;
	CMultiAgent agents;
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
	agents.add(&timer1);
	CTemporitzador timer2;
	timer2.accio(posicio);
	timer2.cicleActiu(10);
	timer2.cicleDesactiu(1);
	agents.add(&timer2);
	CTemporitzador timer3;
	timer3.accio(direccio);
	timer3.cicleActiu(1);
	timer3.cicleDesactiu(5);
	agents.add(&timer3);
	CPosicionador* posicioCentral = new CPosicionador(biotop);
	posicioCentral->pos(1085);
	agents.add(posicioCentral);
	CPosicionador* posicio2 = new CPosicionadorZonal(biotop, posicioCentral);
	((CPosicionadorZonal*)posicio2)->radi(20);
	agents.add(posicio2);
	CCumulador cum2 (biotop, posicio2);
	cum2.distribucio (5, 1);
	cum2.composicio (31, 0);
	agents.add(&cum2);
	for (int i=100; i--;)
	{
		agents();
		if (!(i%5)) biotop.debugPresenta(out);
		out << blanc << i <<endl;
	}
}


