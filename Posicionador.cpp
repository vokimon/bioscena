// Posicionador.cpp: implementation of the CPosicionador class.
//
//////////////////////////////////////////////////////////////////////

#include "Posicionador.h"

//////////////////////////////////////////////////////////////////////
// Construccio/Destruccio
//////////////////////////////////////////////////////////////////////

CPosicionador::CPosicionador(tipus_biotop& biotop)
	:m_biotop(biotop)
{
	m_tipus+="/Posicionador";
	m_pos=0;
};

CDireccionador::CDireccionador(tipus_biotop& biotop)
	:m_biotop(biotop)
{
	m_tipus+="/Direccionador";
	m_dir=0;
}

//////////////////////////////////////////////////////////////////////
// Virtuals redefinibles a les subclasses
//////////////////////////////////////////////////////////////////////

void CPosicionador::dump(CMissatger & msg)
{
	CAgent::dump(msg);
	msg << "- Posicio: " << m_pos << endl;
}


void CDireccionador::dump(CMissatger & msg)
{
	CAgent::dump(msg);
	msg << "- Direccio: " << m_dir << endl;
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

