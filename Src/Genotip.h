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
// ControlOrganisme Interface Implementation
public:
	virtual bool init(const CCariotip& c);
	virtual void clear();
	virtual uint32 seguentInstruccio(const uint32 * fenotip);
	virtual void dump(CMissatger & msg);
// Atributs
public:
	uint32 m_genActual;
	vector<CGen> m_gens;
// Proves
public:
	static void ProvaClasse();
};

#endif // !defined(__KKEP_GENOTIP_H_INCLUDED)
