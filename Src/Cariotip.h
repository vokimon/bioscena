// Cariotip.h: interface for the CCariotip class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19991119 VoK - Afegit linies alternatives al cpp per compilar amb
//                versions de la biblioteca STL sense 'assign'
// 19991207 VoK - Fix: La probabilitat de mutacio mai es complia
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_CARIOTIP_H_INCLUDED)
#define __KKEP_CARIOTIP_H_INCLUDED

#include <vector>
#include "Cromosoma.h"
// Forwarding
class CMutacioCariotip;

class CCariotip  
{
// Tipus Propis
public:
	typedef CCromosoma * t_cromosoma;
	typedef vector<t_cromosoma> t_cromosomes;
// Construccio/Destruccio
public:
	CCariotip();
	virtual ~CCariotip();
// Redefinibles
public:
	virtual void dump(CMissatger & msg);
	virtual istream & load(istream & str);
	virtual ostream & store(ostream & str);
// Operacions
public:
	bool init (const CCariotip &c);
	bool init (uint32 nCromosomes);
	bool init (uint32 primer, uint32 ultim);
//	t_cromosoma & operator [ ](const uint32 n);
	t_cromosoma & operator [ ](uint32 n);
	const t_cromosoma & operator [ ](uint32 n) const;
	uint32 cromosomaAleatori(void) const;
	uint32 tamany() const;

	void afegeix(t_cromosoma & c, uint32 index);
	void afegeix(t_cromosoma & c);
	t_cromosoma extreu(uint32 index);
	t_cromosoma extreu();

// Atributs
protected:
	t_cromosomes m_cromosomes;
// Proves
public:
	uint32 tamanyCodons();
	bool muta(CMutacioCariotip * mutacio=NULL);
	static void ProvaClasse(void);	
// Implementacio
private:
	bool ocupaCromosomes(uint32 nCromosomes);
	void alliberaCromosomes(void);

	void ompleCromosomesCopiant(t_cromosomes cromosomes);
	void ompleCromosomesAleatoriament(uint32 minLong, uint32 maxLong);
	void ompleCromosomesSequencialment(uint32 primer);

//	void copiaCodonsSuperficialment(uint32 * codons, uint32 nCodons);
};

#endif // !defined(__KKEP_CARIOTIP_H_INCLUDED)
