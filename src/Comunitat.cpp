// Comunitat.cpp: implementation of the CComunitat class.
//
//////////////////////////////////////////////////////////////////////

#include <algorithm>
//#include <functional>
#include "Comunitat.h"
#include "Configuracio.h"
#include "RandomStream.h"
#include "Color.h"
//#include "Grafic.h"

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
	m_nOrganismes=0;
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

	vector<uint32>::iterator it = m_disponibles.begin();
	for (uint32 index=0; it!=m_disponibles.end(); ++it, ++index)
		msg << *it << "-";

	msg << endl;
}

void CComunitat::dumpEnergies(CMissatger & msg)
{
/*
	static CDominiGrafica dominiEnergia (true);
	static CDominiGrafica dominiEdat (true);
	static uint32 altura = 22;
	static uint32 amplada = 050;
	static uint32 margeSup = 0;
	static uint32 margeInf = 2;
	static uint32 posX = 41; // 1;
	static uint32 posY = 22;
	static uint32 tope = altura-margeInf-margeSup;
	static uint32 primerOrg = 0100;

	dominiEdat.fixaFactor(tope);
	dominiEnergia.fixaFactor(tope);

	using AnsiCodes::gotoxy;
	using AnsiCodes::clrlin;

	uint32 col, fil;
	uint32 idOrg;

	// Esborrem el fons
	for (fil=posY; fil<posY+tope+margeInf; ++fil)
	{
		msg << gotoxy(posX,fil) << blanc.fons(blanc) << clrlin;
	}

	// Coloquem les guies
	if (dominiEdat.premapeja(15)<tope) {
		msg << gotoxy(posX, posY+tope-dominiEdat.mapeja(15)) << blanc.fons(groc) << clrlin;
		msg << blanc.fons(blanc);
	}
	if (dominiEdat.premapeja(7)<tope) {
		msg << gotoxy(posX, posY+tope-dominiEdat.mapeja(7)) << blanc.fons(verd) << clrlin;
		msg << blanc.fons(blanc);
	}
	if (dominiEdat.premapeja(31)<tope) {
		msg << gotoxy(posX, posY+tope-dominiEdat.mapeja(31)) << blanc.fons(vermell) << clrlin;
		msg << blanc.fons(blanc);
	}

	// Imprimim el contingut

	for (idOrg=primerOrg,col=posX; col<posX+amplada && idOrg<m_organismes.size(); col++,idOrg++)
	{
		// Color identificador
		msg << CColor(1+(idOrg>>3)&7).brillant();
		COrganisme * org = m_organismes[idOrg].cos();
		if (org) 
		{
			// Posem l'energia
			uint32 energia = org->energia();
			energia = dominiEnergia.mapeja(energia);
			if (energia>tope)
				msg << gotoxy(col, posY) << '?';
			else
				msg << gotoxy(col, posY+tope-energia) << '*';

			// Posem l'edat
			uint32 edat    = org->edat();
			edat    = dominiEdat.mapeja(edat);
			if (edat>tope)
				msg << gotoxy(col, posY) << '!';
			else
				msg << gotoxy(col, posY+tope-edat) << '-';
		}
		else {
			msg << gotoxy(col, posY+tope) << 'X';
		}
		// Numero identificador
		msg << gotoxy(col, posY+tope+1) << (idOrg&7);
	}

	// Afegim els ids dels taxons
	msg << gotoxy(posX, posY+tope+2);
	for (idOrg=primerOrg,col=posX; col<posX+amplada && idOrg<m_organismes.size(); col++,idOrg++)
	{
		if ((*this)[idOrg].cos()) {
			msg << negre.fons((m_organismes[idOrg].taxo()>>3)&7)
				<< (m_organismes[idOrg].taxo()&7);
		}
		else
			msg << blanc.fons(negre) << " ";
	}

	// Restaurem els colors
	msg << blanc.fosc();
*/
}


//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

CInfoOrganisme & CComunitat::operator[](uint32 index) {
	if (index>=m_organismes.size()) {
		cout << "Accediendo a un organisme no existent!" << endl;
		cin.get();
	}
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
	ostrstream fluxe;
	fluxe
		<< setfill('0') 
		<< (index>>6) << CColor(1+((index&070)>>3)).brillant() << (index&07) << blanc.fosc() << "-" 
		<< setw(3) << nouOrganisme.subidentificador() << "-" 
		<< (taxo>>6) << (taxo&070?negre.fons((taxo&070)>>3):blanc) << (taxo&7) << blanc.fosc() << " "
		<< setfill(' ') 
		<< ends; // El fluxe no afegeix un /0 sino ho fem nosaltres
	nouOrganisme.descripcio(fluxe.str());
	fluxe.freeze(false);
	++m_nOrganismes;
	return index;
}

void CComunitat::extreu (uint32 index)
	// Pre: index es un organisme anteriorment introduit
{
	if (tracaBaixes)
		out << "Extreient organisme " << index << endl;
//	CInfoOrganisme & vellOrganisme = m_organismes[index];
	// Eliminem l'organisme, si encara esta
	m_organismes[index].cos(NULL);
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
		if (org>=m_organismes.size()) {
			error << "Error escogiendo organismo" << endl;
			cin.get();
		}
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

bool CComunitat::esValid(uint32 index)
{
	return index<m_organismes.size() && m_organismes[index].cos()!=0;
}
