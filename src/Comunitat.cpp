// Comunitat.cpp: implementation of the CComunitat class.
//
//////////////////////////////////////////////////////////////////////

#include <algorithm>
//#include <functional>
#include "Comunitat.h"
#include "Configuracio.h"
#include "RandomStream.h"
#include "Color.h"

//////////////////////////////////////////////////////////////////////
// Variables estatiques
//////////////////////////////////////////////////////////////////////

static bool tracaAltes=false;
static bool tracaBaixes=false;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComunitat::CComunitat()
//	: m_txmist(Config.get("Comunitat/NumeroMarquesPerTaxo"))
{
}

CComunitat::~CComunitat()
{
	vector<CInfoOrganisme>::iterator it = m_organismes.begin();
	for (; it!=m_organismes.end(); ++it)
		it->cos(NULL);
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

void CComunitat::dump(CMissatger & msg)
{
	m_nOrganismes=0;
	msg << "Comunitat de " << (m_organismes.size() - m_disponibles.size()) << " organismes." << endl;
	vector<CInfoOrganisme>::iterator it = m_organismes.begin();
	for (uint32 index=0; it!=m_organismes.end(); ++it, ++index) {
//		if (it->cos()) {
			msg 
				<< groc.brillant() << "* Organisme #" << index << blanc.fosc()
				<< " Posicio: " << it->posicio() 
				<< " Taxo: " << it->taxo() 
				<< endl;
			msg << " Punter " << it->cos() << endl;
//			it->cos()->dump(msg);
//		}
	}
	dumpDisponibles(msg);
}

void CComunitat::dumpDisponibles(CMissatger & msg)
{
/*
	vector<uint32>::iterator it = m_disponibles.begin();
	for (uint32 index=0; it!=m_disponibles.end(); ++it, ++index)
		msg << *it << "-";
*/
	msg << endl;
}

void CComunitat::dumpEnergies(CMissatger & msg)
{
	using AnsiCodes::gotoxy;
	using AnsiCodes::clrlin;
	uint32 col;
	for (uint32 fil=32; fil<44; ++fil)
	{
		msg << gotoxy(fil,1) << blanc.fons(blanc) << clrlin;
	}
	for (col=0; col<79 && col<m_organismes.size(); col++)
	{
		msg << CColor(1+(col>>3)&7).brillant();
		if ((*this)[col].cos()) 
		{
			uint32 energia = m_organismes[col].cos()->energia() >> 2;
			uint32 edat    = m_organismes[col].cos()->edat()    >> 3;
			msg << gotoxy(42-energia, col+1) << '*';
			msg << gotoxy(42-edat   , col+1) << '-';
		}
		else 
			msg << gotoxy(42, col+1) << 'X';
		msg << gotoxy(43, col+1) << (col&7);
	}
	for (col=0; col<79 && col<m_organismes.size(); col++)
	{
		if ((*this)[col].cos()) {
			msg << negre.fons(1+(m_organismes[col].taxo()>>3)&7)
				<< gotoxy(44, col+1) 
				<< (m_organismes[col].taxo()&7);
		}
		else
			msg << blanc.fons(negre)
			    << gotoxy(44, col+1) << " ";
	}
	msg << blanc.fosc();
}


//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

CInfoOrganisme & CComunitat::operator[](uint32 index) {
	return m_organismes[index];
}

uint32 CComunitat::introdueix(COrganisme* org, uint32 posicio, uint32 taxo)
	// PRE: org és un punter vàlid
{
	uint32 index;
	if (m_disponibles.size()) {
		index = m_disponibles.front();
		pop_heap(m_disponibles.begin(), m_disponibles.end(), greater<uint32>());
		m_disponibles.pop_back();
	} 
	else {
		index = m_organismes.size();
		m_organismes.push_back(CInfoOrganisme());
	}
	if (tracaAltes)
		out << "Introduint organisme " << index << endl;
	CInfoOrganisme & nouOrganisme = m_organismes[index];
	nouOrganisme.taxo(taxo);
	nouOrganisme.posicio(posicio);
	nouOrganisme.cos(org);
	++m_nOrganismes;
	return index;
}

void CComunitat::extreu (uint32 index)
	// Pre: index es un organisme anteriorment introduit
{
	if (tracaBaixes)
		out << "Extreient organisme " << index << endl;
	CInfoOrganisme & vellOrganisme = m_organismes[index];
	// Eliminem l'organisme, si encara esta
	vellOrganisme.cos(NULL);
	// Posem el node 'index' disponible
	m_disponibles.push_back(index);
	push_heap(m_disponibles.begin(), m_disponibles.end(), greater<uint32>());
	// TODO: Alliberar espai quan hi hagi molt de lliure
	--m_nOrganismes;
}

uint32 CComunitat::organismeAleatori()
{
	KKEPAssert ((m_organismes.size())>(m_disponibles.size()), 
		"Comunitat: No hi ha organismes per escollir");
	while (true) {
		uint32 org=rnd.get(0,m_organismes.size()-1);
		if (m_organismes[org].cos()) return org;
	}
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CComunitat::ProvaClasse (void)
{
	CComunitat com;
	com.dump(out); cin.get();
	COrganisme * org;
	org = new COrganisme();
	com.introdueix(org,1,11);
	com.dump(out); cin.get();
	org = new COrganisme();
	com.introdueix(org,2,12);
	com.dump(out); cin.get();
	org = new COrganisme();
	com.introdueix(org,3,13);
	com.dump(out); cin.get();
	org = new COrganisme();
	com.introdueix(org,4,14);
	com.dump(out); cin.get();
	org = new COrganisme();
	com.introdueix(org,2,12);
	org = new COrganisme();
	com.introdueix(org,2,12);
	org = new COrganisme();
	com.introdueix(org,2,12);
	org = new COrganisme();
	com.introdueix(org,2,12);
	org = new COrganisme();
	com.introdueix(org,2,12);
	com.dump(out); cin.get();
	com.extreu(com.organismeAleatori());
	com.dump(out); cin.get();
	com.extreu(com.organismeAleatori());
	com.dump(out); cin.get();
	com.extreu(com.organismeAleatori());
	com.extreu(com.organismeAleatori());
	com.extreu(com.organismeAleatori());
	com.extreu(com.organismeAleatori());
	com.extreu(com.organismeAleatori());
	com.extreu(com.organismeAleatori());
	com.extreu(com.organismeAleatori());
//	com.extreu(com.organismeAleatori());
	com.dump(out); cin.get();
	org = new COrganisme();
	com.introdueix(org,5,15);
	com.dump(out); cin.get();
	org = new COrganisme();
	com.introdueix(org,6,16);
	com.dump(out); cin.get();
	org = new COrganisme();
	com.introdueix(org,7,17);
	com.dump(out); cin.get();
}


uint32 CComunitat::tamany()
{
	return m_organismes.size() - m_disponibles.size();
}
