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
// TODO:
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_COMUNITAT_H_INCLUDED)
#define __KKEP_COMUNITAT_H_INCLUDED

#include "LlistaEstatica.h"
#include "InfoOrganisme.h"
#include "Taxonomista.h"

class CComunitat  
{
// Construccio/Destruccio
public:
	CComunitat(uint32 maxOrganismes);
	virtual ~CComunitat();
// Operacions (Acces)
public:
	CInfoOrganisme & operator[](uint32 index) {
		return m_organismes[index];
	}
// Operacions
public:
	bool creua(uint32 ind1, uint32 ind2);
	void envelleixTaxons();
	void defuncio (uint32);
	uint32 mitosi (uint32 ind);
	uint32 introdueix(COrganisme* ind);
// Atributs
public:
	CLlistaEstatica<CInfoOrganisme> m_organismes;
	CTaxonomista m_txmist;
	uint32 m_maxOrganismes;
// Proves
public:
	uint32 organismeAleatori();
	static void ProvaClasse (void);
	ostream & dump(ostream & stream);
};

#endif // !defined(__KKEP_COMUNITAT_H_INCLUDED)
