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
#include "Cariotip.h"
#include "Genotip.h"

class COrganisme  
{
// Tipus de la classe
public:
	typedef uint32 t_mollecula;
	typedef uint32 t_instruccio;
// Construcció/Destruccio
public:
	COrganisme();
	COrganisme(CCariotip &c);
	virtual ~COrganisme();
// Atributs
public:
	// Fenotip
	uint32 * m_fenotip;
	// Nutricio
	list<t_mollecula> m_nutrients;
	CEnergiaDisipable m_energia;
	// Control
	CCariotip m_cariotip;
	CGenotip m_genotip;
	uint32 m_lecturaDiferida;
	uint32 m_edat;
// Operacions
public:
	void operator () (void);
	t_instruccio seguentInstruccio();
	// Fenotip
	void fenotip(uint32 index, uint32 valor);
	uint32 fenotip(uint32);
	uint32 & operator[](uint32);
	// Nutricio
	bool anabolitza(uint32 & energia, uint32 A, uint32 toleranciaA, uint32 B, uint32 toleranciaB);
	bool catabolitza(uint32 & energia, uint32 C, uint32 toleranciaC, uint32 clauCatabolica);
	bool excreta(t_mollecula & excrecio, uint32 patro, uint32 tolerancia);
	void engoleix(t_mollecula mol);
	bool defensa(list<t_mollecula> &fluxeQuimic, uint32 patroAtac, uint32 patroNutrient, uint32 toleranciaNutrient);
	bool consumeix(uint32 energia);
	uint32 energia();
	uint32 edat();
	// 
// Proves
public:
	void dump(CMissatger &msgr);
	void debugPresentaNutrients(CMissatger &msgr);
	void debugPresentaFenotip(CMissatger &msgr);
	static void ProvaClasse();
};

#endif // !defined(__KKEP_ORGANISME_H_INCLUDED)
