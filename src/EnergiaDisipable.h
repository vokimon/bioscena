// EnergiaDisipable.h: interface for the CEnergiaDisipable class.
//
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
	virtual void dump(CMissatger & msg);
	virtual istream & load(istream & str);
	virtual ostream & store(ostream & str);
// Operacions
public:
	uint32 consumeix(uint32 energia);
	void afegeix(uint32 energia);
	void disipa (void);
	operator uint32() const;
	uint32 total() const;
// Proves
public:
	static void ProvaClasse();
};

ostream& operator<< (ostream& stream, CEnergiaDisipable t);

#endif // !defined(__KKEP_ENERGIADISIPABLE_H_INCLUDED)
