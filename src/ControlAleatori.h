// ControlAleatori.h: interface for the CControlAleatori class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_CONTROLALEATORI_INCLUDED)
#define __KKEP_CONTROLALEATORI_INCLUDED

#include "BioIncludes.h"
#include "Missatger.h"
#include "Gen.h"
#include "Cariotip.h"
#include "ControlOrganisme.h"

class CControlAleatori : public CControlOrganisme
{
// Construccio/Destruccio
public:
	CControlAleatori();
	virtual ~CControlAleatori();
// ControlOrganisme Interface Implementation
public:
	virtual bool init(const CCariotip& c);
	virtual void clear();
	virtual uint32 seguentInstruccio(const uint32 * fenotip);
	virtual void dump(CMissatger & msg);
// Atributs
public:
	// Cap
// Proves
public:
	static void ProvaClasse();
};

#endif // !defined(__KKEP_CONTROLALEATORI_INCLUDED)
