// Actuador.cpp: implementation of the CActuador class.
//
//////////////////////////////////////////////////////////////////////

#include "Actuador.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CActuador::CActuador()
{
	m_tipus+="/Actuador";
	m_posicionador=NULL;
}

CActuador::~CActuador()
{

}

//////////////////////////////////////////////////////////////////////
// Virtuals redefinibles a les subclasses
//////////////////////////////////////////////////////////////////////

void CActuador::operator() (void)
{
	if (!m_posicionador)
		warning << "CActuador " << nom() << " accionat sense posicionador" << endl;
	else
		(*this)(m_posicionador->substrat());
}

void CActuador::dump(CMissatger & msg)
{
	CAgent::dump(msg);
	if (m_posicionador) 
		msg << "- Posicionador: " << m_posicionador->nom() << endl; 
}

list<CAgent*> CActuador::dependencies() {
	list<CAgent*> l=CAgent::dependencies();
	if (m_posicionador) l.push_back(m_posicionador); 
	return l;
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

void CActuador::posicionador (CPosicionador* pos)
{
	m_posicionador=pos;
}

CPosicionador* CActuador::posicionador ()
{
	return m_posicionador;
}

