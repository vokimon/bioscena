// Genotip.h: interface for the CGenotip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_GENOTIP_H_INCLUDED)
#define __KKEP_GENOTIP_H_INCLUDED

#include "BioIncludes.h"
#include "Gen.h"
#include "Cariotip.h"

class CGenotip
{
// Construccio/Destruccio
public:
	CGenotip();
	virtual ~CGenotip();
// Operacions
public:
	uint32 seguentInstruccio(uint32 * fenotip);
// Atributs
public:
	bool init(CCariotip& c);
	uint32 m_genActual;
	vector<CGen> m_gens;
};

#endif // !defined(__KKEP_GENOTIP_H_INCLUDED)
