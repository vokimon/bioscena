// Grafic.h: interface for the CGrafic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_KKEP_GRAFIC_H_INCLUDED)
#define _KKEP_GRAFIC_H_INCLUDED

#include <functional>
#include <list>
#include <deque>
#include "BioIncludes.h"
#include "Missatger.h"
#include "Comunitat.h"
#include "Biosistema.h"
#include "Biotop.h"
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

class CDominiGraficaEvolutiva : public CDominiGrafica {
// Atributs
private:
	deque<uint32> m_valors;
	uint32 m_tamanyMemoria;
// Construccio/Destruccio
public:
	CDominiGraficaEvolutiva(bool esLogaritmic, uint32 factor, uint32 tamanyMemoria=80)
		: CDominiGrafica(esLogaritmic, factor)
	{m_tamanyMemoria = tamanyMemoria;}
};



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
	virtual void tamany(uint32 posX, uint32 posY, uint32 amplada, uint32 altura);
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
	CDominiGrafica m_dominiEnergia;
	CDominiGrafica m_dominiEdat;
};

class CGraficaEvolutiva : public CGrafic
{
// Tipus interns
	typedef CGrafic inherited;
// Construccio/Destruccio
public:
	CGraficaEvolutiva(CBiosistema * biosistema);
	virtual ~CGraficaEvolutiva();
// Operacions
public:
	virtual void visualitza(CMissatger & msg);
	void biosistema(CBiosistema * biosistema);
	CBiosistema * biosistema(void);
// Implementacio
private:
// Atributs
private:
//	list<CDominiGrafica> m_dominis;
	uint32 * m_memoria;
	CBiosistema * m_biosistema;
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
	void biosistema(CBiosistema * biosistema);
	CBiosistema * biosistema(void);
	void primeraPosicio(uint32 pos);
	uint32 primeraPosicio();
	virtual void visualitza(CMissatger & msg);
	void scrollUp(uint32 steps);
	void scrollDown(uint32 steps);
	void scrollLeft(uint32 steps);
	void scrollRight(uint32 steps);
	void scrollPageUp(uint32 steps);
	void scrollPageDown(uint32 steps);
	void scrollPageLeft(uint32 steps);
	void scrollPageRight(uint32 steps);
// Implementacio
private:
// Atributs
private:
	CBiosistema * m_biosistema;
	CBiotop<CSubstrat> * m_biotop;
	uint32 m_primeraPosicio;
	uint32 m_ampladaZona;
	uint32 m_alturaZona;
	uint32 m_totalCelles;
};

#endif // !defined(_KKEP_GRAFIC_H_INCLUDED)

