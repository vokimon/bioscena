// Grafic.cpp: implementation of the CGrafic class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19991107 VoK - Creat de l'unio d'algunes funcions de visualitzacio
// 19991107 VoK - Doble constructor de DominiGrafic
// 19991117 VoK - Diferenciem CGrafic de CComparativaOrganismes
// 19991118 VoK - Mapa es un nou objete Grafic
// 19991118 VoK - Mapa: Tamanys de mapa i topologia independents
// 19991119 VoK - Mapa: Origen movil -> escroll
// 20000217 VoK - Fix: ScrollDown no definia be els limits
//////////////////////////////////////////////////////////////////////
// TODO:
// Fer mes prima l'escala logaritmica
// Fer programable el que visualitzen les comparatives d'organismes
// Fer programable el que visualitzen els mapes
// Implementar Grafic Evolutiu/Temporal
// Muntar un conjunt de visualitzadors des d'un fitxer
// Implementar Comparativa Taxons

#include "Grafic.h"
#include "Color.h"
#include "Comunitat.h"
#include "TopologiaToroidal.h"

//////////////////////////////////////////////////////////////////////
// (CGrafic) Construction/Destruction
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
// (CGrafic) Operacions
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
// (CGrafic) Implementacio
//////////////////////////////////////////////////////////////////////

void CGrafic::recalculaTope() 
{
	m_tope = m_altura-m_margeInf-m_margeSup;
}


//////////////////////////////////////////////////////////////////////
// (CComparativaOrganismes) Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComparativaOrganismes::CComparativaOrganismes(CComunitat *comunitat=NULL)
	: m_dominiEnergia (true)
	, m_dominiEdat (true)
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
// (CComparativaOrganismes) Operacions
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
	m_dominiEdat.fixaFactor(m_tope);
	m_dominiEnergia.fixaFactor(m_tope);

	using AnsiCodes::gotoxy;
	using AnsiCodes::clrlin;

	uint32 col, fil;
	uint32 idOrg;
	uint32 colorOrg;

	// Esborrem el fons
	for (fil=m_posY; fil<m_posY+m_tope+m_margeInf; ++fil)
	{
		msg << gotoxy(m_posX, fil) << blanc.fons(blanc) << clrlin;
	}

	if (!m_comunitat)
		return;
	CComunitat & comunitat = *m_comunitat;

	// Coloquem les guies
	if (m_dominiEdat.premapeja(0xFF)<m_tope) {
		msg << gotoxy(m_posX, m_posY+m_tope-m_dominiEdat.mapeja(0xFF)) << blanc.fons(verd) << clrlin;
		msg << blanc.fons(blanc);
	}
	if (m_dominiEdat.premapeja(0xFFFF)<m_tope) {
		msg << gotoxy(m_posX, m_posY+m_tope-m_dominiEdat.mapeja(0xFFFF)) << blanc.fons(groc) << clrlin;
		msg << blanc.fons(blanc);
	}
	if (m_dominiEdat.premapeja(0x0FFFFFFF)<m_tope) {
		msg << gotoxy(m_posX, m_posY+m_tope-m_dominiEdat.mapeja(0xFFFFFFFF)) << blanc.fons(vermell) << clrlin;
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
			energia = m_dominiEnergia.mapeja(energia);
			if (energia>m_tope)
				msg << gotoxy(col, m_posY) << '?';
			else
				msg << gotoxy(col, m_posY+m_tope-energia) << '*';

			// Posem l'edat
			uint32 edat    = org->edat();
			edat    = m_dominiEdat.mapeja(edat);
			if (edat>m_tope)
				msg << gotoxy(col, m_posY) << '!';
			else
				msg << gotoxy(col, m_posY+m_tope-edat) << '-';
		}
		else {
			msg << gotoxy(col, m_posY+m_tope) << 'X';
		}
	}

	msg << gotoxy(m_posX, m_posY+m_tope+1);
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
	msg << gotoxy(m_posX, m_posY+m_tope+2);
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
// (CGraficaEvolutiva) Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraficaEvolutiva::CGraficaEvolutiva(CBiosistema *bio=NULL)
{
	m_biosistema=bio;
	m_margeSup = 0;
	m_margeInf = 2;
	recalculaTope();
}

CGraficaEvolutiva::~CGraficaEvolutiva()
{

}

//////////////////////////////////////////////////////////////////////
// (CComparativaOrganismes) Operacions
//////////////////////////////////////////////////////////////////////

