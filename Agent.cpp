// Agent.cpp: implementation of the CAgent class.
//
//////////////////////////////////////////////////////////////////////

#include "Agent.h"
#include "Color.h"

//////////////////////////////////////////////////////////////////////
// Construccio/Desctruccio
//////////////////////////////////////////////////////////////////////

CAgent::CAgent()
{
	// TODO: Que passa quan ja hi ha un nom amb el meu nom per defecte?
	m_numeroAgent=s_ultimNumeroAgent++;
	m_tipus="Agent";
	m_nomDefinit=false;
	ostrstream stream;
	stream 
		<< tipus() << "-" 
		<< setfill('0') << setw(4) << m_numeroAgent 
		<< setfill(' ') << ends;
	nom(stream.str());
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
	msg << groc.brillant() << "Diccionari d'agents" << blanc.fosc() << endl;
	map<string, CAgent*>::iterator it;
	for (it=s_DiccionariAgents.begin(); it!=s_DiccionariAgents.end(); it++)
		msg 
			<< it->first << endl 
			<< "\t" << it->second->nom() 
			<< " (" << it->second->tipus() << ") " << endl;
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
// Seralitzacio
//////////////////////////////////////////////////////////////////////

ostream& operator<< (ostream& str, CAgent& ag) {
	str << ag.nom() << ":" << endl 
		<< "\tTipus(" << ag.tipus() << ")" << endl;
	return str;
}


//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

bool CAgent::nom(string unNom)
{
	// Primer em trec del diccionari amb el nom antic
	if (m_nomDefinit) s_DiccionariAgents.erase(m_nom); 
	map<string, CAgent*>::value_type parell(unNom,this);
	// Si ja existia no fem 
	if (!s_DiccionariAgents.insert(parell).second) 
		return false;
	m_nomDefinit=true;
	m_nom=unNom;
	return true;
}

void CAgent::dumpAll(CMissatger& msg)
{
	list<CAgent*> l=subordinats();
	for (list<CAgent*>::iterator it=l.begin(); it!=l.end(); it++)
		(*it)->dumpAll(msg);
	dump(msg);
}

void CAgent::dump(CMissatger& msg)
{
	msg << endl
		<< nom() << endl
		<< "\tTipus(" << tipus() << ")" << endl
		;
}

//////////////////////////////////////////////////////////////////////
// Proves CFuncioAgent
//////////////////////////////////////////////////////////////////////

static void hola(){out << "Hola mon!"<<endl;}

void CFuncioAgent::ProvaClasse ()
{
	CFuncioAgent ag(hola);
	ag();
	// De fet es una prova molt simpleta :-)
}


