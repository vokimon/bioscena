// EnergiaDisipable.h: interface for the CEnergiaDisipable class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 199908?? VoK - Creat
// 19991126 VoK - Fix: No feiem delete dels slots d'energia
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_ENERGIADISIPABLE_H_INCLUDED)
#define __KKEP_ENERGIADISIPABLE_H_INCLUDED

#include "BioIncludes.h"

class CEnergiaDisipable  
{
// Amigues
	friend ostream& operator<< (ostream& stream, CEnergiaDisipable t);
// Construcció/Destruccio
public:
	CEnergiaDisipable(uint32 slots);
	virtual ~CEnergiaDisipable();
// Atributs
public:
	uint32 m_total;
	uint32 m_nSlots;
	uint32 *m_slots;
	uint32 m_slotActual;
// Operacions
public:
	uint32 consumeix(uint32 energia);
	void afegeix(uint32 energia);
	void operator () (void);
	operator uint32();
// Proves
public:
	uint32 total();
	static void ProvaClasse();
};

ostream& operator<< (ostream& stream, CEnergiaDisipable t);

#endif // !defined(__KKEP_ENERGIADISIPABLE_H_INCLUDED)
