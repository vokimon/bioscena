// Grafic.cpp: implementation of the CGrafic class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19991107 VoK - Creat de l'unio d'algunes funcions de visualitzacio
// 19991107 VoK - Doble constructor de DominiGrafic
// 19991117 VoK - Diferenciem CGrafic de CComparativaOrganismes
// 20000527 VoK - Mapa ja no es en aquest fitxer
// 20000528 VoK - Canvis estructurals als grafics comparatius
// 20000528 VoK - Creat el template CDominiComparativa
// 20000528 VoK - Es poden configurar els dominis a representar a la 
//                comparativa d'organismes
// 20000528 VoK - Les linies divisories son altre atribut dels dominis
// 20000528 VoK - Comparativa de taxons
//////////////////////////////////////////////////////////////////////
// TODO:
// Fer mes prima l'escala logaritmica
// Fer programable el que visualitzen els mapes
// Implementar Grafic Evolutiu/Temporal
// Muntar un conjunt de visualitzadors des d'un fitxer

#include "Grafic.h"
#include "Color.h"
#include "Comunitat.h"

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


CComparativaOrganismes::CComparativaOrganismes()
{
	m_comunitat=NULL;
	m_margeSup = 0;
	m_margeInf = 2;
	m_inici = 0;
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

void CComparativaOrganismes::inici(uint32 org)
{
	m_inici = org;
}

void CComparativaOrganismes::afegeixDomini(CDominiGraficaComparativa<CInfoOrganisme> & domini)
{
	m_dominis.push_back(domini);
}

void CComparativaOrganismes::visualitza(CMissatger & msg)
{
	using AnsiCodes::gotoxy;
	using AnsiCodes::clrlin;

	uint32 col, fil;
	uint32 idOrg;
	uint32 colorOrg;

	// Esborrem el fons
	for (fil=m_posY; fil<m_posY+m_tope+m_margeInf; ++fil) {
		msg << gotoxy(m_posX, fil) << blanc.fons(blanc) << clrlin;
	}

	if (!m_comunitat)
		return;
	CComunitat & comunitat = *m_comunitat;

	vector<CDominiGraficaComparativa<CInfoOrganisme> >::iterator it;
	for (it=m_dominis.begin(); it<m_dominis.end(); it++) {
		it->adaptaFactor(m_tope);
		}

	it=m_dominis.begin();
	if (it<m_dominis.end()) {
		uint32 valor;
		valor = it->premapeja(0xFF);
		if (valor<=m_tope) {
			msg << gotoxy(m_posX, m_posY + m_tope - valor) << blanc.fons(verd) << clrlin;
			valor = it->premapeja(0xFFFF);
			if (valor<=m_tope) {
				msg << gotoxy(m_posX, m_posY + m_tope - valor) << blanc.fons(groc) << clrlin;
				valor = it->premapeja(0x0FFFFFFF);
				if (valor<=m_tope) {
					msg << gotoxy(m_posX, m_posY + m_tope - valor) << blanc.fons(vermell) << clrlin;
				}
			}
		}
		msg << blanc.fons(blanc);
	}

	// Imprimim el contingut
	colorOrg=(m_inici>>3)&7;
	if (m_inici&7)
		msg << CColor((colorOrg++)&7).brillant();

	for (idOrg=m_inici,col=m_posX; col<m_posX+m_amplada && idOrg<comunitat.m_organismes.size(); col++,idOrg++) {
		// Color identificador
		if (!(idOrg&7)) msg << CColor((colorOrg++)&7).brillant();
		COrganisme * org = comunitat[idOrg].cos();
		CInfoOrganisme & info = comunitat[idOrg];
		if (org) {
			for (it=m_dominis.begin(); it<m_dominis.end(); it++) {
				uint32 valor = it->funcio()(info);
				valor = it->mapeja(valor);
				if (valor>m_tope)
					msg << gotoxy(col, m_posY) << it->m_simbolFora;
				else
					msg << gotoxy(col, m_posY+m_tope-valor) << it->m_simbol;
				}
			} 
		else {
			msg << gotoxy(col, m_posY+m_tope) << 'X';
			}
	}

	msg << gotoxy(m_posX, m_posY+m_tope+1);
	colorOrg=(m_inici>>3)&7;
	if (m_inici&7)
		msg << CColor((colorOrg++)&7).brillant();
	for (idOrg=m_inici,col=m_posX; col<m_posX+m_amplada && idOrg<comunitat.m_organismes.size(); col++,idOrg++)
	{
		// Color identificador
		if (!(idOrg&7)) msg << CColor((colorOrg++)&7).brillant();
		// Numero identificador
		msg << char((idOrg&7)+'0');
	}

	// Afegim els ids dels taxons
	msg << gotoxy(m_posX, m_posY+m_tope+2);
	for (idOrg=m_inici,col=m_posX; col<m_posX+m_amplada && idOrg<comunitat.m_organismes.size(); col++,idOrg++)
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
// (CComparativaTaxons) Construction/Destruction
//////////////////////////////////////////////////////////////////////


CComparativaTaxons::CComparativaTaxons()
{
	m_taxonomista=NULL;
	m_margeSup = 0;
	m_margeInf = 1;
	m_inici = 0;
	recalculaTope();

}

CComparativaTaxons::~CComparativaTaxons()
{

}

//////////////////////////////////////////////////////////////////////
// (CComparativaTaxons) Operacions
//////////////////////////////////////////////////////////////////////

void CComparativaTaxons::taxonomista(CTaxonomista * tax)
{
	m_taxonomista=tax;
}

CTaxonomista * CComparativaTaxons::taxonomista(void)
{
	return m_taxonomista;
}

void CComparativaTaxons::inici(uint32 tax)
{
	m_inici = tax;
}

void CComparativaTaxons::afegeixDomini(CDominiGraficaComparativa<CInfoTaxo> & domini)
{
	m_dominis.push_back(domini);
}

void CComparativaTaxons::visualitza(CMissatger & msg)
{
	using AnsiCodes::gotoxy;
	using AnsiCodes::clrlin;

	uint32 col, fil;
	uint32 idTax;
	uint32 colorTax;

	// Esborrem el fons
	for (fil=m_posY; fil<m_posY+m_tope+m_margeInf; ++fil) {
		msg << gotoxy(m_posX, fil) << blanc.fons(blanc) << clrlin;
	}
	msg << gotoxy(m_posX, fil) << blanc.fons(negre) << clrlin;

	if (!m_taxonomista)
		return;
	CTaxonomista & taxonomista = *m_taxonomista;

	vector<CDominiGraficaComparativa<CInfoTaxo> >::iterator it;
	for (it=m_dominis.begin(); it<m_dominis.end(); it++) {
		it->adaptaFactor(m_tope);
		}

	it=m_dominis.begin();
	if (it<m_dominis.end()) {
		uint32 valor;
		valor = it->premapeja(0xFF);
		if (valor<=m_tope) {
			msg << gotoxy(m_posX, m_posY + m_tope - valor) << blanc.fons(verd) << clrlin;
			valor = it->premapeja(0xFFFF);
			if (valor<=m_tope) {
				msg << gotoxy(m_posX, m_posY + m_tope - valor) << blanc.fons(groc) << clrlin;
				valor = it->premapeja(0x0FFFFFFF);
				if (valor<=m_tope) {
					msg << gotoxy(m_posX, m_posY + m_tope - valor) << blanc.fons(vermell) << clrlin;
				}
			}
		}
		msg << blanc.fons(blanc);
	}

	// Imprimim el contingut
	colorTax=50; // Un numero absurd

	CTaxonomista::t_taxons::iterator ittx;
	ittx=taxonomista.m_taxons.begin();
	if (m_inici<taxonomista.m_taxons.size())
		for (int i=m_inici; i--;) ittx++;
	else 
		ittx=taxonomista.m_taxons.end();
	for (idTax=m_inici, col=m_posX; col<m_posX+m_amplada && idTax<taxonomista.m_taxons.size(); col++, ittx++, idTax++) {
		// Color identificador
		uint32 nouColor = ((ittx->first)>>3)&7;
		if (nouColor!=colorTax) {
			colorTax=nouColor;
			msg << CColor(colorTax).brillant();
			}
		CInfoTaxo & info = ittx->second;
		for (it=m_dominis.begin(); it<m_dominis.end(); it++) {
			uint32 valor = it->funcio()(info);
			valor = it->mapeja(valor);
			if (valor>m_tope)
				msg << gotoxy(col, m_posY) << it->m_simbolFora;
			else
				msg << gotoxy(col, m_posY+m_tope-valor) << it->m_simbol;
			}
	}
/*
	msg << gotoxy(m_posX, m_posY+m_tope+1);
	colorTax=(m_inici>>3)&7;
	if (m_inici&7)
		msg << CColor((colorTax++)&7).brillant();
	ittx=taxonomista.m_taxons.begin();
	for (int i=m_inici; i--;) ittx++;
	for (idTax=m_inici,col=m_posX; col<m_posX+m_amplada && idTax<taxonomista.m_taxons.size(); col++,idTax++, ittx++) {
		// Color identificador
		if (!(idTax&7)) msg << CColor((colorTax++)&7).brillant();
		// Numero identificador
		msg << char((idTax&7)+'0');
	}
*/
	// Afegim els ids dels taxons
	ittx=taxonomista.m_taxons.begin();
	if (m_inici<taxonomista.m_taxons.size())
		for (int i=m_inici; i--;) ittx++;
	else 
		ittx=taxonomista.m_taxons.end();
	msg << gotoxy(m_posX, m_posY+m_tope+1);
	for (idTax=m_inici,col=m_posX; col<m_posX+m_amplada && idTax<taxonomista.m_taxons.size(); col++,idTax++, ittx++) {
		msg << negre.fons(((ittx->first)>>3)&7)
			<< ((ittx->first)&7);
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
// (CGraficaEvolutiva) Operacions
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
	colorOrg=(m_inici>>3)&7;
	if (m_inici&7)
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

CDominiGrafica::CDominiGrafica ()
{
	m_maxim = 0;
	m_factor = 0; 
	m_esDinamic = true;
	m_esLogaritmic = false;
	m_simbol = '@';
	m_simbolFora = '?';
}

CDominiGrafica::CDominiGrafica (uint32 factor)
{
	m_maxim = 0; 
	m_factor = factor; 
	m_esDinamic = false;
	m_esLogaritmic = false;
	m_simbol = '@';
	m_simbolFora = '?';
}


//////////////////////////////////////////////////////////////////////
// (CDominiGrafica) Operacions
//////////////////////////////////////////////////////////////////////

void CDominiGrafica::representacio (char simbol, char simbolFora) 
// Fixa el domini a un factor de proporcio donat
{
	m_simbol = simbol;
	m_simbolFora = simbolFora;
}

void CDominiGrafica::factorFixe (uint32 factor) 
// Fixa el domini a un factor de proporcio donat
{
	m_esDinamic = false;
	m_factor = factor;
}

void CDominiGrafica::dinamic () 
// Fa que el factor de proporcio es pugui adaptar
{
	m_esDinamic = true;
}

void CDominiGrafica::logaritmic (bool esLogaritmic) 
// Fa que el factor de proporcio es pugui adaptar
{
	m_esLogaritmic = esLogaritmic;
}

void CDominiGrafica::adaptaFactor (uint32 tope) 
// Si el factor es dinamic, es fixa per que el maxim sigui representable
{
	if (!m_esDinamic) return;
	uint32 factor=m_factor;
	for (m_factor=0; mapeja (m_maxim) > tope; m_factor++);
	if (m_factor<factor) m_factor=factor-1;
	m_maxim=0;
}

uint32 CDominiGrafica::mapeja (uint32 valor) 
{
	// Actualitza el maxim per la proxima visualitzacio
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


