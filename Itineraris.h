// ItinerarisConcrets.h: interface for the CItinerari subclasses.
//
//////////////////////////////////////////////////////////////////////
// Aquesta capcelera descriu els prototips per a les especialitzacions
// de CPosicionador
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_ITINERARISCONCRETS_H_INCLUDED)
#define __KKEP_ITINERARISCONCRETS_H_INCLUDED

#include "Posicionador.h"
#include "RandomStream.h"

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
};

// Es mou en la mateixa direccio
class CItinerariDireccioConstant : public CPosicionador
{
// Construccio/Destruccio
public:
	CItinerariDireccioConstant(tipus_biotop & biotop):CPosicionador(biotop)
		{m_direccio=0;};
// Operacions
public:
	void direccio(tipus_direccio dir) {
		m_direccio=dir;
	}
	tipus_direccio direccio() {
		return m_direccio;
	}
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (void) 
	{
		m_pos = m_biotop.desplacament(m_pos, m_direccio);
	}
// Atributs
protected:
	tipus_direccio m_direccio;
};

// Mou en direccio aleatoria
class CItinerariAleatori : public CPosicionador
{
// Construccio/Destruccio
public:
	CItinerariAleatori::CItinerariAleatori(tipus_biotop & biotop):CPosicionador(biotop) {};
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (void) 
	{
		m_pos = m_biotop.desplacament(m_pos, rnd.get());
	}
};


#endif // !defined(__KKEP_ITINERARISCONCRETS_H_INCLUDED)
