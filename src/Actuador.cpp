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
		msg << "- Posicionador " << m_posicionador->nom() << endl; 
}

bool CActuador::configura(string parametre, istream & valor, t_diccionariAgents & diccionari, CMissatger & errors)
{
	if (parametre=="Posicionador") {
		string nomDependencia;
		t_diccionariAgents::iterator it;
		if (!(valor>>nomDependencia))
			errors << "Format invalid per a l'especificacio del posicionador de '" << nom() << "'" << endl;
		else if ((it=diccionari.find(nomDependencia))==diccionari.end())
			errors << "El posicionador '" << nomDependencia << "' de l'agent '" << nom() << "' no esta definit al fitxer." << endl;
		else 
			posicionador((CPosicionador*)it->second);
		return true; // Parametre interceptat
	}
	// Li deixem a la superclasse que l'intercepti si vol
	return CAgent::configura(parametre, valor, diccionari, errors);
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

