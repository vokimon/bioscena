// Grafic.cpp: implementation of the CGrafic class.
//
//////////////////////////////////////////////////////////////////////

#include "Grafic.h"
#include "Color.h"
#include "Comunitat.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrafic::CGrafic()
{
	m_altura = 22;
	m_amplada = 050;
	m_margeSup = 0;
	m_margeInf = 0;
	m_posX = 41; // 1;
	m_posY = 22;
	recalculaTope();
}

CGrafic::~CGrafic()
{
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

void CGrafic::tamany(uint32 posX, uint32 posY, uint32 amplada, uint32 altura)
{
	m_altura = altura;
	m_amplada = amplada;
	m_posX = posX;
	m_posY = posY;
	recalculaTope();
}

//////////////////////////////////////////////////////////////////////
// Implementacio
//////////////////////////////////////////////////////////////////////

void CGrafic::recalculaTope() 
{
	m_tope = m_altura-m_margeInf-m_margeSup;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComparativaOrganismes::CComparativaOrganismes(CComunitat *comunitat=NULL)
{
	m_comunitat=comunitat;
	m_margeSup = 0;
	m_margeInf = 2;
	m_primerOrg = 0;
	recalculaTope();
}

CComparativaOrganismes::~CComparativaOrganismes()
{

}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

void CComparativaOrganismes::comunitat(CComunitat * com)
{
	m_comunitat=com;
}

CComunitat * CComparativaOrganismes::comunitat(void)
{
	return m_comunitat;
}

void CComparativaOrganismes::primerOrganisme(uint32 org)
{
	m_primerOrg = org;
}

void CComparativaOrganismes::visualitza(CMissatger & msg)
{
	static CDominiGrafica dominiEnergia (true);
	static CDominiGrafica dominiEdat (true);

	dominiEdat.fixaFactor(m_tope);
	dominiEnergia.fixaFactor(m_tope);

	using AnsiCodes::gotoxy;
	using AnsiCodes::clrlin;

	uint32 col, fil;
	uint32 idOrg;
	uint32 colorOrg;

	// Esborrem el fons
	for (fil=m_posY; fil<m_posY+m_tope+m_margeInf; ++fil)
	{
		msg << gotoxy(fil, m_posX) << blanc.fons(blanc) << clrlin;
	}

	if (!m_comunitat)
		return;
	CComunitat & comunitat = *m_comunitat;

	// Coloquem les guies
	if (dominiEdat.premapeja(15)<m_tope) {
		msg << gotoxy(m_posY+m_tope-dominiEdat.mapeja(15), m_posX) << blanc.fons(groc) << clrlin;
		msg << blanc.fons(blanc);
	}
	if (dominiEdat.premapeja(7)<m_tope) {
		msg << gotoxy(m_posY+m_tope-dominiEdat.mapeja(7), m_posX) << blanc.fons(verd) << clrlin;
		msg << blanc.fons(blanc);
	}
	if (dominiEdat.premapeja(31)<m_tope) {
		msg << gotoxy(m_posY+m_tope-dominiEdat.mapeja(31), m_posX) << blanc.fons(vermell) << clrlin;
		msg << blanc.fons(blanc);
	}

	// Imprimim el contingut
	colorOrg=(m_primerOrg>>3)&7;
	if (m_primerOrg&7)
		msg << CColor((colorOrg++)&7).brillant();
	for (idOrg=m_primerOrg,col=m_posX; col<m_posX+m_amplada && idOrg<comunitat.m_organismes.size(); col++,idOrg++)
	{
		// Color identificador
		if (!(idOrg&7)) msg << CColor((colorOrg++)&7).brillant();
		COrganisme * org = comunitat[idOrg].cos();
		if (org) 
		{
			// Posem l'energia
			uint32 energia = org->energia();
			energia = dominiEnergia.mapeja(energia);
			if (energia>m_tope)
				msg << gotoxy(m_posY, col) << '?';
			else
				msg << gotoxy(m_posY+m_tope-energia, col) << '*';

			// Posem l'edat
			uint32 edat    = org->edat();
			edat    = dominiEdat.mapeja(edat);
			if (edat>m_tope)
				msg << gotoxy(m_posY, col) << '!';
			else
				msg << gotoxy(m_posY+m_tope-edat   , col) << '-';
		}
		else {
			msg << gotoxy(m_posY+m_tope, col) << 'X';
		}
	}

	msg << gotoxy(m_posY+m_tope+1, m_posX);
	colorOrg=(m_primerOrg>>3)&7;
	if (m_primerOrg&7)
		msg << CColor((colorOrg++)&7).brillant();
	for (idOrg=m_primerOrg,col=m_posX; col<m_posX+m_amplada && idOrg<comunitat.m_organismes.size(); col++,idOrg++)
	{
		// Color identificador
		if (!(idOrg&7)) msg << CColor((colorOrg++)&7).brillant();
		// Numero identificador
		msg << char((idOrg&7)+'0');
	}

	// Afegim els ids dels taxons
	msg << gotoxy(m_posY+m_tope+2, m_posX);
	for (idOrg=m_primerOrg,col=m_posX; col<m_posX+m_amplada && idOrg<comunitat.m_organismes.size(); col++,idOrg++)
	{
		if (comunitat[idOrg].cos()) {
			msg << negre.fons((comunitat[idOrg].taxo()>>3)&7)
				<< (comunitat[idOrg].taxo()&7);
		}
		else
			msg << blanc.fons(negre) << " ";
	}

	// Restaurem els colors
	msg << blanc.fosc();
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDominiGrafica::CDominiGrafica (bool esLogaritmic)
{
	m_maxim = 0;
	m_factor = 0; 
	m_esDinamic = true;
	m_esLogaritmic = esLogaritmic;
}

CDominiGrafica::CDominiGrafica (bool esLogaritmic, uint32 factor)
{
	m_maxim = 0; 
	m_factor = factor; 
	m_esDinamic = false;
	m_esLogaritmic = esLogaritmic;
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

void CDominiGrafica::fixaFactor (uint32 tope) 
// Si el factor es dinamic, es fixa per que el maxim sigui representable
{
	if (!m_esDinamic) return;
	uint32 factor=m_factor;
	for (m_factor=0; mapeja (m_maxim) > tope; m_factor++);
	if (m_factor<factor) m_factor=factor-1;

/*	
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
*/

	m_maxim=0;
}

uint32 CDominiGrafica::mapeja (uint32 valor) 
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


uint32 CDominiGrafica::premapeja(uint32 valor) 
{
	// Calcul del valor
	if (m_esLogaritmic) {
		uint32 valorLog=0;
		while (valor>>=1) valorLog++;
		valor=valorLog;
		}
	return valor>>m_factor;
}

