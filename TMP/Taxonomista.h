// Taxonomista.h: interface for the CTaxonomista class.
//
//////////////////////////////////////////////////////////////////////
// Change Log: 
// 19990306 VoK - Implementat 'parentesc'
// 19990306 VoK - Implementat 'discrimina'
// 19990306 VoK - Adaptat 'taxoAmbIndex' per les modificacions que es
//          VoK - van fer 'CLlistaEstatica'
//////////////////////////////////////////////////////////////////////
// Pendent Log:
// 19990306 VoK - Fer que la llista de taxons sigui un membre i no 
//          una classe base
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_TAXONOMISTA_H_INCLUDED)
#define __KKEP_TAXONOMISTA_H_INCLUDED

#include "LlistaEstatica.h"
#include "Taxo.h"

class CTaxonomistaNode
{
friend class CTaxonomista;
public:
	CTaxo *taxo;
	uint32 cens;
	uint32 naixements;
	uint32 defuncions;
	CTaxonomistaNode () {taxo=NULL;};
};

class CTaxonomista : public CLlistaEstatica <CTaxonomistaNode>
{
public:
// Operacions
	bool defuncio (uint32 tx);
	void naixement (uint32 tx);
	uint32 nouTaxoIndependent (void);
	bool creuaTaxons (uint32 tx1, uint32 tx2);
	void envelleixTaxons();
	uint32 discrimina (uint32 taxo);
	uint32 parentesc (uint32 taxo1, uint32 taxo2);
// Acces
	inline CTaxo *taxoAmbIndex (uint32 i);

// Construccio/Destruccio
	CTaxonomista(uint32 nNivells);
	virtual ~CTaxonomista();
// Debug
	ostream & dump (ostream& stream);
	void static  ProvaClasse();
// Atributs
	uint32 nivells;
};

CTaxo *CTaxonomista::taxoAmbIndex(uint32 i)
{
	return valors[i].taxo;
}

#endif // !defined(__KKEP_TAXONOMISTA_H_INCLUDED)
