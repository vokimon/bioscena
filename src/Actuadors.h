// Actuadors.h: interface for the CActuador subclasses.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_ACTUADORS_H_INCLUDED)
#define __KKEP_ACTUADORS_H_INCLUDED

#include "Substrat.h"
#include "Actuador.h"
#include "Posicionador.h"

class CNutridor : public CActuador
{
// Construccio/Destruccio
public:
	CNutridor();
	virtual ~CNutridor();
// Operadors (Per accionar)
public:
	virtual void operator () (CSubstrat & s) {
		uint32 mol = m_element^(m_variabilitat&rnd.get());
		s.deposita(mol);
	}
// Operadors (Per configurar)
public:
	void composicio(uint32 element, uint32 variabilitat=0);
// Atributs
public:
	uint32 m_element; // Element basic que s'afegira
	uint32 m_variabilitat; // Mascara amb els bits de l'element que poden variar
// Proves
public:
	static void ProvaClasse();
};

class CDesnutridor : public CNutridor
{
// Construccio/Destruccio
public:
	CDesnutridor();
// Operadors (Per accionar)
public:
	virtual void operator() (CSubstrat & s) {
		uint32 nutrient;
		s.extreu(nutrient, m_element,m_variabilitat);
	}
/*	void operator() ()
	{
		(*(CActuador*)(this))();
	}
*/
// Proves
public:
	static void ProvaClasse();
};

#endif // !defined(__KKEP_ACTUADORS_H_INCLUDED)
