// Gen.h: interface for the CGen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_GEN_H_INCLUDED)
#define __KKEP_GEN_H_INCLUDED

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
	uint32 seguentInstruccio();
	uint32 seguentInstruccio(uin32 patro);
// Atributs
public:
// Proves
public:
	static void ProvaClasse(void);	
};

#endif // !defined(__KKEP_GEN_H_INCLUDED)
