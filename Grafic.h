// Grafic.h: interface for the CGrafic class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19991107 VoK - Creat de l'unio d'algunes funcions de visualitzacio
// 19991107 VoK - Doble constructor de DominiGrafic
// 19991117 VoK - Diferenciem CGrafic de CComparativaOrganismes
// 19991118 VoK - Mapa es un nou objete Grafic
// 19991118 VoK - Mapa: Tamanys de mapa i topologia independents
// 19991119 VoK - Mapa: Origen movil -> escroll
//////////////////////////////////////////////////////////////////////
// TODO:
// Fer mes prima l'escala logaritmica
// Fer programable el que visualitzen les comparatives d'organismes
// Fer programable el que visualitzen els mapes
// Implementar Grafic Evolutiu/Temporal
// Muntar un conjunt de visualitzadors des d'un fitxer
// Implementar Comparativa Taxons

#if !defined(_KKEP_GRAFIC_H_INCLUDED)
#define _KKEP_GRAFIC_H_INCLUDED

#include <functional>
#include <list>
#include "BioIncludes.h"
#include "Missatger.h"
#include "Comunitat.h"
#include "Biosistema.h"
#include "TopologiaToroidal.h"

class CDominiGrafica {
// Atributs
private:
	uint32 m_maxim;
	uint32 m_factor;
	uint32 m_esDinamic;
	bool m_esLogaritmic;
// Construccio/Destruccio
public:
	CDominiGrafica (bool esLogaritmic); // Domini dinàmic
	CDominiGrafica (bool esLogaritmic, uint32 factor); // Domini fixe
// Operacions
public:
	void fixaFactor (uint32 tope); // Si el factor es dinamic, es fixa per que el maxim sigui representable
	uint32 mapeja (uint32 valor); // 
	uint32 premapeja (uint32 valor); // 
};


/*
class CDominiGraficaComparativa : public CDominiGrafica {
// Atributs
private:
	unary_function<uint32, uint32> & m_f;
// Construccio/Destruccio
public:
	CDominiGraficaComparativa(unary_function<uint32, uint32> & f) : m_f=f {}
};
*/


class CGrafic  
{
// Tipus interns
	typedef void inherited;
// Construccio/Destruccio
public:
	CGrafic();
	virtual ~CGrafic();
// Operacions
public:
	virtual void visualitza(CMissatger & msg)=0;
	void tamany(uint32 posX, uint32 posY, uint32 amplada, uint32 altura);
// Implementacio
protected:
	void recalculaTope();
// Atributs
protected:
	uint32 m_altura;
	uint32 m_amplada;
	uint32 m_margeSup;
	uint32 m_margeInf;
	uint32 m_posX;
	uint32 m_posY;
	uint32 m_tope;
};

class CComparativaOrganismes : public CGrafic
{
// Tipus interns
	typedef CGrafic inherited;
// Construccio/Destruccio
public:
	CComparativaOrganismes(CComunitat * comunitat);
	virtual ~CComparativaOrganismes();
// Operacions
public:
	virtual void visualitza(CMissatger & msg);
	void primerOrganisme(uint32 org);
	void comunitat(CComunitat * comunitat);
	CComunitat * comunitat(void);
// Implementacio
private:
// Atributs
private:
//	list<CDominiGrafica> m_dominis;
	CComunitat * m_comunitat;
	uint32 m_primerOrg;
};

class CMapa : public CGrafic
{
// Tipus interns
	typedef CGrafic inherited;
// Construccio/Destruccio
public:
	CMapa(CBiosistema * biosistema);
	virtual ~CMapa();
// Operacions
public:
	virtual void visualitza(CMissatger & msg);
	void primeraPosicio(uint32 pos);
	void scrollUp(uint32 steps);
	void scrollDown(uint32 steps);
	void scrollLeft(uint32 steps);
	void scrollRight(uint32 steps);
	void scrollPageUp(uint32 steps);
	void scrollPageDown(uint32 steps);
	void scrollPageLeft(uint32 steps);
	void scrollPageRight(uint32 steps);
	void biosistema(CBiosistema * biosistema);
	CBiosistema * biosistema(void);
// Implementacio
private:
// Atributs
private:
	CBiosistema * m_biosistema;
	CTopologiaToroidal<CSubstrat> * m_biotop;
	uint32 m_primeraPosicio;
	uint32 m_ampladaZona;
	uint32 m_alturaZona;
	uint32 m_totalCelles;
};

#endif // !defined(_KKEP_GRAFIC_H_INCLUDED)
