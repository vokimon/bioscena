// Actuadors.h: interface for the CActuador subclasses.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990820 VoK - Creat (a partir de l'antic Cumulador)
//////////////////////////////////////////////////////////////////////


#if !defined(__KKEP_ACTUADORS_H_INCLUDED)
#define __KKEP_ACTUADORS_H_INCLUDED

#include "Actuador.h"

class CNutridor : public CActuador
{
// Construccio/Destruccio
public:
	CNutridor();
	virtual ~CNutridor();
// Virtuals redefinibles a les subclasses
public:
	virtual void operator () (CSubstrat & s) {
		uint32 mol = m_element^(m_variabilitat&rnd.get());
		s.deposita(mol);
	}
	virtual void dump (CMissatger & msg);
// Operadors
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
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (CSubstrat & s) {
		uint32 nutrient;
		s.extreu(nutrient, m_element,m_variabilitat);
	}
// Proves
public:
	static void ProvaClasse();
};

#endif // !defined(__KKEP_ACTUADORS_H_INCLUDED)
