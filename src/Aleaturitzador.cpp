// Aleaturitzador.cpp: implementation of the CAleaturitzador class.
//
//////////////////////////////////////////////////////////////////////

#include "Aleaturitzador.h"
#include "Color.h"

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
// Virtuals redefinibles a les subclasses
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
	msg << "- Probabilitat: " 
		<< m_probabilitat.m_encerts << " de "
		<< m_probabilitat.m_mostra << endl;
	if (m_reAccio) msg << "- ReAccio: " << m_reAccio->nom() << endl; 
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
	out << "\033[J";// Un clrscr xapuser pero standard (ANSI)
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



