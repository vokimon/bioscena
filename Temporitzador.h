// Temporitzador.h: interface for the CTemporitzador class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_TEMPORITZADOR_H_INCLUDED)
#define __KKEP_TEMPORITZADOR_H_INCLUDED

#include <string>
#include "BioIncludes.h"
#include "RandomStream.h"
#include "Missatger.h"
#include "MultiAgent.h"

class CTemporitzador : public CMultiAgent
{
	typedef CAgent t_accio;
//	typedef void (TipusAccio)(); // Aixo es per fer proves nomes
	struct t_cicle 
	{
		uint32 m_periodeMinim;
		uint32 m_magnitudDau;
		uint32 m_nombreDaus;
		uint32 periode ()
		{
			uint32 periode;
			// Inicialitzem am el periode minim del cicle
			periode=m_periodeMinim;
			// I li afegim l'extra dels daus
			for (uint32 i=m_nombreDaus; i--;)
				periode+=rnd.get(0,m_magnitudDau);
			return periode;
		}
	};

// Construccio/Destruccio
public:
	CTemporitzador();
	virtual ~CTemporitzador();
// Operacions
public:
	virtual list<CAgent*> subordinats();
	void operator()(void);
	void cicleActiu(uint32 periode, uint32 margeDau=1, uint32 daus=0);
	void cicleInactiu(uint32 periode, uint32 margeDau=1, uint32 daus=0);
	void modificaCicleActual(uint32 m_restantPeriode);
	void modificaCicleActual(bool actiu, uint32 m_restantPeriode);
private:
	// No fixat al protocol, son aqui per debug
	void antiAccio(t_accio * a);
// Atributs
public:
	bool m_actiu;
	uint32 m_periodeActual;
	t_cicle m_cicleActiu;
	t_cicle m_cicleInactiu;
private:
	// No fixat al protocol, son aqui per debug
	t_accio * m_antiAccio;
// Proves
public:
	virtual void dump(CMissatger & msg);
	static void ProvaClasse();
// Implementacio
private:
	void recalculaActual();
};


#endif // !defined(__KKEP_TEMPORITZADOR_H_INCLUDED)
