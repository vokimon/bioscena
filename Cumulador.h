// Cumulador.h: interface for the CCumulador class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_CUMULADOR_H_INCLUDED)
#define __KKEP_CUMULADOR_H_INCLUDED

#include "TopologiaToroidal.h"
#include "Substrat.h"
#include "Agent.h"
#include "Posicionador.h"

class CCumulador : public CAgent
{
// Construccio/Destruccio
public:
	CCumulador(CTopologia<CSubstrat>& biotop, CPosicionador *pos);
	virtual ~CCumulador();
// Operadors (Per accionar)
public:
	virtual void operator () (void)
	{
		uint32 posicioMitja = m_posicionador->pos();
		for (int i=m_densitat;i--;) {
			uint32 pos = posicioMitja;
			for (int j=m_radi; j--;)
				pos = m_biotop.desplacament(pos,rnd.get()); 
			uint32 mol = m_element^(m_variabilitat&rnd.get());
			m_biotop[pos].afegeixMollecula(mol);
		}
	}
// Operadors (Per configurar)
public:
	void distribucio(uint32 densitat, uint32 radio=1);
	void composicio(uint32 element, uint32 variabilitat=0);
	void posicio(CPosicionador *pos);
// Atributs
public:
	CTopologia<CSubstrat>& m_biotop; // Biotop on s'aplica
	uint32 m_element; // Element basic que s'afegira
	uint32 m_variabilitat; // Mascara amb els bits de l'element que poden variar
	uint32 m_densitat; // Numero de elements inserits per execucio
	uint32 m_radi; // Desplacaments que es pot separar del centre
	CPosicionador *m_posicionador; // Objecte que determina el centre
// Proves
public:
	static void ProvaClasse();
};

class CClarejador : public CCumulador
{
// Construccio/Destruccio
public:
	CClarejador(CTopologia<CSubstrat>& biotop, CPosicionador *pos)
		:CCumulador(biotop, pos) {}
// Operadors (Per accionar)
public:
	void operator () (void)
	{
		uint32 posicioMitja = m_posicionador->pos();
		for (int i=m_densitat;i--;) {
			uint32 pos = posicioMitja;
			for (int j=m_radi; j--;)
				pos = m_biotop.desplacament(pos,rnd.get()); 
			uint32 nutrient;
			m_biotop[pos].agafaMollecula(nutrient, m_element,m_variabilitat);
		}
	}
// Proves
public:
	static void ProvaClasse();
};

#endif // !defined(__KKEP_CUMULADOR_H_INCLUDED)
