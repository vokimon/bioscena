// Temporitzador.h: interface for the CTemporitzador class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_TEMPORITZADOR_H_INCLUDED)
#define __KKEP_TEMPORITZADOR_H_INCLUDED

#include <string>
#include "BioIncludes.h"
#include "RandomStream.h"
#include "Missatger.h"

class CTemporitzador 
{
	struct t_cicle 
	{
		uint32 m_periodeMinim;
		uint32 m_magnitudDau;
		uint32 m_nombreDaus;
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
	typedef CAgent t_accio;
//	typedef void (*TipusAccio)(); // Aixo es per fer proves nomes

// Construccio/Destruccio
public:
	CTemporitzador();
	virtual ~CTemporitzador();
// Operacions
public:
	void tic ();
	void cicleActiu(uint32 periode, uint32 margeDau=0, uint32 daus=1);
	void cicleDesactiu(uint32 periode, uint32 margeDau=0, uint32 daus=1);
	void probabilitat(uint32 mostra,uint32 encerts=1);
	void accio(TipusAccio a);
// Atributs
public:
	string m_nom;
	bool m_actiu;
	bool m_accionat;
	uint32 m_periodeActual;
	t_cicle m_cicleActiu;
	t_cicle m_cicleDesactiu;
	t_probabilitat_saxona m_probabilitat;
	t_accio * m_accio;
// Proves
public:
	static void ProvaClasse();
// Implementacio
private:
	void recalculaActual();
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTemporitzador::CTemporitzador()
{
	m_nom="Anonim";
	m_cicleActiu.m_periodeMinim=1;
	m_cicleActiu.m_nombreDaus=0;
	m_cicleActiu.m_magnitudDau=0;
	m_cicleDesactiu.m_periodeMinim=0;
	m_cicleDesactiu.m_nombreDaus=0;
	m_cicleDesactiu.m_magnitudDau=0;
	m_probabilitat.m_encerts=1;
	m_probabilitat.m_mostra=1;
	m_accio = NULL;
	m_actiu = true;
	m_accionat = false;
}

CTemporitzador::~CTemporitzador()
{

}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

void CTemporitzador::cicleActiu(uint32 periode, uint32 margeDau, uint32 nombreDaus)
{
	m_cicleActiu.m_periodeMinim=periode;
	m_cicleActiu.m_nombreDaus=nombreDaus;
	m_cicleActiu.m_magnitudDau=margeDau;
	recalculaActual();
}

void CTemporitzador::cicleDesactiu(uint32 periode, uint32 margeDau, uint32 nombreDaus)
{
	m_cicleDesactiu.m_periodeMinim=periode;
	m_cicleDesactiu.m_nombreDaus=nombreDaus;
	m_cicleDesactiu.m_magnitudDau=margeDau;
	recalculaActual();
}

void CTemporitzador::probabilitat(uint32 mostra, uint32 encerts)
{
	m_probabilitat.m_mostra=mostra;
	m_probabilitat.m_encerts=encerts;
}

void CTemporitzador::accio(TipusAccio a)
{
	m_accio=a;
}

void CTemporitzador::tic()
{
	m_accionat=false;
	if (!m_periodeActual) {
		// S'acabat el periode actual, canviem a l'altre
		m_actiu = !m_actiu;
		recalculaActual();
	}
	if (m_actiu) 
	{
		if (!m_accio) 
			warning << "Executant agent sense accio associada" << endl;

		// Nomes si es dona la probabilitat i existeix l'accio, l'executem
		if (m_accio && m_probabilitat.esDona()) 
		{
			(*m_accio)();
			m_accionat=true;
		}
//		else out<<"caca ";
	}
	m_periodeActual--;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////
void hola(); // Definit al .cpp

void CTemporitzador::ProvaClasse()
{
	CTemporitzador ag;
	ag.cicleDesactiu(4,0,0);
	ag.cicleActiu(0,2,3);
	ag.probabilitat(20,15);
	ag.accio(hola);
	for (int i=40; i--;) {
		out << i << " - ";
		ag.tic();
		out << endl;
	}
}

//////////////////////////////////////////////////////////////////////
// Ìmplementacio
//////////////////////////////////////////////////////////////////////

void CTemporitzador::recalculaActual()
{
	t_cicle & nouCicle = m_actiu?m_cicleActiu:m_cicleDesactiu;
	// Inicialitzem am el periode minim del cicle
	m_periodeActual=nouCicle.m_periodeMinim;
	// I li afegim l'extra dels daus
	for (uint32 i=nouCicle.m_nombreDaus; i--;)
		m_periodeActual+=rnd.get(0,nouCicle.m_magnitudDau);
	if (!m_periodeActual) recalculaActual();
}

#endif // !defined(__KKEP_TEMPORITZADOR_H_INCLUDED)
