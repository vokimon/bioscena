// Grafic.h: interface for the CGrafic class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19991107 VoK - Creat de l'unio d'algunes funcions de visualitzacio
// 19991107 VoK - Doble constructor de DominiGrafic
//////////////////////////////////////////////////////////////////////
// TODO:
// Fer mes prima l'escala logaritmica

#if !defined(_KKEP_GRAFIC_H_INCLUDED)
#define _KKEP_GRAFIC_H_INCLUDED

#include <functional>
#include <list>
#include "BioIncludes.h"
#include "Missatger.h"
#include "Comunitat.h"

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
	typedef void inherited;
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
	list<CDominiGrafica> m_dominis;
	CComunitat * m_comunitat;
	uint32 m_primerOrg;
};


#endif // !defined(_KKEP_GRAFIC_H_INCLUDED)
