// Cumulador.cpp: implementation of the CCumulador class.
//
//////////////////////////////////////////////////////////////////////

#include "Cumulador.h"
#include "Agent.h"
#include "Itineraris.h"
#include "Temporitzador.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCumulador::CCumulador(CTopologia<CSubstrat>& biotop, CPosicionador *pos)
	: m_biotop(biotop)
{
	m_posicionador=pos;
	m_radi=1;
	m_densitat=1;
	m_variabilitat=0;
	m_element=0;
}

CCumulador::~CCumulador()
{

}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

void CCumulador::distribucio(uint32 densitat, uint32 radi)
{
	m_radi=radi;
	m_densitat=densitat;
}

void CCumulador::composicio(uint32 element, uint32 variabilitat)
{
	m_variabilitat=variabilitat;
	m_element=element;
}


void CCumulador::posicio(CPosicionador *pos)
{
	m_posicionador=pos;
}


//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CCumulador::ProvaClasse()
{
	CTopologiaToroidal<CSubstrat> biotop(70,22);
	CDireccionador* direccio = new CDireccionadorAleatori(biotop);
	CPosicionador* posicio = new CItinerari(biotop, direccio);
	posicio->pos(123);
	CCumulador cum (biotop, posicio);
	cum.distribucio (1, 0);
	cum.composicio (31, 0);
	CTemporitzador timer1, timer2, timer3;
	timer1.accio(&cum);
	timer1.cicleActiu(6);
	timer1.cicleDesactiu(2);
	timer2.accio(direccio);
	timer2.cicleActiu(1);
	timer2.cicleDesactiu(5);
	for (int i=30; i--;)
	{
		(*posicio)();
		timer2();
		timer1();
		biotop.debugPresenta(out);
		out << blanc << i <<endl;
	}
}

void CClarejador::ProvaClasse()
{
	CTopologiaToroidal<CSubstrat> biotop(70,22);
	CDireccionador* direccio = new CDireccionadorAleatori(biotop);
	CPosicionador* posicio = new CItinerari(biotop, direccio);
	CCumulador cum (biotop, posicio);
	cum.distribucio (10, 1);
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
		biotop.debugPresenta(out);
		out << blanc << i <<endl;
	}
	CClarejador cla (biotop, posicio);
	cla.distribucio (100, 3);
	cla.composicio (31, 0xFFFFFFFF);
	timer1.accio(&cla);
	for (i=30; i--;)
	{
		timer2();
		timer1();
		biotop.debugPresenta(out);
		out << blanc << i <<endl;
	}
}
