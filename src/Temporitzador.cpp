// Temporitzador.cpp: implementation of the CTemporitzador class.
//
//////////////////////////////////////////////////////////////////////

#include "Temporitzador.h"
#include "Missatger.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTemporitzador::CTemporitzador()
{
	m_cicleActiu.m_periodeMinim=1;
	m_cicleActiu.m_nombreDaus=0;
	m_cicleActiu.m_magnitudDau=1;
	m_cicleDesactiu.m_periodeMinim=0;
	m_cicleDesactiu.m_nombreDaus=0;
	m_cicleDesactiu.m_magnitudDau=1;
	m_probabilitat.m_encerts=1;
	m_probabilitat.m_mostra=1;
	m_accio = NULL;
	m_antiAccio = NULL;
	m_reAccio = NULL;
	m_actiu = true;
	m_accionat = false;
	m_nom = "Temporitzador" + 23;
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

void CTemporitzador::accio(t_accio * a)
{
	m_accio=a;
}

void CTemporitzador::antiAccio(t_accio * a)
{
	m_antiAccio=a;
}

void CTemporitzador::reAccio(t_accio * a)
{
	m_reAccio=a;
}

void CTemporitzador::operator()(void)
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
		else if (m_reAccio)
			(*m_reAccio)();
	}
	else if (m_antiAccio)
		(*m_antiAccio)();
	m_periodeActual--;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

static void Ko () {out << "K";}
static void Ok () {out << "O";}
static void No () {out << "-";}

void CTemporitzador::ProvaClasse()
{
	int exits=0;
	int intents=0;
	CTemporitzador timer;
	CAgent * ag1= new CAgentEnmascarador<void ()>(Ok);
	CAgent * ag2= new CAgentEnmascarador<void ()>(Ko);
	CAgent * ag3= new CAgentEnmascarador<void ()>(No);
	timer.cicleDesactiu(4,0,0);
	timer.cicleActiu(0,3,2);
	timer.probabilitat(10,9);
	timer.accio(ag1);
	timer.reAccio(ag2);
	timer.antiAccio(ag3);
	for (int i=400; i--;) {
		timer();
		if (timer.m_actiu) {
			intents++;
			if (timer.m_accionat)
				exits++;
		}
	}
	out << endl << "Estant actiu, s'han executat " << exits << " de " << intents << endl;
}

//////////////////////////////////////////////////////////////////////
// Ìmplementacio
//////////////////////////////////////////////////////////////////////

void CTemporitzador::recalculaActual()
{
	t_cicle & nouCicle = m_actiu?m_cicleActiu:m_cicleDesactiu;
	m_periodeActual=nouCicle.periode();
	if (m_actiu&&!m_periodeActual) 
	{
		m_actiu=!m_actiu;
		recalculaActual();
	}
}
