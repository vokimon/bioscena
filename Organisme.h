// Organisme.h: interface for the COrganisme class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_ORGANISME_H_INCLUDED)
#define __KKEP_ORGANISME_H_INCLUDED

#include <list>
#include "EnergiaDisipable.h"

#define OrganismeCaducitatEnergia	10
#define OrganismeNumRegistres		32
// Algunos registros especiales
#define R_METABOLISME	1

class COrganisme  
{
// Tipus de la classe
	typedef uint32 t_mollecula;
// Construcció/Destruccio
public:
	COrganisme();
	virtual ~COrganisme();
// Atributs
public:
	uint32 m_registres[64];
	list<t_mollecula> m_nutrients;
	CEnergiaDisipable m_energia;
// Operacions
public:
// Proves
public:
	void registre(uint32 index, uint32 valor);
	uint32 registre(uint32);
	void debugPresentaNutrients(CMissatger &msgr);
	void engulleix(t_mollecula mol);
	t_mollecula metabolitza(uint32 patro, uint32 tolerancia);
	void operator () (void);
	static void ProvaClasse();
};



#endif // !defined(__KKEP_ORGANISME_H_INCLUDED)
