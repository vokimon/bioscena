// Grafic.h: interface for the CGrafic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_GRAFIC_H_INCLUDED)
#define __KKEP_GRAFIC_H_INCLUDED

#include <functional>
#include <list>
#include <vector>
#include <deque>
#include "BioIncludes.h"
#include "Missatger.h"
#include "Comunitat.h"
#include "Biosistema.h"
#include "Biotop.h"

class CDominiGrafica {
// Construccio/Destruccio
public:
	CDominiGrafica (); // Domini dinàmic
	CDominiGrafica (uint32 factor); // Domini fixe
// Operacions
public:
	void logaritmic(bool esLogaritmic);
	void adaptaFactor (uint32 tope);
	void factorFixe (uint32 factor);
	void dinamic ();
	uint32 mapeja (uint32 valor);
	uint32 premapeja (uint32 valor);
	void representacio (char simbol, char simbolFora);
	char simbol() {return m_simbol;};
	char simbolFora() {return m_simbolFora;};
// Atributs
private:
	uint32 m_maxim;
	uint32 m_factor;
	uint32 m_esDinamic;
	bool m_esLogaritmic;
public:
	char m_simbol;
	char m_simbolFora;
};

template <class TObjecteComparat> 
class CDominiGraficaComparativa : public CDominiGrafica {
// Tipus propis
	typedef uint32 (*t_funcio) (TObjecteComparat &);
// Construccio/Destruccio
public:
	CDominiGraficaComparativa() {m_funcio=NULL;};
// Operacions
public:
	void funcio(t_funcio f) {m_funcio=f;};
	t_funcio funcio() {return m_funcio;};
// Atributs
private:
	t_funcio m_funcio;
};

class CDominiGraficaEvolutiva : public CDominiGrafica {
// Atributs
private:
	deque<uint32> m_valors;
	uint32 m_tamanyMemoria;
// Construccio/Destruccio
public:
	CDominiGraficaEvolutiva(bool esLogaritmic, uint32 factor, uint32 tamanyMemoria=80)
		: CDominiGrafica(factor)
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
	virtual void visualitza(std::ostream & msg)=0;
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

/*
class CMarc: public CGrafic {
// Tipus interns
	typedef CGrafic inherited;
// Construccio/Destruccio
public:
	CMarc();
	virtual ~CMarc();
// Operacions
public:
	virtual void visualitza(std::ostream & msg)=0;
	virtual void tamany(uint32 posX, uint32 posY, uint32 ample, uint32 altura);
// Implementacio
protected:
	void recalculaTope();
// Atributs
protected:
//	dequeue<CGrafic *> m_grafics;
};
*/

class CComparativaOrganismes : public CGrafic
{
// Tipus interns
	typedef CGrafic inherited;
// Construccio/Destruccio
public:
	CComparativaOrganismes();
	virtual ~CComparativaOrganismes();
// Operacions
public:
	virtual void visualitza(std::ostream & msg);
	void inici(uint32 org);
	void comunitat(CComunitat * comunitat);
	CComunitat * comunitat(void);
	void afegeixDomini(CDominiGraficaComparativa<CInfoOrganisme> & domini);
// Implementacio
private:
// Atributs
private:
	uint32 m_inici;
	CComunitat * m_comunitat;
	vector<CDominiGraficaComparativa<CInfoOrganisme> > m_dominis;
};

class CComparativaTaxons : public CGrafic
{
// Tipus interns
	typedef CGrafic inherited;
// Construccio/Destruccio
public:
	CComparativaTaxons();
	virtual ~CComparativaTaxons();
// Operacions
public:
	virtual void visualitza(std::ostream & msg);
	void inici(uint32 tax);
	void taxonomista(CTaxonomista * comunitat);
	CTaxonomista * taxonomista(void);
	void afegeixDomini(CDominiGraficaComparativa<CInfoTaxo> & domini);
// Implementacio
private:
// Atributs
private:
	uint32 m_inici;
	CTaxonomista * m_taxonomista;
	vector<CDominiGraficaComparativa<CInfoTaxo> > m_dominis;
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
	virtual void visualitza(std::ostream & msg);
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
	virtual void visualitza(std::ostream & msg);
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
	Bioscena::CBiotop<CSubstrat> * m_biotop;
	uint32 m_primeraPosicio;
	uint32 m_ampladaZona;
	uint32 m_alturaZona;
	uint32 m_totalCelles;
};

#endif // !defined(__KKEP_GRAFIC_H_INCLUDED)

