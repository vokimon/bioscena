// Gen.h: interface for the CGen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_GEN_H_INCLUDED)
#define __KKEP_GEN_H_INCLUDED

#include "BioIncludes.h"

class CGen  
{
// Construction/Destruction
public:
	CGen();
	virtual ~CGen();
// Redefinibles
public:
// Operacions
public:
	bool seguentInstruccio(uint32 & ip, uint32 & valor);
	bool seguentInstruccio(uint32 & ip, uint32 & valor, uint32 patro);
	bool parametres(uint32 & ip, uint32 & valor);
	bool traduible(uint32 * fenotip, uint32 & ip);
// Atributs
public:
// Proves
public:
	static void ProvaClasse(void);
};


#endif // !defined(__KKEP_GEN_H_INCLUDED)
