// Substrat.h: interface for the CSubstrat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_SUBSTRAT_H_INCLUDED)
#define __KKEP_SUBSTRAT_H_INCLUDED

#include <list>
#include "BioIncludes.h"

class CSubstrat  
{
// Els tipus propis
	typedef uint32 t_mollecula;
// Algunes amigues
	friend ostream & operator<< (ostream & stream, CSubstrat s);
// Atributs
private:
	list<t_mollecula> m_nutrients; // Llista de nutrients presents
	bool m_ocupat; // Cert si no hi cap cap altre organisme
	uint32 m_ocupant; // Identificador de l'organisme, si n'hi ha
//	uint32 m_costSortida;
//	uint32 m_idonietatNutricio;
//	uint32 m_idonietatMoviment;
	uint32 m_limitMollecules;
// Construccio/Destruccio
public:
	CSubstrat();
	virtual ~CSubstrat();
// Redefinibles
	virtual ostream & store(ostream & str);
	virtual istream & load(istream & str);
// Operacions (Inquilins)
public:
	void ocupa(uint32 idOrganisme);
	void desocupa();
	bool esOcupat();
	uint32 ocupant();
	CSubstrat operator=(int i) {ocupa(i); return *this;};
// Operacions (Quimica)
public:
	bool rastreja(t_mollecula &n, uint32 clau, uint32 tolerancia);
	bool extreu  (t_mollecula &n, uint32 clau, uint32 tolerancia);
	void deposita(t_mollecula n);
	void limitMollecules(uint32 n);
	uint32 limitMollecules();
	uint32 numeroMollecules();
// Proves
public:
	void dump (CMissatger & out);
	static void ProvaClasse();
};

// Funciones no miembros
ostream & operator<< (ostream & stream, CSubstrat s);

#endif // !defined(__KKEP_SUBSTRAT_H_INCLUDED)
