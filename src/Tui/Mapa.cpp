// Grafic.cpp: implementation of the CGrafic class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19991118 VoK - Mapa es un nou objete Grafic
// 19991118 VoK - Mapa: Tamanys de mapa i topologia independents
// 19991119 VoK - Mapa: Origen movil -> escroll
// 20000217 VoK - Fix: ScrollDown no definia be els limits
//////////////////////////////////////////////////////////////////////
// TODO:
// Fer programable el que visualitzen els mapes
// Muntar un conjunt de visualitzadors des d'un fitxer

#include "Grafic.h"
#include "Color.h"
#include "Comunitat.h"
#include "TopologiaToroidal.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
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

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////
// Operacions (Scroll)
//////////////////////////////////////////////////////////////////////

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
	// Cal posicionar-se tantes posicions enlla com l'altura del mapa
	scrollDown(m_altura*steps);
}

void CMapa::scrollPageUp(uint32 steps)
{
	// Cal posicionar-se tantes posicions enlla com l'altura del mapa
	scrollUp(m_altura*steps);
}

void CMapa::scrollPageRight(uint32 steps)
{
	// Cal posicionar-se tantes posicions enlla com l'amplada del mapa
	scrollRight(m_amplada*steps);
}

void CMapa::scrollPageLeft(uint32 steps)
{
	// Cal posicionar-se tantes posicions enlla com l'amplada del mapa
	scrollLeft(m_amplada*steps);
}

//////////////////////////////////////////////////////////////////////
// Operacions (Test & Set)
//////////////////////////////////////////////////////////////////////

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
	if (!m_biotop)
		return;
	m_ampladaZona =((Bioscena::Torus*)m_biotop->topologia())->width();
	m_alturaZona = ((Bioscena::Torus*)m_biotop->topologia())->height();
	m_totalCelles = m_alturaZona * m_ampladaZona;
	m_primeraPosicio = 0;
}

CBiosistema * CMapa::biosistema(void)
{
	return m_biosistema;
}


