// Temporitzador.h: interface for the CTemporitzador class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_TEMPORITZADOR_H_INCLUDED)
#define __KKEP_TEMPORITZADOR_H_INCLUDED

#include <string>
#include "BioIncludes.h"
#include "RandomStream.h"
#include "Missatger.h"
#include "Agent.h"

class CTemporitzador : public CAgent
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
	struct t_probabilitat_saxona
	{
		bool esDona()
		{
			return rnd.get(0,m_mostra-1)<m_encerts;
		};
		uint32 m_encerts;
		uint32 m_mostra;
	};

// Construccio/Destruccio
public:
	CTemporitzador();
	virtual ~CTemporitzador();
// Operacions
public:
	void operator()(void);
	void cicleActiu(uint32 periode, uint32 margeDau=1, uint32 daus=0);
	void cicleDesactiu(uint32 periode, uint32 margeDau=1, uint32 daus=0);
	void probabilitat(uint32 mostra,uint32 encerts=1);
	void accio(t_accio * a);
private:
	// No fixat al protocol, son aqui per debug
	void antiAccio(t_accio * a);
	void reAccio(t_accio * a);
// Atributs
public:
	bool m_actiu;
	bool m_accionat;
	uint32 m_periodeActual;
	t_cicle m_cicleActiu;
	t_cicle m_cicleDesactiu;
	t_probabilitat_saxona m_probabilitat;
	t_accio * m_accio;
private:
	// No fixat al protocol, son aqui per debug
	t_accio * m_antiAccio;
	t_accio * m_reAccio;
// Proves
public:
	static void ProvaClasse();
// Implementacio
private:
	void recalculaActual();
};


#endif // !defined(__KKEP_TEMPORITZADOR_H_INCLUDED)
