// Actuadors.h: interface for the CActuador subclasses.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990820 VoK - Creat (a partir de l'antic Cumulador)
// 19990822 VoK - Funcions de dump
// 19990823 VoK - Reordenat els fitxers
// 19990824 VoK - dump ja no posa ':' despres de nom del parametre
// 19990824 VoK - Funcions de configuracio amb parametres
//////////////////////////////////////////////////////////////////////


#if !defined(__KKEP_ACTUADORS_H_INCLUDED)
#define __KKEP_ACTUADORS_H_INCLUDED

#include "Actuador.h"

class CNutridor : public CActuador
{
// Tipus interns
public:
	typedef CActuador inherited;
// Construccio/Destruccio
public:
	CNutridor(t_biotop & biotop);
	virtual ~CNutridor();
// Redefinibles
public:
	virtual void operator () (CSubstrat & s) {
		uint32 mol = m_element^(m_variabilitat&rnd.get());
		s.deposita(mol);
	}
	virtual void dump (CMissatger & msg);
	virtual bool configura(string parametre, istream & nom, t_diccionariAgents & diccionari, CMissatger & errors);
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
// Tipus interns
public:
	typedef CNutridor inherited;
// Construccio/Destruccio
public:
	CDesnutridor(t_biotop & biotop);
// Redefinibles
public:
	virtual void operator() (CSubstrat & s) {
		uint32 nutrient;
		s.extreu(nutrient, m_element,m_variabilitat);
	}
// Proves
public:
	static void ProvaClasse();
};

class CInoculador : public CActuador
{
// Tipus interns
public:
	typedef CActuador inherited;
// Construccio/Destruccio
public:
	CInoculador(t_biotop & biotop);
	virtual ~CInoculador();
// Redefinibles
public:
	virtual void operator () (CSubstrat & s) {
//		m_biosistema->organismeExpontani();
	}
	virtual void dump (CMissatger & msg);
	virtual bool configura(string parametre, istream & nom, t_diccionariAgents & diccionari, CMissatger & errors);
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

#endif // !defined(__KKEP_ACTUADORS_H_INCLUDED)
