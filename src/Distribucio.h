// Distribucio.h: interface for the CDistribucio class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_DISTRIBUCIO_H_INCLUDED)
#define __KKEP_DISTRIBUCIO_H_INCLUDED

#include "BioIncludes.h"
#include "RandomStream.h"

class CDistribucio  
{
public:
	CDistribucio()
	{
		m_minim=0;
		m_magnitudDau=0;
		m_nombreDaus=0;
	};
	virtual ~CDistribucio();
	operator uint32()
	{
		uint32 val;
		// Inicialitzem amb el valor minim
		val=m_minim;
		// I li afegim l'extra dels daus
		for (uint32 i=m_nombreDaus; i--;)
			val+=rnd.get(0,m_magnitudDau);
		return val;
	}
	void minim(uint32 minim) {
		m_minim=minim;
	}
private:
	uint32 m_minim;
	uint32 m_magnitudDau;
	uint32 m_nombreDaus;
};

#endif // !defined(__KKEP_DISTRIBUCIO_H_INCLUDED)