void CGraficaEvolutiva::biosistema(CBiosistema * bio)
{
	m_biosistema=bio;
}

CBiosistema * CGraficaEvolutiva::biosistema(void)
{
	return m_biosistema;
}

void CGraficaEvolutiva::visualitza(CMissatger & msg)
{
/*	static CDominiGrafica domini (true);

	domini.fixaFactor(m_tope);

	using AnsiCodes::gotoxy;
	using AnsiCodes::clrlin;

	uint32 col, fil;

	// Esborrem el fons
	for (fil=m_posY; fil<m_posY+m_tope+m_margeInf; ++fil)
	{
		msg << gotoxy(m_posX, fil) << blanc.fons(blanc) << clrlin;
	}

	if (!m_biosistema)
		return;
	CBiosistema & biosistema = * m_biosistema;
	if (!biosistema.comunitat())
		return;
	CComunitat & comunitat = * biosistema.comunitat();

	// Coloquem les guies
	if (domini.premapeja(15)<m_tope) {
		msg << gotoxy(m_posX, m_posY+m_tope-domini.mapeja(15)) << blanc.fons(groc) << clrlin;
		msg << blanc.fons(blanc);
	}
	if (domini.premapeja(7)<m_tope) {
		msg << gotoxy(m_posX, m_posY+m_tope-domini.mapeja(7)) << blanc.fons(groc) << clrlin;
		msg << blanc.fons(blanc);
	}
	if (domini.premapeja(31)<m_tope) {
		msg << gotoxy(m_posX, m_posY+m_tope-domini.mapeja(31)) << blanc.fons(groc) << clrlin;
		msg << blanc.fons(blanc);
	}

	// Imprimim el contingut
	uint32 col;
	uint32 time;
	for (col=m_posX,time=m_actual; col>
	colorOrg=(m_primerOrg>>3)&7;
	if (m_primerOrg&7)
		msg << CColor((colorOrg++)&7).brillant();
	biosistema->comunitat()->tamany();
	for (time=0,col=m_posX; col<m_posX+m_amplada && i<domini.size(); col++,i++,time--)
	{
		if (!time) time = domini.
		// Posem l'energia
		uint32 valor = domini[time];
		valor = dominiEnergia.mapeja(valor);
		if (valor>m_tope)
			msg << gotoxy(col, m_posY) << '?';
		else
			msg << gotoxy(col, m_posY+m_tope-valor) << '*';
	}
	// Restaurem els colors
	msg << blanc.fosc();
*/
}

//////////////////////////////////////////////////////////////////////
// (CDominiGrafica) Construction/Destruction
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
// (CDominiGrafica) Operacions
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

//////////////////////////////////////////////////////////////////////
// (CMapa) Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapa::CMapa(CBiosistema * biosist=NULL)
{
	biosistema(biosist);
	m_altura = 20;
	m_amplada = 20;
	m_margeSup = 0;
	m_margeInf = 0;
	m_posX = 1;
	m_posY = 2;
	recalculaTope();
}

CMapa::~CMapa()
{
}

