// Cromosoma.h: interface for the CCromosoma class.
//
//////////////////////////////////////////////////////////////////////

#include "BioIncludes.h"

#if !defined(__KKEP_CROMOSOMA_H_INCLUDED)
#define __KKEP_CROMOSOMA_H_INCLUDED

class CCromosoma  
{
// Construccio/Destruccio
public:
	CCromosoma();
	virtual ~CCromosoma();
// Redefinibles
public:
	virtual void dump(CMissatger & msg);
// Operacions
public:
	bool init(CCromosoma & c);
	bool init(uint32 nCodons);
	bool init(uint32 primer, uint32 ultim);
	uint32 & operator [ ](uint32 n);
	uint32 codoAleatori(void);
	uint32 tamany(void);
// Operadors Unaris (Mutacions)
public:
	CCromosoma & mutacioPuntualBinariaGaussiana(void);
	CCromosoma & mutacioPuntualBinaria(void);
	CCromosoma & mutacioPuntualDrastica(void);
	CCromosoma & mutacioDesplacament (void);
	CCromosoma & mutacioInversio (void);
	CCromosoma & mutacioInsercioReplicada(void);
	CCromosoma & mutacioInsercioAleatoria(void);
	CCromosoma & mutacioDeleccio(void);
	CCromosoma & randomize (void);
// Atributs
public:
	uint32 * m_codons;
	uint32 m_nCodons;
	bool m_enSocPropietari;
// Proves
public:
	static void ProvaClasse(void);	
// Implementacio
private:
	void initCodons(void);
	bool ocupaCodons(uint32 nCodons);
	void copiaCodonsSuperficialment(uint32 * codons, uint32 nCodons);
	void alliberaCodons(void);
	void ompleCodonsCopiant(uint32 * codons);
	void ompleCodonsAleatoriament();
	void ompleCodonsSequencialment(uint32 primer);

};

#endif // !defined(__KKEP_CROMOSOMA_H_INCLUDED)
