// MultiAgent.cpp: implementation of the CMultiAgent class.
//
//////////////////////////////////////////////////////////////////////

#include <fstream>
#include "MultiAgent.h"
#include "Missatger.h"
#include "TopologiaToroidal.h"
#include "Substrat.h"
#include "Itineraris.h"
#include "Actuadors.h"
#include "Iterador.h"
#include "Temporitzador.h"

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

void CMultiAgent::dump(CMissatger & msg)
{
	CAgent::dump(msg);
	for (list<CAgent*>::iterator it=m_agents.begin(); it!=m_agents.end(); it++)
	{
		msg	<< "\tAccio(" << (*it)->nom() << ")" << endl;
	}
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CMultiAgent::ProvaClasse()
{
	out << "\033[J";// Un clrscr xapuser pero standard (ANSI)
	out << blanc.brillant() << "Provant MultiAgent" << blanc.fosc() << endl;
	CTopologiaToroidal<CSubstrat> biotop(70,24);

	CMultiAgent agents;

	// La direccio de la qual dependra l'itinerari del Actuador1
	CDireccionador* direccio1 = new CDireccionadorAleatori(biotop);
	// El temporitzador determina la frequencia en que es varia la direccio
	CTemporitzador *timerDireccio1=new CTemporitzador;
	timerDireccio1->accio(direccio1);
	timerDireccio1->cicleActiu(1);
	timerDireccio1->cicleInactiu(5);
	agents.accio(timerDireccio1);
	// La posicio es mou segons el direccionador
	CPosicionador* posicio1 = new CItinerari(biotop);
	((CItinerari*)posicio1)->direccionador(direccio1);
	agents.accio(posicio1);
	// L'actuador 1 es un Nutridor que afegeix nutrients
	CActuador *actuador1 = new CNutridor;
	actuador1->posicionador(posicio1);
	((CNutridor*)actuador1)->composicio (31, 0);
	// L'iterador diu quantes vegades actua per accionat
	CIterador * iterador1 = new CIterador;
	iterador1->iteracions(14,0,0);
	iterador1->accio(actuador1);
	agents.accio(iterador1);

	// Posicionador
	CPosicionador* posicioCentral = new CPosicionador(biotop);
	posicioCentral->pos(1085);
	agents.accio(posicioCentral);

	CPosicionador* posicio2 = new CPosicionadorZonal(biotop);
	((CPosicionadorZonal*)posicio2)->posicionador(posicioCentral);
	((CPosicionadorZonal*)posicio2)->radi(10);
	agents.accio(posicio2);

	CIterador * iterador2 = new CIterador;
	iterador2->iteracions(10,10,2);
	agents.accio(iterador2);

	CPosicionador* distribucio2 = new CPosicionadorZonal(biotop);
	((CPosicionadorZonal*)distribucio2)->posicionador(posicio2);
	((CPosicionadorZonal*)distribucio2)->radi(1);
	iterador2->accio(distribucio2);

	CActuador *actuador2 = new CDesnutridor;
	actuador2->posicionador(distribucio2);
//	((CNutridor*)actuador2)->distribucio (5, 1);
	((CNutridor*)actuador2)->composicio (31, 0	);
	iterador2->accio(actuador2);

	for (int i=400; i--;)
	{
		agents();
		if (!(i%5)) {
			biotop.debugPresenta(out);
			out << blanc << setw(4) << i <<endl;
		}
	}

//	dumpDiccionari(out);
	ofstream file("AgentsLog.txt");
	CColorOutputer miout(file);
	CMissatger msg(NULL, NULL, miout);
	agents.dumpAll(msg);
}

