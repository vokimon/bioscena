// Comunitat.h: interface for the CComunitat class.
//
// CComunitat gestiona els aspectes que tenen a veure amb els
//////////////////////////////////////////////////////////////////////
// Change Log: 
// 19990306 VoK - Ja no hereda de CLlistaEstatica
// 19990306 VoK - Afegit membre 'CLlistaEstatica<CComunitatNode> nodes>
// 19990306 VoK - Afegit membre 'CTaxonomista txmist'
// 19990307 VoK - Esboçats els operadors per afegir, creuar, matar...
// 19990321 VoK - Arreglat una 
//////////////////////////////////////////////////////////////////////
// Pendent Log:
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_COMUNITAT_H_INCLUDED)
#define __KKEP_COMUNITAT_H_INCLUDED

#include "LlistaEstatica.h"
#include "Individu.h"
#include "Taxonomista.h"

class CComunitatNode//: protected CLlistaEstatica::Node
{
friend class CComunitat;
public:
	CIndividu *individu;
	uint32 posicio;
	uint32 taxo;
	CComunitatNode () {individu=NULL;};
};


class CComunitat
{
public:
// Acces
	inline void taxo (uint32 ind, uint32 tx);
	inline uint32 taxo (uint32 ind);
	inline void posicio (uint32 ind, uint32 pos);
	inline uint32 posicio(uint32 ind);
// Operacions
	bool creua(uint32 ind1, uint32 ind2);
	void envelleixTaxons();
	void defuncio (uint32);
	uint32 mitosi (uint32 ind);
	uint32 introdueix(CIndividu* ind);
// Construction/Destruction
	CComunitat(uint32 nIndividus);
	virtual ~CComunitat();
// Atributs
	CLlistaEstatica<CIndividu*> individus;
	CTaxonomista txmist;
// Debug
	static void ProvaClasse (void);
	ostream & dump(ostream & stream);
};


uint32 CComunitat::posicio(uint32 ind)
{
	return individus[ind]->posicio;
}

void CComunitat::posicio(uint32 ind, uint32 pos)
{
	individus[ind]->posicio = pos;
}

uint32 CComunitat::taxo(uint32 ind)
{
	return individus[ind]->taxo;
}

void CComunitat::taxo(uint32 ind, uint32 tx)
{
	individus[ind]->taxo = tx;
}


#endif // !defined(__KKEP_COMUNITAT_H_INCLUDED)
