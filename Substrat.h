// Substrat.h: interface for the CSubstrat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_SUBSTRAT_H_INCLUDED)
#define __KKEP_SUBSTRAT_H_INCLUDED

#include <list>
#include "BioIncludes.h"
#include "Nutrient.h"
#include "Encaix.h"

class CSubstrat  
{
// Algunas amigas
	friend ostream & operator<< (ostream & stream, CSubstrat s);
// Atributs
private:
	list<CNutrient> m_nutrients; // Llista de nutrients presents
	bool m_ocupat; // Cert si no hi cap cap altre organisme
	uint32 m_ocupant; // Identificador de l'organisme, si n'hi ha
	uint32 m_costSortida;
	uint32 m_idonietatNutricio;
	uint32 m_idonietatMoviment;
// Construccio/Destruccio
public:
	CSubstrat();
	virtual ~CSubstrat();
// Operacions (Inquilins)
public:
	void ocupa(uint32 idOrganisme);
	void desocupa();
	bool esOcupat();
	uint32 ocupant();
// Operacions (Quimica)
public:
	bool getNutrient(CNutrient &n, CEncaix<uint32> clau, uint32 tolerancia);
	void addNutrient(CNutrient &n);
// Proves
public:
	static void ProvaClasse();
};

// Funciones no miembros
ostream & operator<< (ostream & stream, CSubstrat s);

#endif // !defined(__KKEP_SUBSTRAT_H_INCLUDED)
