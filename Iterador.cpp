// Iterador.cpp: implementation of the CIterador class.
//
//////////////////////////////////////////////////////////////////////

#include "Iterador.h"
#include "RandomStream.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIterador::CIterador()
{
	m_iteracionsMinimes=1;
	m_nombreDaus=0;
	m_magnitudDau=0;
	m_tipus+="/Iterador";
}

CIterador::~CIterador()
{

}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

void CIterador::operator ( )()
{
	uint32 iteracions;
	// Inicialitzem amb les iteracions minimes
	iteracions=m_iteracionsMinimes;
	// I li afegim l'extra dels daus
	for (uint32 i=m_nombreDaus; i--;)
		iteracions+=rnd.get(0,m_magnitudDau);
	for (;iteracions--;)
		CMultiAgent::operator()();
}

void CIterador::iteracions(uint32 iteracions, uint32 magDaus, uint32 nDaus)
{
	m_iteracionsMinimes=iteracions;
	m_nombreDaus=nDaus;
	m_magnitudDau=magDaus;

}

void CIterador::dump(CMissatger & msg)
{
	CMultiAgent::dump(msg);
	msg << "\tIteracions(" 
		<< m_iteracionsMinimes << ", " 
		<< m_nombreDaus << ", " 
		<< m_magnitudDau << ")" << endl;
}
