// ActuadorsNutrients.h: interface for the CActuador subclasses.
//
//////////////////////////////////////////////////////////////////////


#ifndef __KKEP_ACTUADORSNUTRIENTS_H_INCLUDED
#define __KKEP_ACTUADORSNUTRIENTS_H_INCLUDED

#include "Actuador.h"
#include "Biotop.h"
#include "Substrat.h"

class CNutridor : public CActuador
{
// Tipus interns
public:
	typedef CActuador inherited;
	typedef CActuador super; // equivalent a l'anterior
	typedef Bioscena::CBiotop<CSubstrat> t_biotop;
// Construccio/Destruccio
public:
	CNutridor(t_biotop & biotop);
	virtual ~CNutridor();
// Redefinibles
public:
	virtual void operator () ();
	virtual void dump (CMissatger & msg);
	virtual bool configura(string parametre, istream & nom, t_diccionariAgents & diccionari, CMissatger & errors);
// Operadors
public:
	void composicio(uint32 element, uint32 variabilitat=0);
// Atributs
public:
	t_biotop & m_biotop;
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
	typedef CNutridor super; // equivalent a l'anterior
// Construccio/Destruccio
public:
	CDesnutridor(t_biotop & biotop);
// Redefinibles
public:
	virtual void operator() ();
// Proves
public:
	static void ProvaClasse();
};

class CBiosistema;

class CInoculador : public CActuador
{
// Tipus interns
public:
	typedef CActuador inherited;
	typedef CBiosistema t_biosistema;
// Construccio/Destruccio
public:
	CInoculador(t_biosistema & biosistema);
	virtual ~CInoculador() {};
// Redefinibles
public:
	virtual void operator () ();
	virtual void dump (CMissatger & msg);
	virtual bool configura(string parametre, istream & nom, t_diccionariAgents & diccionari, CMissatger & errors);
// Operadors
public:
	void nomControl(string tipus);
// Atributs
public:
	string m_nomControl;
	t_biosistema & m_biosistema;
// Proves
public:
	static void ProvaClasse();
};

#endif // !defined(__KKEP_ACTUADORSNUTRIENTS_H_INCLUDED)
