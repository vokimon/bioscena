// Nutrient.h: interface for the CNutrient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_NUTRIENT_H_INCLUDED)
#define __KKEP_NUTRIENT_H_INCLUDED
#include "BioIncludes.h"

class CNutrient  
{
// Construccio/Destruccio
public:
	CNutrient();
	CNutrient(uint32 idonietat, uint32 quantitat=1);
	virtual ~CNutrient();
// Operacions
public:
/*	bool operator>(CNutrient n);
	bool operator<(CNutrient n);
	bool operator==(CNutrient n);
	bool operator!=(CNutrient n);
*/	CNutrient & operator=(CNutrient n);
// Atributs
public:
	uint32 m_idonietat; // Clau de compatibilitat
	uint32 m_quantitat; // Quantitat de l'aliment que es representa
};

#endif // !defined(__KKEP_NUTRIENT_H_INCLUDED)
