// Organisme.h: interface for the COrganisme class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990902 VoK - 
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_ORGANISME_H_INCLUDED)
#define __KKEP_ORGANISME_H_INCLUDED

#include <list>
#include "EnergiaDisipable.h"

#define OrganismeCaducitatEnergia	10
#define OrganismeLongitudFenotip	32
#define OrganismeEnergiaMinima		30

class COrganisme  
{
// Tipus de la classe
	typedef uint32 t_mollecula;
	typedef uint32 t_instruccio;
// Construcció/Destruccio
public:
	COrganisme();
	virtual ~COrganisme();
// Atributs
public:
	uint32 * m_fenotip;
	list<t_mollecula> m_nutrients;
	CEnergiaDisipable m_energia;
	uint32 m_lecturaDiferida;
// Operacions
public:
	void operator () (void);
	// Fenotip
	void fenotip(uint32 index, uint32 valor);
	uint32 fenotip(uint32);
	uint32 & operator[](uint32);
	// Nutrients
	bool anabolitza(uint32 & energia, uint32 A, uint32 toleranciaA, uint32 B, uint32 toleranciaB);
	bool catabolitza(uint32 & energia, uint32 C, uint32 toleranciaC, uint32 clauCatabolica);
	bool excreta(t_mollecula & excrecio, uint32 patro, uint32 tolerancia);
	void engoleix(t_mollecula mol);
	// 
// Proves
public:
	t_instruccio seguentInstruccio();
	void debugPresentaNutrients(CMissatger &msgr);
	void debugPresentaFenotip(CMissatger &msgr);
	static void ProvaClasse();
};

#endif // !defined(__KKEP_ORGANISME_H_INCLUDED)
