// Genotip.h: interface for the CGenotip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_GENOTIP_H_INCLUDED)
#define __KKEP_GENOTIP_H_INCLUDED

#include "BioIncludes.h"
#include "Missatger.h"
#include "Gen.h"
#include "Cariotip.h"
#include "ControlOrganisme.h"

class CGenotip : public CControlOrganisme
{
// Construccio/Destruccio
public:
	CGenotip();
	virtual ~CGenotip();
// Operacions
public:
	bool init(CCariotip& c);
	void clear();
	uint32 seguentInstruccio(uint32 * fenotip);
	void dump(CMissatger & msg);
// Atributs
public:
	uint32 m_genActual;
	vector<CGen> m_gens;
// Proves
public:
	static void ProvaClasse();
};

#endif // !defined(__KKEP_GENOTIP_H_INCLUDED)
