// Actuador.h: interface for the CActuador class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_ACTUADOR_H_INCLUDED)
#define __KKEP_ACTUADOR_H_INCLUDED

#include "Agent.h"
#include "Posicionador.h"

class CActuador : public CAgent  
{
// Construccio/Destruccio
public:
	CActuador();
	virtual ~CActuador();
// Operacions
public:
	void posicionador (CPosicionador* pos);
	CPosicionador* posicionador ();
// Redefinibles per les subclasses
public:
	virtual void operator() (void);
	virtual void operator() (CSubstrat & s)=0;
// Atributs
public:
	CPosicionador * m_posicionador;
};

#endif // !defined(__KKEP_ACTUADOR_H_INCLUDED)
