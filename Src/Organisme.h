// Organisme.h: interface for the COrganisme class.
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
	COrganisme(string tipus);
	COrganisme(string tipus, CCariotip &c);
	virtual ~COrganisme();
// Redefinibles
public:
	virtual void dump(CMissatger &msgr);
	virtual istream & load(istream & str);
	virtual ostream & store(ostream & str);
// Atributs
public:
	// Fenotip
	uint32 * m_fenotip;
	// Nutricio
	list<t_mollecula> m_nutrients;
	CEnergiaDisipable m_energia;
	uint32 m_tempsDisipacio;
	uint32 m_tempsDisipacioRestant;
	// Control
	CCariotip m_cariotip;
	CControlOrganisme * m_genotip;
	// Dades tontes
	uint32 m_edat;
public:
	// Debug
	uint32 m_foo; // TODO: Treure aixo
	bool m_primigeni; // TODO: Treure aixo
	bool m_mutat; // TODO: Treure aixo
// Operacions
public:
//	void operator () (void);
	// Control
	t_instruccio seguentInstruccio();
	// Fenotip
	void fenotip(uint32 index, uint32 valor);
	uint32 fenotip(uint32);
	uint32 & operator[](uint32);
	// Metabolisme
	bool detecta(uint32 & clauDetectada, uint32 patro, uint32 tolerancia);
	bool excreta(t_mollecula & excretada, uint32 patro, uint32 tolerancia);
	void engoleix(t_mollecula mol);
	bool defensa(list<t_mollecula> &fluxeQuimic, uint32 patroAtac, uint32 patroNutrient, uint32 toleranciaNutrient);
	bool consumeixEnergia(uint32 energia);
	void guanyaEnergia(uint32 energia);
	// Consulta
	uint32 energia() const;
	uint32 carrega() const;
	uint32 edat() const;
	// 
// Proves
public:
	void debugPresentaNutrients(CMissatger &msgr);
	void debugPresentaFenotip(CMissatger &msgr);
	static void ProvaClasse();
};

#endif // !defined(__KKEP_ORGANISME_H_INCLUDED)
