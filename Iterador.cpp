// Iterador.cpp: implementation of the CIterador class.
//
//////////////////////////////////////////////////////////////////////

#include "Iterador.h"
#include "RandomStream.h"
#include "Color.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIterador::CIterador()
{
	m_tipus+="/Iterador";
	m_iteracionsMinimes=1;
	m_nombreDaus=0;
	m_magnitudDau=0;
	m_accionats=0;
	m_preAccio=NULL;
	m_postAccio=NULL;
}

CIterador::~CIterador()
{
	if (m_preAccio) delete m_preAccio;
	if (m_postAccio) delete m_postAccio;
}

//////////////////////////////////////////////////////////////////////
// Virtuals redefinibles a les subclasses
//////////////////////////////////////////////////////////////////////

void CIterador::operator ( )()
{
	uint32 iteracions;
	if (m_preAccio)
		(*m_preAccio)();
	// Inicialitzem amb les iteracions minimes
	iteracions=m_iteracionsMinimes;
	// I li afegim l'extra dels daus
	for (uint32 i=m_nombreDaus; i--;)
		iteracions+=rnd.get(0,m_magnitudDau);
	m_accionats=iteracions;
	for (;iteracions--;)
		CMultiAgent::operator()();
	if (m_postAccio)
		(*m_postAccio)();
}

void CIterador::dump(CMissatger & msg)
{
	CMultiAgent::dump(msg);
	msg << "- Iteracions " 
		<< m_iteracionsMinimes << " " 
		<< m_nombreDaus << " " 
		<< m_magnitudDau << endl;
	if (m_preAccio) msg << "- PreAccio: " << m_preAccio->nom() << endl; 
	if (m_postAccio) msg << "- PostAccio: " << m_postAccio->nom() << endl; 
}

bool CIterador::configura(string parametre, istream & valor, t_diccionariAgents & diccionari, CMissatger & errors)
{
	if (parametre=="Iteracions") {
		uint32 minim, nDaus, magDau;
		if (!(valor>>minim))
			errors << "Format invalid pel nombre d'iteracions de '" << nom() << "'" << endl;
		else if (!(valor>>nDaus))
			errors << "Falta la quantitat de daus pel nombre d'iteracions de '" << nom() << "'" << endl;
		else if (!(valor>>magDau))
			errors << "Falta la magnitud dels daus pel nombre d'iteracions de '" << nom() << "'" << endl;
		else 
			iteracions(minim, magDau, nDaus);
		return true;
	}
	if (parametre=="PreAccio") {
		string nomSubordinat;
		t_diccionariAgents::iterator it;
		if (!(valor>>nomSubordinat))
			errors << "Format invalid per a l'especificacio de subordinat de '" << nom() << "'" << endl;
		else if (m_preAccio)
			errors << "L'agent '" << nom() << "' ja tenia una preAccio" << endl;
		else if ((it=diccionari.find(nomSubordinat))==diccionari.end())
			errors << "L'agent subordinat '" << nomSubordinat << "' de l'agent '" << nom() << "' no esta definit al fitxer." << endl;
		else if ((*it).second->subordinant())
			errors << "L'agent subordinat '" << nomSubordinat << "' de l'agent '" << nom() << "' ja esta subordinat a l'agent '" << it->second->subordinant()->nom() << "'." << endl;
		else 
			preAccio(it->second);
		return true; // Parametre interceptat
	}
	if (parametre=="PostAccio") {
		string nomSubordinat;
		t_diccionariAgents::iterator it;
		if (!(valor>>nomSubordinat))
			errors << "Format invalid per a l'especificacio de subordinat de '" << nom() << "'" << endl;
		else if (m_postAccio)
			errors << "L'agent '" << nom() << "' ja tenia una postAccio" << endl;
		else if ((it=diccionari.find(nomSubordinat))==diccionari.end())
			errors << "L'agent subordinat '" << nomSubordinat << "' de l'agent '" << nom() << "' no esta definit al fitxer." << endl;
		else if ((*it).second->subordinant())
			errors << "L'agent subordinat '" << nomSubordinat << "' de l'agent '" << nom() << "' ja esta subordinat a l'agent '" << it->second->subordinant()->nom() << "'." << endl;
		else 
			postAccio(it->second);
		return true; // Parametre interceptat
	}
	// Li deixem a la superclasse que l'intercepti si vol
	return CMultiAgent::configura(parametre, valor, diccionari, errors);
}

list<CAgent*> CIterador::subordinats() {
	list<CAgent*> l=CMultiAgent::subordinats();
	if (m_preAccio) l.push_back(m_preAccio);
	if (m_postAccio) l.push_back(m_postAccio);
	return l;
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

void CIterador::iteracions(uint32 iteracions, uint32 magDaus, uint32 nDaus)
{
	m_iteracionsMinimes=iteracions;
	m_nombreDaus=nDaus;
	m_magnitudDau=magDaus;
}

void CIterador::preAccio(t_accio * a)
{
	m_preAccio=a;
	if (a&&!a->subordinant(this))
		warning << "Subordinant l'agent '" << a->nom() << "' a un segon subordinant" << endl;
}

void CIterador::postAccio(t_accio * a)
{
	m_postAccio=a;
	if (a&&!a->subordinant(this))
		warning << "Subordinant l'agent '" << a->nom() << "' a un segon subordinant" << endl;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

static void Pre () {out << "(";}
static void Post () {out << ")";}
static void Ok () {out << "O";}
static void Ok2 () {out << "E";}

void CIterador::ProvaClasse()
{
	out << "\033[J";// Un clrscr xapuser pero standard (ANSI)
	out << blanc.brillant() << "Provant Agent Iterador" << blanc.fosc() << endl;
	int exits=0;
	int intents=0;
	CIterador iterador;
	CAgent * ag1= new CFuncioAgent(Ok);
	CAgent * ag1b= new CFuncioAgent(Ok2);
	CAgent * ag2= new CFuncioAgent(Pre);
	CAgent * ag3= new CFuncioAgent(Post);
	iterador.iteracions(2,4,2);
	iterador.accio(ag1);
	iterador.accio(ag1b);
	iterador.preAccio(ag2);
	iterador.postAccio(ag3);
	for (int i=20; i--;) {
		intents++;
		iterador();
		exits+=iterador.m_accionats;
	}
	out << endl << "Els subordinats s'han accionat " 
		<< exits << " cops per " 
		<< intents << " accionats del iterador" << endl;
	iterador.dumpAll(out);
	cin.get();
}
