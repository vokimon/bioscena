// Gen.h: interface for the CGen class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19991201 VoK - Fix: Errors calculant la condicio operadora
// 19991201 VoK - Es pot especificar la mascara de la regio operadora
//                des del fitxer de configuracio
// 19991202 VoK - Procediment de proves. Dumping
// 19991202 VoK - Fix: Inicialitzacions
// 19991202 VoK - Fix: Mecanismes d'expressio en general
// 19991202 VoK - Fix: Mecanismes de traduccio
// 19991202 VoK - Mascares per Promotors i Introns configurables
// 19991202 VoK - Implementats els Terminadors
// 19991212 VoK - Canvi condicioOperadora: Fenotips 32 -> 16 bits
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
	bool esTerminador(uint32 codo);
	t_instruccio traduccio(uint32 codo);
	bool esOperadora();
	uint32 tamany();
	bool condicioOperadora(uint32 * fenotip);
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
