// MultiAgent.cpp: implementation of the CMultiAgent class.
//
//////////////////////////////////////////////////////////////////////

#include <fstream>
#include <algorithm>
#include "MultiAgent.h"
#include "Missatger.h"
#include "TopologiaToroidal.h"
#include "Biotop.h"
#include "Substrat.h"
#include "Itineraris.h"
#include "Actuadors.h"
#include "Iterador.h"
#include "Temporitzador.h"
#include "Color.h"

using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Construccio/Destruccio
//////////////////////////////////////////////////////////////////////

CMultiAgent::CMultiAgent()
{
	m_tipus+="/Multiple";
}

CMultiAgent::~CMultiAgent()
{
	while (!m_agents.empty()) {
		delete (*(m_agents.begin()));
		m_agents.pop_front();
	}
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

void CMultiAgent::operator() (void){
	for (list<CAgent*>::iterator it=m_agents.begin(); it!=m_agents.end(); it++)
		(**it)();
};

void CMultiAgent::dump(CMissatger & msg)
{
	CAgent::dump(msg);
	for (list<CAgent*>::iterator it=m_agents.begin(); it!=m_agents.end(); it++)
	{
		msg	<< "- Accio " << (*it)->nom() << endl;
	}
}

bool CMultiAgent::configura(string parametre, istream & valor, t_diccionariAgents & diccionari, CMissatger & errors)
{
	if (parametre=="Accio") {
		string nomSubordinat;
		t_diccionariAgents::iterator it;
		if (!(valor>>nomSubordinat))
			errors << "Format invalid per a l'especificacio de subordinat de '" << nom() << "'" << endl;
		else if ((it=diccionari.find(nomSubordinat))==diccionari.end())
			errors << "L'agent subordinat '" << nomSubordinat << "' de l'agent '" << nom() << "' no esta definit al fitxer." << endl;
		else if ((*it).second->subordinant())
			errors << "L'agent subordinat '" << nomSubordinat << "' de l'agent '" << nom() << "' ja esta subordinat a l'agent '" << it->second->subordinant()->nom() << "'." << endl;
		else 
			accio(it->second);
		return true; // Parametre interceptat
	}
	if (parametre=="ArxiuSubordinat") {
		string nomArxiuSubordinat;
		CAgent * accioArxiu=NULL;
		if (!(valor>>nomArxiuSubordinat))
			errors << "Format invalid per a l'especificacio de un fitxer subordinat de '" << nom() << "'" << endl;
		else if (!(accioArxiu/*=CAgent::ParsejaArxiu(nomArxiuSubordinat, m_biotop, errors)*/))
			errors << "El fitxer '" << nomArxiuSubordinat << "' no conte una estructura d'agents minimament valida per subordinar-ho a '" << nom() << "'" << endl;
		else accio(accioArxiu);
	}
	// Li deixem a la superclasse que l'intercepti si vol
	return inherited::configura(parametre, valor, diccionari, errors);
}

list<CAgent*> CMultiAgent::subordinats() {
	list<CAgent*> l=CAgent::subordinats();
	for (list<CAgent*>::iterator it=m_agents.begin(); it!=m_agents.end(); it++)
		l.push_back(*it);
	return l;
};

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

void CMultiAgent::accio(t_accio * a) 
{
	m_agents.push_back(a);
	if (a&&!a->subordinant(this))
		warning << "Subordinant l'agent '" << a->nom() << "' a un segon subordinant" << endl;
}

bool CMultiAgent::desubordina(CAgent * ag)
{
	t_accions::iterator it = find(m_agents.begin(), m_agents.end(), ag);
	if (it==m_agents.end()) return false;
	m_agents.remove(ag);
	return true;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CMultiAgent::ProvaClasse()
{
	out << clrscr;
	out << blanc.brillant() << "Provant MultiAgent" << blanc.fosc() << endl;
	CTopologiaToroidal topo(40,20);//(70,24);
	CBiotop<CSubstrat> biotop(&topo);

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
	CActuador *actuador1 = new CNutridor(biotop);
	actuador1->posicionador(posicio1);
	((CNutridor*)actuador1)->composicio (31, 0);
	// L'iterador diu quantes vegades actua per accionat
	CIterador * iterador1 = new CIterador;
	iterador1->iteracions(6,0,0);
	iterador1->accio(actuador1);
	agents.accio(iterador1);

	// Posicionador
	CPosicionador* posicioCentral = new CPosicionador(biotop);
	posicioCentral->pos(685);
	agents.accio(posicioCentral);

	CPosicionador* posicio2 = new CPosicionadorZonal(biotop);
	((CPosicionadorZonal*)posicio2)->posicionador(posicioCentral);
	((CPosicionadorZonal*)posicio2)->radi(3);
	agents.accio(posicio2);

	CIterador * iterador2 = new CIterador;
	iterador2->iteracions(10,10,2);
	agents.accio(iterador2);

	CPosicionador* distribucio2 = new CPosicionadorZonal(biotop);
	((CPosicionadorZonal*)distribucio2)->posicionador(posicio2);
	((CPosicionadorZonal*)distribucio2)->radi(1);
	iterador2->accio(distribucio2);

	CActuador *actuador2 = new CDesnutridor(biotop);
	actuador2->posicionador(distribucio2);
	((CNutridor*)actuador2)->composicio (31, 0	);
	iterador2->accio(actuador2);

	for (int i=40; i--;)
	{
		agents();
		if (!(i%5)) {
			biotop.debugPresenta(out);
			out << blanc.fosc() << setw(4) << i <<endl;
		}
	}
/*	agents.desubordina(iterador1);
	cin.get();
	for (int i=40; i--;)
	{
		agents();
		if (!(i%5)) {
			biotop.debugPresenta(out);
			out << blanc.fosc() << setw(4) << i <<endl;
		}
	}
*/
	ofstream file("AgentsLog.txt");
	CColorOutputer miout(file);
	CMissatger msg(NULL, NULL, miout);
	agents.dumpAll(msg);
	agents.dumpAll(out);
	cin.get();
}

