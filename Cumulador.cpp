// Cumulador.cpp: implementation of the CCumulador class.
//
//////////////////////////////////////////////////////////////////////

#include "Cumulador.h"
#include "Agent.h"

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
}

CCumulador::~CCumulador()
{

}

void CCumulador::ProvaClasse()
{
	CTopologiaToroidal<CSubstrat> biotop(70,40);
	CCumulador cum (biotop, 100, 31, 1);
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
	CTopologiaToroidal<CSubstrat> biotop(70,40);
	CCumulador cum (biotop, 10, 31, 1);
	CAgent<CCumulador*> ag;
	ag.accio(&cum);
	ag.cicleActiu(6);
	ag.cicleDesactiu(1);
	for (int i=30; i--;)
	{
		ag.tic();
		biotop.debugPresenta(out);
		out << blanc << i <<endl;
	}
	CClareador cla (biotop, 100, 31, 1,0xFFFFFFFF);
	ag.accio(&cla);
	for (int i=30; i--;)
	{
		ag.tic();
		biotop.debugPresenta(out);
		out << blanc << i <<endl;
	}
}
