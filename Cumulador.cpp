// Cumulador.cpp: implementation of the CCumulador class.
//
//////////////////////////////////////////////////////////////////////

#include "Cumulador.h"
#include "Agent.h"
#include "Itineraris.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCumulador::CCumulador(CTopologia<CSubstrat>& biotop, 
	uint32 densitat, uint32 element, uint32 radio, uint32 variabilitat)
	: m_biotop(biotop)
{
	m_radio=radio;
	m_densitat=densitat;
	m_variabilitat=variabilitat;
	m_element=element;
	m_posicionador=new CItinerariAleatori(m_biotop);
	log << "Inicialitzant Sense Posicionador"<< endl;
}

CCumulador::CCumulador(CTopologia<CSubstrat>& biotop, CPosicionador *pos,
	uint32 densitat, uint32 element, uint32 radio, uint32 variabilitat)
	: m_biotop(biotop)
{
	m_radio=radio;
	m_densitat=densitat;
	m_variabilitat=variabilitat;
	m_element=element;
	m_posicionador=pos;
}

CCumulador::~CCumulador()
{

}

void CCumulador::ProvaClasse()
{
	CTopologiaToroidal<CSubstrat> biotop(70,40);
	CCumulador cum (biotop, new CItinerariAleatori(biotop), 100, 31, 1, 0);
	CAgent<CCumulador*> ag;
	ag.accio(&cum);
	ag.cicleActiu(6);
	ag.cicleDesactiu(2);
	for (int i=30; i--;)
	{
		ag.tic();
		biotop.debugPresenta(out);
		out << blanc << i <<endl;
	}
}

void CClareador::ProvaClasse()
{
	CTopologiaToroidal<CSubstrat> biotop(70,22);
	CPosicionador* posicio = new CItinerariAleatori(biotop);
	CCumulador cum (biotop, posicio, 100, 31, 1, 0);
	CAgent<CCumulador*> ag;
	ag.accio(&cum);
	ag.cicleActiu(6);
	ag.cicleDesactiu(1);
	CAgent<CPosicionador*> agmove;
	agmove.accio(posicio);
	agmove.cicleActiu(20);
	agmove.cicleDesactiu(1);
	for (int i=100; i--;)
	{
		(*posicio)();
		ag.tic();
		biotop.debugPresenta(out);
		out << blanc << i <<endl;
	}
	CClareador cla (biotop, 100, 31, 1,0xFFFFFFFF);
	ag.accio(&cla);
	for (i=30; i--;)
	{
		agmove.tic();
		ag.tic();
		biotop.debugPresenta(out);
		out << blanc << i <<endl;
	}
}
