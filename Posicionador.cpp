// Posicionador.cpp: implementation of the CPosicionador class.
//
//////////////////////////////////////////////////////////////////////

#include "Posicionador.h"

//////////////////////////////////////////////////////////////////////
// Construccio/Destruccio
//////////////////////////////////////////////////////////////////////

CPosicionador::CPosicionador(tipus_biotop& biotop):m_biotop(biotop)
{
	m_pos=0;
	m_tipus+="/Posicionador";
};

CPosicionador::~CPosicionador()
{

}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

void CPosicionador::pos(tipus_posicio nova) 
{
	if (m_biotop.esValidaCassella(nova))
		m_pos=nova;
	else
		warning << "Posicionador '" << nom() << "' fixat a una posicio invalida del biotop" << endl;
}

CSubstrat & CPosicionador::substrat()
{
	return m_biotop[m_pos];
}
