// ItinerarisConcrets.h: interface for the CItinerari subclasses.
//
//////////////////////////////////////////////////////////////////////
// Aquesta capcelera descriu els prototips per a les especialitzacions
// de CPosicionador i CDireccionador
//////////////////////////////////////////////////////////////////////
// TODO: Fer Direccionadors i Posicionadors segons una sequencia.
// TODO: Fer Posicionadors en l'entorn d'una posicio

#if !defined(__KKEP_ITINERARIS_H_INCLUDED)
#define __KKEP_ITINERARIS_H_INCLUDED

#include "Posicionador.h"
#include "RandomStream.h"

//////////////////////////////////////////////////////////////////////
// CItinerari: Varia la posicio respecte la posició anterior, segons 
// un direccionador.
//////////////////////////////////////////////////////////////////////

class CItinerari : public CPosicionador
{
// Construccio/Destruccio
public:
	CItinerari(tipus_biotop & biotop, CDireccionador* dir):CPosicionador(biotop)
	{
		m_direccionador=dir;
		m_radi=1;
	};
// Operacions
public:
	void direccionador(CDireccionador * dir) {
		m_direccionador=dir;
	}
	CDireccionador * direccionador() {
		return m_direccionador;
	}
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (void) 
	{
		for (int i=m_radi; i--;)
			m_pos = m_biotop.desplacament(m_pos, m_direccionador->dir());
	}
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
	CPosicionadorZonal(tipus_biotop & biotop, CPosicionador* pos):CPosicionador(biotop)
	{
		m_posicionador=pos;
		m_radi=1;
	};
// Operacions
public:
	void radi(uint32 radi) {
		m_radi=radi;
	}
	void posicionador(CPosicionador * pos) {
		m_posicionador=pos;
	}
	CPosicionador * posicionador() {
		return m_posicionador;
	}
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (void) 
	{
		m_pos=m_posicionador->pos();
		for (int i=m_radi; i--;)
			m_pos = m_biotop.desplacament(m_pos, rnd.get());
	}
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
	CPosicionadorAleatori(tipus_biotop & biotop):CPosicionador(biotop)
		{};
// Operacions
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (void) 
	{
		m_pos=m_biotop.cassellaAlAtzar();
	}
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
	CDireccionadorAleatori::CDireccionadorAleatori(tipus_biotop & biotop)
		:CDireccionador(biotop) {};
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (void) 
	{
		m_dir = rnd.get();
	}
// Proves
public:
	static void ProvaClasse();
};


#endif // !defined(__KKEP_ITINERARIS_H_INCLUDED)
