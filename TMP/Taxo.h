// Taxon.h: interface for the CTaxon class.
//
//////////////////////////////////////////////////////////////////////
// Change Log: 
// 19990306 VoK - Implementat constructor per copia
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_TAXO_H_INCLUDED)
#define __KKEP_TAXO_H_INCLUDED

#include "BioIncludes.h"

class CTaxo
{
	friend class CTaxonomista;
public:
	const uint32 nMarques;
	uint32 *marques;

	CTaxo(uint32 nMarques);
	CTaxo (CTaxo &taxo);
	virtual ~CTaxo();

	inline uint32 & operator[] (uint32 nivell);
	inline uint32 & marca (uint32 nivell);
	inline const  bool operator< (const CTaxo t);
	inline const  bool operator== (const CTaxo t);
	void marca (uint32 nivell, uint32 valor);
	uint32 cosanguineitatAmb (CTaxo *taxo);
	void shiftaMarques (uint32 &mesAntiga, uint32 &menysAntiga, uint32 &nova);
	void lligaAmb(uint32 cosanguinietat, uint32 &diferencial, uint32 &substituit, uint32 &nouDiferencial);
	

	void static ProvaClasse();
	friend ostream& operator<< (ostream& stream, CTaxo t);
};

uint32 & CTaxo::operator[] (uint32 nivell)
{
	return marques[nivell];
}
uint32 & CTaxo::marca(uint32 nivell)
{
	return marques[nivell];
}
const bool CTaxo::operator< (const CTaxo t)
{
	// Tenen mes pes els de major index (mes antics)
	uint32 i;
	for (i=nMarques; --i && marques[i]!=t.marques[i];); 
	return marques[i]<t.marques[i];
}

const bool CTaxo::operator== (const CTaxo t)
{
	// Tenen mes pes els de major index (mes antics)
	uint32 i;
	for (i=nMarques; --i && marques[i]!=t.marques[i];); 
	return marques[i]==t.marques[i];
}

inline ostream& operator<< (ostream& stream, CTaxo t)
{
	uint32 i;
	stream << '[';
	for (i=t.nMarques; ((stream << t.marques[--i]),1) && i; ) stream << '.';
	stream << ']';
	return stream;
}

#endif // !defined(__KKEP_TAXO_H_INCLUDED)

