// Gen.h: interface for the CGen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_GEN_H_INCLUDED)
#define __KKEP_GEN_H_INCLUDED

#include <vector>
#include "BioIncludes.h"
#include "Missatger.h"
#include "Cromosoma.h"

class CGen  
{
// Tipus interns
public:
	typedef CGen self;
	typedef void inherited;
//	typedef void super;
	typedef uint32 t_instruccio;
	typedef vector<t_instruccio> t_instruccions;
// Construction/Destruction
public:
	CGen();
	virtual ~CGen();
// Redefinibles
public:
// Operacions traduccio
public:
	bool init(CCromosoma & crm, uint32 & pos);
	bool esIntro(uint32 codo);
	bool esPromotor(uint32 codo);
	bool esTerminador(uint32 codo);
	t_instruccio traduccio(uint32 codo);
// Operacions expresio
public:
	bool traduible(const uint32 * fenotip);
	bool seguentInstruccio(uint32 & t_instruccio);
	bool parametres(uint32 & valor);
	bool finalitzat();
	void reset();
	bool esOperadora();
	bool condicioOperadora(const uint32 * fenotip);
// Operacions expresio
public:
	uint32 tamany();
	void dump(CMissatger & msg);
// Atributs
public:
	t_instruccions m_instruccions;
	uint32 m_ip;
// Proves
public:
	static void ProvaClasse(void);
};


#endif // !defined(__KKEP_GEN_H_INCLUDED)
