// Aleaturitzador.cpp: implementation of the CAleaturitzador class.
//
//////////////////////////////////////////////////////////////////////

#include "Aleaturitzador.h"
#include "FuncioAgent.h"
#include "Color.h"

using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAleaturitzador::CAleaturitzador()
{
	m_tipus+="/Aleaturitzador";
	m_probabilitat.m_encerts=1;
	m_probabilitat.m_mostra=1;
	m_reAccio = NULL;
	m_accionat = false;
}

CAleaturitzador::~CAleaturitzador()
{
	if (m_reAccio) delete m_reAccio;
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

void CAleaturitzador::operator()(void)
{
	m_accionat=false;
	// Nomes si es dona la probabilitat i existeix l'accio, l'executem
	if (m_probabilitat.esDona()) 
	{
		CMultiAgent::operator()();
		m_accionat=true;
	}
	else if (m_reAccio)
		(*m_reAccio)();
}

void CAleaturitzador::dump(CMissatger & msg)
{
	CMultiAgent::dump(msg);
	msg << "- Probabilitat " 
		<< m_probabilitat.m_encerts << " "
		<< m_probabilitat.m_mostra << endl;
	if (m_reAccio) msg << "- ReAccio " << m_reAccio->nom() << endl; 
}

bool CAleaturitzador::configura(string parametre, istream & valor, t_diccionariAgents & diccionari, CMissatger & errors)
{
	if (parametre=="Probabilitat") {
		uint32 mostra, encerts;
		if (!(valor>>encerts))
			errors << "Format invalid per la probabilitat de '" << nom() << "'" << endl;
		else if (!(valor>>mostra))
			errors << "Falta la quantitat de mostra per a la probabilitat de '" << nom() << "'" << endl;
		else 
			probabilitat(mostra, encerts);
		return true; // Parametre interceptat
	}
	if (parametre=="ReAccio") {
		string nomSubordinat;
		t_diccionariAgents::iterator it;
		if (!(valor>>nomSubordinat))
			errors << "Format invalid per a l'especificacio de subordinat de '" << nom() << "'" << endl;
		else if (m_reAccio)
			errors << "L'agent '" << nom() << "' ja tenia una reAccio" << endl;
		else if ((it=diccionari.find(nomSubordinat))==diccionari.end())
			errors << "L'agent subordinat '" << nomSubordinat << "' de l'agent '" << nom() << "' no esta definit al fitxer." << endl;
		else if ((*it).second->subordinant())
			errors << "L'agent subordinat '" << nomSubordinat << "' de l'agent '" << nom() << "' ja esta subordinat a l'agent '" << it->second->subordinant()->nom() << "'." << endl;
		else 
			reAccio(it->second);
		return true; // Parametre interceptat
	}
	// Li deixem a la superclasse que l'intercepti si vol
	return CMultiAgent::configura(parametre, valor, diccionari, errors);
}

list<CAgent*> CAleaturitzador::subordinats() {
	list<CAgent*> l=CMultiAgent::subordinats();
	if (m_reAccio) l.push_back(m_reAccio);
	return l;
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

void CAleaturitzador::probabilitat(uint32 mostra, uint32 encerts)
{
	m_probabilitat.m_mostra=mostra;
	m_probabilitat.m_encerts=encerts;
}

void CAleaturitzador::reAccio(t_accio * a)
{
	m_reAccio=a;
	if (a&&!a->subordinant(this))
		warning << "Subordinant l'agent '" << a->nom() << "' a un segon subordinant" << endl;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

static void Ko () {out << "-";}
static void Ok () {out << "O";}

void CAleaturitzador::ProvaClasse()
{
	out << clrscr;
	out << blanc.brillant() << "Provant Agent Aleaturitzador" << blanc.fosc() << endl;
	int exits=0;
	int intents=0;
	CAleaturitzador aleaturitzador;
	CAgent * ag1= new CFuncioAgent(Ok);
	CAgent * ag2= new CFuncioAgent(Ko);
	aleaturitzador.probabilitat(40,25);
	aleaturitzador.accio(ag1);
	aleaturitzador.reAccio(ag2);
	for (int i=400; i--;) {
		aleaturitzador();
		intents++;
		if (aleaturitzador.m_accionat)
			exits++;
	}
	out << endl << "S'han executat " << exits << " de " << intents << endl;
	aleaturitzador.dumpAll(out);
	cin.get();
}



