// Actuadors.cpp: implementation of the CActuador subclasses.
//
//////////////////////////////////////////////////////////////////////

#include "Actuadors.h"
#include "Biosistema.h"
#include "Color.h"

using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInoculador::CInoculador(t_biosistema & biosistema)
	: m_biosistema(biosistema)
{
	m_tipus+="/Inoculador";
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

void CInoculador::operator () () 
{
	COrganisme * org = new COrganisme(m_nomControl);
	if (org)
		m_biosistema.introdueix(org, m_posicionador->pos());
}

void CInoculador::dump(CMissatger & msg)
{
	inherited::dump(msg);
	msg << "- NomControl " << m_nomControl << endl;
}

bool CInoculador::configura(string parametre, istream & valor, t_diccionariAgents & diccionari, CMissatger & errors)
{
	if (parametre=="NomControl") {
		string tipus;
		CControlOrganisme * ctrl = NULL;
		if (!(valor>>tipus))
			errors << "Format invalid pel tipus de l'agent '" << nom() << "'" << endl;
		else if (!(ctrl= CControlOrganisme::Crea(tipus)))
			errors << "El tipus de control d'organisme '" << tipus << "' de l'agent '" << nom() << "' no esta implementat" << endl;
		else {
			nomControl(tipus);
			delete ctrl;
			}
		return true; // Parametre interceptat
	}
	if (parametre=="IdControl") {
		uint32 tipus;
		CControlOrganisme * ctrl = NULL;
		if (!(valor>>tipus))
			errors << "Format invalid pel tipus de l'agent '" << nom() << "'" << endl;
		else if (!(ctrl = CControlOrganisme::Crea(tipus)))
			errors << "El numero de tipus de control d'organisme '" << tipus << "' de l'agent '" << nom() << "' no esta implementat" << endl;
		else {
			nomControl(ctrl->tipus());
			delete ctrl;
			}
		return true; // Parametre interceptat
		}
	// Li deixem a la superclasse que l'intercepti si vol
	return inherited::configura(parametre, valor, diccionari, errors);
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

void CInoculador::nomControl(string tipus)
{
	m_nomControl=tipus;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CInoculador::ProvaClasse()
{
	//TODO: Proves dels inoculadors
	cin.get();
}

