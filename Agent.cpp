// Agent.cpp: implementation of the CAgent class.
//
//////////////////////////////////////////////////////////////////////

#include <algorithm>
#include "Agent.h"
#include "Color.h"

//////////////////////////////////////////////////////////////////////
// Construccio/Desctruccio
//////////////////////////////////////////////////////////////////////

CAgent::CAgent()
{
	m_tipus="Agent";
	m_numeroAgent=s_ultimNumeroAgent++;
	m_nomDefinit=false;
	ostrstream stream;
	stream 
		<< tipus() << " " 
		<< setfill('0') << setw(4) << m_numeroAgent 
		<< setfill(' ') << ends;
	nom(stream.str());
	m_subordinant=NULL;
};

CAgent::~CAgent() 
{
	if (m_nomDefinit) s_DiccionariAgents.erase(m_nom); 
};

//////////////////////////////////////////////////////////////////////
// Definicio de membres estatics
//////////////////////////////////////////////////////////////////////

uint32 CAgent::s_ultimNumeroAgent=0;
map<string, CAgent*> CAgent::s_DiccionariAgents;

void CAgent::dumpDiccionari(CMissatger& msg) 
{
//	msg << groc.brillant() << "Diccionari d'agents" << blanc.fosc() << endl;
	map<string, CAgent*>::iterator it;
	for (it=s_DiccionariAgents.begin(); it!=s_DiccionariAgents.end(); it++)
		msg << "* " << it->second->nom() << " " << it->second->tipus() << endl;
}

CAgent* CAgent::cercaDiccionari(string s) 
{
	map<string, CAgent*>::iterator it;
	it=s_DiccionariAgents.find(s); 
	if (it==s_DiccionariAgents.end())
		return NULL;
	return it->second;
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

bool CAgent::nom(string unNom)
{
	// Primer em trec del diccionari amb el nom antic
	if (m_nomDefinit) s_DiccionariAgents.erase(m_nom); 
	replace(unNom.begin(), unNom.end(), ' ', '_');
	replace(unNom.begin(), unNom.end(), '\"', '\'');
	map<string, CAgent*>::value_type parell(unNom,this);
	// Si ja existia no fem 
	if (!s_DiccionariAgents.insert(parell).second) 
		return false;
	m_nomDefinit=true;
	m_nom=unNom;
	return true;
}

bool CAgent::subordinant(CAgent * ag)
{
	if (m_subordinant&&ag) // Ep! Que ja en tinc pare!
		return false;
	m_subordinant=ag;
	return true;
}

CAgent * CAgent::subordinant(void)
{
	return m_subordinant;
}

void CAgent::dumpAll(CMissatger& msg) 
{
	dumpTipus(msg);
	dumpConfiguracions(msg);
}

void CAgent::dumpConfiguracions(CMissatger& msg)
{
	list<CAgent*> l=subordinats();
	for (list<CAgent*>::iterator it=l.begin(); it!=l.end(); it++)
		(*it)->dumpConfiguracions(msg);
	dump(msg);
}

void CAgent::dumpTipus(CMissatger& msg)
{
	msg << "* " << nom() << " " << tipus() << endl;
	list<CAgent*> l=subordinats();
	for (list<CAgent*>::iterator it=l.begin(); it!=l.end(); it++)
		(*it)->dumpTipus(msg);
}


void CAgent::dump(CMissatger& msg)
{
	msg << endl
		<< "+ " << nom() << endl
//		<< "- Tipus: " << tipus() << endl
		;
}

//////////////////////////////////////////////////////////////////////
// Proves CFuncioAgent
//////////////////////////////////////////////////////////////////////

static void hola(){out << "Hola mon!"<<endl;}

void CFuncioAgent::ProvaClasse ()
{
	out << "\033[J";// Un clrscr xapuser pero standard (ANSI)
	out << blanc.brillant() << "Provant Agent Funcio Usuari" << blanc.fosc() << endl;
	CFuncioAgent ag(hola);
	ag();
	// De fet es una prova molt simpleta :-)
	ag.dumpAll(out);
	cin.get();
}

