// Cromosoma.h: interface for the CCromosoma class.
//
//////////////////////////////////////////////////////////////////////

#include "BioIncludes.h"

#if !defined(__KKEP_CROMOSOMA_H_INCLUDED)
#define __KKEP_CROMOSOMA_H_INCLUDED

class CCromosoma  
{
// Algunes amigues
	friend class CMutacioGenica;
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
	CCromosoma & operator= (CCromosoma & c);
	uint32 & operator [ ](uint32 n);
	uint32 codoAleatori(void) const;
	uint32 tamany(void) const;
	void fusiona(CCromosoma const &c);
	void parteix(CCromosoma &c, uint32 centromer);
	void treuCodons(uint32 primer, uint32 longitud);
// Atributs
protected:
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
