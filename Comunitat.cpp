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
/*
	vector<uint32>::iterator it = m_disponibles.begin();
	for (uint32 index=0; it!=m_disponibles.end(); ++it, ++index)
		msg << *it << "-";
*/
	msg << endl;
}

class CDominiGrafica {
private:
	uint32 m_maxim;
	uint32 m_minim;
	uint32 m_factor;
	uint32 m_esDinamic;
	bool m_esLogaritmic;
public:
	CDominiGrafica ()
	{
		m_maxim = 0; // El minimo que se quiere representar
//		m_minim = 0; // El minimo que se quiere representar
		m_factor = 0; 
		m_esDinamic = false;
		m_esLogaritmic = true;
	}
	void fixaFactor (uint32 tope) 
	// Si el factor es dinamic, es fixa per que el maxim sigui representable
	{
		if (!m_esDinamic) return;
		if (m_esLogaritmic) {
			uint32 valorLog=0;
			while (m_maxim) {
				valorLog++;
				m_maxim>>=1;
			}
			m_maxim=valorLog;
			}
		uint32 factor=0;
		while ((m_maxim>>=1)>tope) factor++;
		m_factor = (m_factor>factor) ? (m_factor-1): factor;
		m_maxim=0;
	}
	uint32 mapeja (uint32 valor) 
	{
		m_maxim=(m_maxim<valor)?valor:m_maxim;
		// Calcul del valor
		if (m_esLogaritmic) {
			uint32 valorLog=0;
			while (valor>>=1) valorLog++;
			valor=valorLog;
			}
		return valor>>m_factor;
	}
};

void CComunitat::dumpEnergies(CMissatger & msg)
{
	static uint32 tope = 10;
	static CDominiGrafica dominiEnergia;
	static CDominiGrafica dominiEdat;

	dominiEdat.fixaFactor(tope);
	dominiEnergia.fixaFactor(tope);

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
			uint32 energia = m_organismes[col].cos()->energia();
			energia = dominiEnergia.mapeja(energia);
			if (energia>tope)
				msg << gotoxy(32, col+1) << '?';
			else
				msg << gotoxy(42-energia, col+1) << '*';

			uint32 edat    = m_organismes[col].cos()->edat();
			edat    = dominiEdat.mapeja(edat);
			if (edat>tope)
				msg << gotoxy(32, col+1) << '?';
			else
				msg << gotoxy(42-edat   , col+1) << '-';
		}
		else 
			msg << gotoxy(42, col+1) << 'X';
		msg << gotoxy(43, col+1) << (col&7);
	}
	for (col=0; col<79 && col<m_organismes.size(); col++)
	{
		if ((*this)[col].cos()) {
			msg << negre.fons((m_organismes[col].taxo()>>3)&7)
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
	if (index>=m_organismes.size()) {
		cout << "Accediendo a un organisme no existent!" << endl;
		cin.get();
	}
	return m_organismes[index];
}

uint32 CComunitat::introdueix(COrganisme* org, uint32 posicio, uint32 taxo)
	// PRE: org és un punter vàlid
{
	static char buffer[50]; // TODO: Reconsiderar la longitud o posar-la als limits d'implementacio
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
	ostrstream fluxe; //(buffer,48);
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
