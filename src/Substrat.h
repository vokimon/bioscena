// Substrat.h: interface for the CSubstrat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_SUBSTRAT_H_INCLUDED)
#define __KKEP_SUBSTRAT_H_INCLUDED

#include <list>
#include "BioIncludes.h"
//#include "Nutrient.h"

class CSubstrat  
{
// Los tipos propios
	typedef uint32 t_mol;
// Algunas amigas
	friend ostream & operator<< (ostream & stream, CSubstrat s);
// Atributs
private:
	list<uint32> m_nutrients; // Llista de nutrients presents
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
	CSubstrat operator=(int i) {ocupa(i); return *this;};
// Operacions (Quimica)
public:
	void limitaMollecules(uint32 n);
	bool agafaMollecula(uint32 &n, uint32 clau, uint32 tolerancia);
	void afegeixMollecula(uint32 n);
	uint32 numeroMollecules();
// Proves
public:
	static void ProvaClasse();
};

// Funciones no miembros
ostream & operator<< (ostream & stream, CSubstrat s);

#endif // !defined(__KKEP_SUBSTRAT_H_INCLUDED)
