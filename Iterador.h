// Iterador.h: interface for the CIterador class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_ITERADOR_H_INCLUDED)
#define __KKEP_ITERADOR_H_INCLUDED

#include "MultiAgent.h"

class CIterador : public CMultiAgent  
{
// Operacions
public:
	void iteracions(uint32 iteracions, uint32 magDaus=0, uint32 nDaus=0);
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (void);
// Construccio/Destruccio
public:
	CIterador();
	virtual ~CIterador();
// Atributs
public:
	void dump(CMissatger &msg);
	uint32 m_iteracionsMinimes;
	uint32 m_nombreDaus;
	uint32 m_magnitudDau;
};

#endif // !defined(__KKEP_ITERADOR_H_INCLUDED)
