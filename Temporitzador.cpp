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
	m_cicleInactiu.m_periodeMinim=0;
	m_cicleInactiu.m_nombreDaus=0;
	m_cicleInactiu.m_magnitudDau=1;
	m_antiAccio = NULL;
	m_actiu = true;
	m_tipus+="/Temporitzador";
}

CTemporitzador::~CTemporitzador()
{
	if (m_antiAccio) delete m_antiAccio;
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

void CTemporitzador::cicleInactiu(uint32 periode, uint32 margeDau, uint32 nombreDaus)
{
	m_cicleInactiu.m_periodeMinim=periode;
	m_cicleInactiu.m_nombreDaus=nombreDaus;
	m_cicleInactiu.m_magnitudDau=margeDau;
	recalculaActual();
}

void CTemporitzador::antiAccio(t_accio * a)
{
	m_antiAccio=a;
}

list<CAgent*> CTemporitzador::subordinats() {
	list<CAgent*> l=CMultiAgent::subordinats();
	if (m_antiAccio) l.push_back(m_antiAccio);
	return l;
}

void CTemporitzador::modificaCicleActual(bool actiu, uint32 restantPeriode)
{
	m_actiu=actiu;
	m_periodeActual=restantPeriode;
}

void CTemporitzador::modificaCicleActual(uint32 restantPeriode)
{
	m_periodeActual=restantPeriode;
}

void CTemporitzador::dump(CMissatger & msg)
{
	CAgent::dump(msg);
	msg << "\tCicleActiu(" 
		<< m_cicleActiu.m_periodeMinim << ", "
		<< m_cicleActiu.m_nombreDaus <<   ", "
		<< m_cicleActiu.m_magnitudDau <<   ")" << endl;
	msg << "\tCicleInactiu(" 
		<< m_cicleInactiu.m_periodeMinim << ", "
		<< m_cicleInactiu.m_nombreDaus <<   ", "
		<< m_cicleInactiu.m_magnitudDau <<   ")" << endl;
	if (m_antiAccio) msg << "\tAntiAccio(" << m_antiAccio->nom() << ")" << endl; 
}

void CTemporitzador::operator()(void)
{
	if (!m_periodeActual) {
		// S'acabat el periode actual, canviem a l'altre
		m_actiu = !m_actiu;
		recalculaActual();
	}
	if (m_actiu) 
	{
		CMultiAgent::operator()();
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

void CTemporitzador::ProvaClasse()
{
	int exits=0;
	int intents=0;
	CTemporitzador timer;
	CAgent * ag1= new CFuncioAgent(Ok);
	CAgent * ag2= new CFuncioAgent(Ko);
	timer.cicleInactiu(4,0,0);
	timer.cicleActiu(0,3,2);
	timer.accio(ag1);
	timer.antiAccio(ag2);
	for (int i=400; i--;) {
		timer();
		intents++;
		if (timer.m_actiu) {
			exits++;
		}
	}
	out << endl << "Han estat actius " << exits << " cicles dels " << intents << " totals" << endl;
}

//////////////////////////////////////////////////////////////////////
// Ìmplementacio
//////////////////////////////////////////////////////////////////////

void CTemporitzador::recalculaActual()
{
	t_cicle & nouCicle = m_actiu?m_cicleActiu:m_cicleInactiu;
	m_periodeActual=nouCicle.periode();
	if (m_actiu&&!m_periodeActual) 
	{
		m_actiu=!m_actiu;
		recalculaActual();
	}
}

