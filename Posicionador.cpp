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
	// Aixi ens asegurem de que la posicio inicial sigui valida
	m_pos=biotop.posicioAleatoria(); 
};

CDireccionador::CDireccionador(tipus_biotop& biotop)
	:m_biotop(biotop)
{
	m_tipus+="/Direccionador";
	// No tots els direccionadors comencen igual
	m_dir=rnd.get();
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

void CPosicionador::dump(CMissatger & msg)
{
	CAgent::dump(msg);
	msg << "- Posicio " << m_pos << endl;
}

bool CPosicionador::configura(string parametre, istream & valor, t_diccionariAgents & diccionari, CMissatger & errors)
{
	if (parametre=="Posicio") {
		uint32 posicio;
		if (!(valor>>posicio))
			errors << "Format invalid per la posicio radi de '" << nom() << "'" << endl;
		else 
			pos(posicio);
		return true;
	}
	// Li deixem a la superclasse que l'intercepti si vol
	return inherited::configura(parametre, valor, diccionari, errors);
}

void CDireccionador::dump(CMissatger & msg)
{
	CAgent::dump(msg);
	msg << "- Direccio " << m_dir << endl;
}

bool CDireccionador::configura(string parametre, istream & valor, t_diccionariAgents & diccionari, CMissatger & errors)
{
	if (parametre=="Direccio") {
		uint32 direccio;
		if (!(valor>>direccio))
			errors << "Format invalid per la direccio radi de '" << nom() << "'" << endl;
		else 
			dir(direccio);
		return true;
	}
	// Li deixem a la superclasse que l'intercepti si vol
	return inherited::configura(parametre, valor, diccionari, errors);
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

void CPosicionador::pos(tipus_posicio nova) 
{
	if (m_biotop.esValidaCassella(nova))
		m_pos=nova;
	else {
		warning << "Posicionador '" << nom() << "' fixat a la posicio " << nova << ", invalida pel biotop" << endl;
		cin.get();
	}
}

CSubstrat & CPosicionador::substrat()
{
	return m_biotop[m_pos];
}