void CMapa::visualitza(CMissatger & msg)
{
	if (!m_biosistema) return;
	if (!m_biosistema->biotop()) return;

	uint32 posY=m_posY;
	uint32 cellaIniciLinia=m_primeraPosicio;
	bool ultimOcupat=false;

	for (uint32 lin=m_altura; lin--; posY++) {
		// Ajustem la cella d'inici de la linia a disn del biotop
		while (cellaIniciLinia>=m_totalCelles) 
			cellaIniciLinia-=m_totalCelles;
		// Ens posicionem al inici de la linia
		msg << blanc.fosc().fons(negre);
//		msg << negre.fosc();
		msg << gotoxy(m_posX, posY); 
		uint32 cella=cellaIniciLinia;
		for (uint32 col=m_amplada; col--;) {
			if (cella>=m_totalCelles) cella=0;
//			msg << (*m_biotop)[cella];
/*
			if ((*m_biotop)[cella].esOcupat()) 
			{
				char tmp[]="\033[40;37;1m ";
				uint32 ocupant = (*m_biotop)[cella].ocupant();
				tmp[ 3] = '0'+((*m_biotop)[cella].numeroMollecules()&7);
				tmp[ 6] = char('0'+((ocupant>>3)&7));
				tmp[10] = char('0'+(ocupant&7));
				msg << tmp;
			}
			else 
			{
				char tmp[]="\033[40m ";
				tmp[3] = '0'+((*m_biotop)[cella].numeroMollecules()&7);
				msg << tmp;
			}

*/
			if ((*m_biotop)[cella].esOcupat())
			{
				if (ultimOcupat) {
					char tmp[] = "\033[40m ";
					uint32 taxo = (*(m_biosistema->comunitat())) [(*m_biotop)[cella].ocupant()].taxo();
					tmp[3] = '0'+((taxo>>3)&7);
					tmp[5] = '0'+(taxo&7);
					msg << tmp;
				}
				else
				{
					char tmp[] = "\033[40;30m ";
					uint32 taxo = (*(m_biosistema->comunitat())) [(*m_biotop)[cella].ocupant()].taxo();
					tmp[3] = '0'+((taxo>>3)&7);
					tmp[8] = '0'+(taxo&7);
					msg << tmp;
					ultimOcupat=true;
				}
			}
			else
			{
				if (ultimOcupat) {
					char tmp[] = "\033[40;37m,";
					tmp[6] = '0'+((*m_biotop)[cella].numeroMollecules()&7);
					msg << tmp;
					ultimOcupat=false;
				} else {
					char tmp[] = "\033[37m,";
					tmp[3] = '0'+((*m_biotop)[cella].numeroMollecules()&7);
					msg << tmp;
				}
			}

			cella++;
		}
		cellaIniciLinia+=m_ampladaZona;
	}		
	msg << blanc.fosc();
}

void CMapa::scrollUp(uint32 steps)
{
	if (!m_biosistema) return;
	if (!m_biotop) return;

	steps%=m_alturaZona;
	while (steps--) {
		if (m_primeraPosicio<m_ampladaZona)
			m_primeraPosicio+=m_totalCelles-m_ampladaZona;
		else 
			m_primeraPosicio-=m_ampladaZona;
	}
}

void CMapa::scrollDown(uint32 steps)
{
	if (!m_biosistema) return;
	if (!m_biotop) return;

	steps%=m_alturaZona;
	while (steps--) {
		if (m_primeraPosicio>=m_totalCelles-m_ampladaZona)
			m_primeraPosicio=m_ampladaZona-(m_totalCelles-m_primeraPosicio);
		else 
			m_primeraPosicio+=m_ampladaZona;
	}
}

void CMapa::scrollRight(uint32 steps)
{
	if (!m_biosistema) return;
	if (!m_biotop) return;

	steps%=m_ampladaZona;
	while (steps--) {
		m_primeraPosicio++;
		if (m_primeraPosicio>=m_totalCelles)
			m_primeraPosicio-=m_totalCelles;
	}
}

void CMapa::scrollLeft(uint32 steps)
{
	if (!m_biosistema) return;
	if (!m_biotop) return;

	steps%=m_ampladaZona;
	while (steps--) {
		if (!m_primeraPosicio)
			m_primeraPosicio=m_totalCelles;
		m_primeraPosicio--;
	}
}

void CMapa::scrollPageDown(uint32 steps)
{
	// Hem de baixar tantes vegades com la altura del mapa
	scrollDown(m_altura);
}

void CMapa::scrollPageUp(uint32 steps)
{
	// Hem de baixar tantes vegades com la altura del mapa
	scrollUp(m_altura);
}

void CMapa::scrollPageRight(uint32 steps)
{
	// Hem de baixar tantes vegades com la altura del mapa
	scrollRight(m_amplada);
}

void CMapa::scrollPageLeft(uint32 steps)
{
	// Hem de baixar tantes vegades com la altura del mapa
	scrollLeft(m_amplada);
}

void CMapa::primeraPosicio(uint32 pos)
{
	if (pos<m_totalCelles)
		m_primeraPosicio=pos;
}

uint32 CMapa::primeraPosicio()
{
	return m_primeraPosicio;
}

void CMapa::biosistema(CBiosistema * biosistema)
{
	m_biosistema=biosistema;
	if (!m_biosistema) {
		m_biotop = NULL;
		return;
	}
	m_biotop = m_biosistema->biotop();
	m_ampladaZona =((CTopologiaToroidal*)m_biotop->topologia())->amplada();
	m_alturaZona = ((CTopologiaToroidal*)m_biotop->topologia())->alcada();
	m_totalCelles = m_alturaZona * m_ampladaZona;
	m_primeraPosicio = 0;
}

CBiosistema * CMapa::biosistema(void)
{
	return m_biosistema;
}
