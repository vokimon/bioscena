// Actuador.h: interface for the CActuador class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_ACTUADOR_H_INCLUDED)
#define __KKEP_ACTUADOR_H_INCLUDED

#include "Agent.h"
#include "Posicionador.h"

class CActuador : public CAgent  
{
// Tipus Propis
	typedef CAgent super;
// Construccio/Destruccio
public:
	CActuador();
	virtual ~CActuador();
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (void);
	virtual void operator() (CSubstrat & s)=0;
	virtual void dump(CMissatger & msg);
	virtual list<CAgent*> dependencies();
// Operacions
public:
	void posicionador (CPosicionador* pos);
	CPosicionador* posicionador ();
// Atributs
public:
	CPosicionador * m_posicionador;
};

#endif // !defined(__KKEP_ACTUADOR_H_INCLUDED)
