// Temporitzador.cpp: implementation of the CTemporitzador class.
//
//////////////////////////////////////////////////////////////////////

#include "Temporitzador.h"
#include "Color.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTemporitzador::CTemporitzador()
{
	m_tipus+="/Temporitzador";
	m_cicleActiu.m_periodeMinim=1;
	m_cicleActiu.m_nombreDaus=0;
	m_cicleActiu.m_magnitudDau=1;
	m_cicleInactiu.m_periodeMinim=0;
	m_cicleInactiu.m_nombreDaus=0;
	m_cicleInactiu.m_magnitudDau=1;
	m_antiAccio = NULL;
	m_actiu = true;
	recalculaActual();
}

CTemporitzador::~CTemporitzador()
{
	if (m_antiAccio) delete m_antiAccio;
}

//////////////////////////////////////////////////////////////////////
// Virtuals redefinibles a les subclasses
//////////////////////////////////////////////////////////////////////

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

void CTemporitzador::dump(CMissatger & msg)
{
	CMultiAgent::dump(msg);
	msg << "- CicleActiu: " 
		<< m_cicleActiu.m_periodeMinim << " "
		<< m_cicleActiu.m_nombreDaus << " "
		<< m_cicleActiu.m_magnitudDau << endl;
	msg << "- CicleInactiu: " 
		<< m_cicleInactiu.m_periodeMinim << " "
		<< m_cicleInactiu.m_nombreDaus << " "
		<< m_cicleInactiu.m_magnitudDau << endl;
	msg << "- CicleActual: " 
		<< (m_actiu? "Actiu":"Inactiu") << " "
		<< m_periodeActual << endl;
	if (m_antiAccio) msg << "- AntiAccio: " << m_antiAccio->nom() << endl; 
}

list<CAgent*> CTemporitzador::subordinats() {
	list<CAgent*> l=CMultiAgent::subordinats();
	if (m_antiAccio) l.push_back(m_antiAccio);
	return l;
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

void CTemporitzador::cicleActual(bool actiu, uint32 restantPeriode)
{
	m_actiu=actiu;
	m_periodeActual=restantPeriode;
}

void CTemporitzador::cicleActual(uint32 restantPeriode)
{
	m_periodeActual=restantPeriode;
}

void CTemporitzador::antiAccio(t_accio * a)
{
	m_antiAccio=a;
	if (a&&!a->subordinant(this))
		warning << "Subordinant l'agent '" << a->nom() << "' a un segon subordinant" << endl;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

static void Ko () {out << "-";}
static void Ok () {out << "O";}

void CTemporitzador::ProvaClasse()
{
	out << "\033[J";// Un clrscr xapuser pero standard (ANSI)
	out << blanc.brillant() << "Provant Agent Temporitzador" << blanc.fosc() << endl;
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
	timer.dumpAll(out);
	cin.get();
}

//////////////////////////////////////////////////////////////////////
// Ìmplementacio
//////////////////////////////////////////////////////////////////////

void CTemporitzador::recalculaActual()
// Restaura el periode actual (indicat per m_actiu) al valor de periode inicial
// Si es un cicle actiu i te un periode zero es torna a posar un cicle inactiu
{
	t_cicle & nouCicle = m_actiu?m_cicleActiu:m_cicleInactiu;
	m_periodeActual=nouCicle.periode();
	if (m_actiu&&!m_periodeActual) 
	{
		m_actiu=false;
		recalculaActual();
	}
}

