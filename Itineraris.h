// Itineraris.h: CPosicionador & CDireccionador subclasses' interface
//
//////////////////////////////////////////////////////////////////////
// Aquesta capcelera descriu els prototips per a les especialitzacions
// de CPosicionador i CDireccionador
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990823 VoK - Eliminadas funciones inline que passan al cpp
// 19990823 VoK - Completadas las funcionalidades estandard de CPosicionadorZonal
//////////////////////////////////////////////////////////////////////
// TODO: Fer Direccionadors i Posicionadors segons una sequencia.

#if !defined(__KKEP_ITINERARIS_H_INCLUDED)
#define __KKEP_ITINERARIS_H_INCLUDED

#include "Posicionador.h"
#include "RandomStream.h"

//////////////////////////////////////////////////////////////////////
// CItinerari: Varia la posicio respecte la posició anterior, segons 
// un direccionador (del qual depen).
//////////////////////////////////////////////////////////////////////

class CItinerari : public CPosicionador
{
// Construccio/Destruccio
public:
	CItinerari(tipus_biotop & biotop);
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (void);
	virtual void dump(CMissatger & msg);
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
};

//////////////////////////////////////////////////////////////////////
// CPosicionadorZonal: Varia la posicio dintre d'un zona
//////////////////////////////////////////////////////////////////////

class CPosicionadorZonal : public CPosicionador
{
// Construccio/Destruccio
public:
	CPosicionadorZonal(tipus_biotop & biotop);
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (void);
	virtual void dump(CMissatger & msg);
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
// Construccio/Destruccio
public:
	CPosicionadorAleatori(tipus_biotop & biotop);
// Virtuals redefinibles a les subclasses
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
// Construccio/Destruccio
public:
	CDireccionadorAleatori(tipus_biotop & biotop);
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (void);
// Proves
public:
	static void ProvaClasse();
};

#endif // !defined(__KKEP_ITINERARIS_H_INCLUDED)
