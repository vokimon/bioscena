// Itineraris.h: CPosicionador & CDireccionador subclasses' interface
//
//////////////////////////////////////////////////////////////////////
// Class Description:
// Aquesta capcelera descriu els prototips per a les especialitzacions
// de CPosicionador i CDireccionador
//////////////////////////////////////////////////////////////////////
// TODO: Fer Direccionadors i Posicionadors segons una sequencia.
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_ITINERARIS_H_INCLUDED)
#define __KKEP_ITINERARIS_H_INCLUDED

#include "Posicionador.h"
#include "RandomStream.hxx"

//////////////////////////////////////////////////////////////////////
// CItinerari: Varia la posicio respecte la posició anterior, segons 
// un direccionador (del qual depen).
//////////////////////////////////////////////////////////////////////

class CItinerari : public CPosicionador
{
// Tipus interns
public:
	typedef CPosicionador inherited;
// Construccio/Destruccio
public:
	CItinerari(t_biotop & biotop);
// Redefinibles
public:
	virtual void operator() (void);
	virtual void dump(CMissatger & msg);
	virtual bool configura(string parametre, istream & nom, t_diccionariAgents & diccionari, CMissatger & errors);
	virtual list<CAgent*> dependencies();
// Operacions
public:
	void direccionador(CDireccionador * dir);
	CDireccionador * direccionador();
	uint32 radi();
	void radi(uint32 rad);
// Atributs
protected:
	CDireccionador * m_direccionador;
	uint32 m_radi;
// Proves
public:
	static void ProvaClasse();
};

//////////////////////////////////////////////////////////////////////
// CPosicionadorZonal: Varia la posicio dintre d'un zona
//////////////////////////////////////////////////////////////////////

class CPosicionadorZonal : public CPosicionador
{
// Tipus interns
public:
	typedef CPosicionador inherited;
// Construccio/Destruccio
public:
	CPosicionadorZonal(t_biotop & biotop);
// Redefinibles
public:
	virtual void operator() (void);
	virtual void dump(CMissatger & msg);
	virtual bool configura(string parametre, istream & nom, t_diccionariAgents & diccionari, CMissatger & errors);
	virtual list<CAgent*> dependencies();
// Operacions
public:
	void posicionador(CPosicionador * pos);
	CPosicionador * posicionador();
	uint32 radi();
	void radi(uint32 rad);
// Atributs
protected:
	CPosicionador * m_posicionador;
	uint32 m_radi;
// Proves
public:
	static void ProvaClasse();
};

//////////////////////////////////////////////////////////////////////
// CPosicionadorAleatori: Varia la Posicio aleatoriament
//////////////////////////////////////////////////////////////////////

class CPosicionadorAleatori : public CPosicionador
{
// Tipus interns
public:
	typedef CPosicionador inherited;
// Construccio/Destruccio
public:
	CPosicionadorAleatori(t_biotop & biotop);
// Redefinibles
public:
	virtual void operator() (void);
// Proves
public:
	static void ProvaClasse();
};

//////////////////////////////////////////////////////////////////////
// CDireccionadorAleatori: Varia la direccio aleatoriament
//////////////////////////////////////////////////////////////////////

class CDireccionadorAleatori : public CDireccionador
{
// Tipus interns
public:
	typedef CDireccionador inherited;
// Construccio/Destruccio
public:
	CDireccionadorAleatori(t_biotop & biotop);
// Redefinibles
public:
	virtual void operator() (void);
// Proves
public:
	static void ProvaClasse();
};

#endif // !defined(__KKEP_ITINERARIS_H_INCLUDED)
