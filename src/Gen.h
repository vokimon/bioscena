// Gen.h: interface for the CGen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_GEN_H_INCLUDED)
#define __KKEP_GEN_H_INCLUDED

#include <vector>
#include "BioIncludes.h"
#include "Cromosoma.h"

class CGen  
{
// Tipus interns
public:
	typedef CGen self;
	typedef void inherited;
//	typedef void super;
	// L'operador es la part que determina si el gen s'executa o no
	struct t_operador {
		uint32 registre;
		uint32 valor;
		uint32 tolerancia;
		uint32 intolerancia;
	};
	typedef uint32 t_instruccio;
	typedef vector<t_instruccio> t_instruccions;
// Construction/Destruction
public:
	CGen();
	virtual ~CGen();
// Redefinibles
public:
// Operacions
public:
	bool init(CCromosoma & crm, uint32 & pos);
	bool traduible(uint32 * fenotip);
	bool salt(uint32 patro);
	bool seguentInstruccio(uint32 & t_instruccio);
	bool parametres(uint32 & valor);
	bool finalitzat();
	bool esIntro(uint32 codo);
	bool esPromotor(uint32 codo);
	t_instruccio traduccio(uint32 codo);
	bool esOperadora();
	uint32 tamany();
// Atributs
public:
	t_instruccions m_instruccions;
	uint32 m_ip;
// Proves
public:
	bool condicioOperadora(uint32 * fenotip);
	static void ProvaClasse(void);
};


#endif // !defined(__KKEP_GEN_H_INCLUDED)
