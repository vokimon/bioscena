// Substrat.cpp: implementation of the CSubstrat class.
//
//////////////////////////////////////////////////////////////////////

#include "Substrat.h"
#include "Color.h"
#include "Encaix.h"
#include "Configuracio.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSubstrat::CSubstrat()
{
	m_ocupat=false;
	m_limitMollecules=Config.get("Biotop/Substrat/MaximInicial");
	uint32 n=Config.get("Biotop/Substrat/MolleculesInicials/Numero");
	uint32 e=Config.get("Biotop/Substrat/MolleculesInicials/Element");
	uint32 t=Config.get("Biotop/Substrat/MolleculesInicials/Tolerancia");
	while (n--)
		deposita((rnd.get()&t)^e);
}

CSubstrat::~CSubstrat()
{

}

//////////////////////////////////////////////////////////////////////
// Operacions (Ocupacio)
//////////////////////////////////////////////////////////////////////

void CSubstrat::ocupa(uint32 idOrganisme)
{
	m_ocupat=true;
	m_ocupant=idOrganisme;
}

void CSubstrat::desocupa()
{
	m_ocupat=false;
}

bool CSubstrat::esOcupat()
{
	return m_ocupat;
}

uint32 CSubstrat::ocupant()
{
	return m_ocupant;
}

//////////////////////////////////////////////////////////////////////
// Operacions (Quimica)
//////////////////////////////////////////////////////////////////////

void CSubstrat::deposita(t_mol n)
{
	m_nutrients.push_front(n);
	if (m_nutrients.size()>m_limitMollecules)
		m_nutrients.pop_back();
}

uint32 CSubstrat::limitMollecules()
{
	return m_limitMollecules;
}

void CSubstrat::limitMollecules(uint32 n)
{
	m_limitMollecules=n;
	if (n<m_nutrients.size()) {
		list<t_mol>::iterator it;
		for (it=m_nutrients.begin(); n--; it++);
		m_nutrients.erase(it, m_nutrients.end());
	}
	// TODO: Optimizar el caso en que hay mas que se quedan que no que se van
}

uint32 CSubstrat::numeroMollecules()
{
	return m_nutrients.size();
}

bool CSubstrat::extreu(uint32 &n, uint32 clau, uint32 tolerancia)
{
	list<uint32>::iterator it;
	for (it=m_nutrients.begin(); it!=m_nutrients.end(); it++) {
		if (compat2(clau,*it,tolerancia)) {
			n=*it;
			m_nutrients.erase(it);
			return true;
		}
	}
	return false;
}

bool CSubstrat::rastreja(uint32 clau, uint32 tolerancia)
{
	list<uint32>::iterator it;
	for (it=m_nutrients.begin(); it!=m_nutrients.end(); it++) {
		if (compat2(clau,*it,tolerancia)) {
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CSubstrat::ProvaClasse () 
{
	CSubstrat s;
	s.dump(out);
	s.ocupa(4);
	s.dump(out);
	s.desocupa();
	s.dump(out);
	s.deposita(0x0003);
	s.deposita(0x0023);
	s.deposita(0x0043);
	s.deposita(0x0234);
	s.deposita(0x0234); 
	s.deposita(0xFFF1);
	s.dump(out);
	s.deposita(2); s.dump(out);
	uint32 estomac=0;
	for (int i=4; i--;) {
		uint32 n;
		if (s.extreu(n,0x0,0xFFFF00F0)) {
			out << "Hum, Menjar!! "<<n<<endl;
			estomac++;
			}
		}
	out << "M'he menjat "<< estomac << " coses!" << endl;
	s.dump(out);
	s.deposita(0x0003);
	s.deposita(0x0023);
	s.deposita(0x0043);
	s.deposita(0x0234);
	s.dump(out);
	out<< "Limitant a 5 mollecules" << endl;
	s.limitMollecules(5); s.dump(out);
	s.deposita(0x20); s.dump(out);
	s.deposita(0x21); s.dump(out);
	s.deposita(0x22); s.dump(out);
	s.deposita(0x23); s.dump(out);
	s.deposita(0x24); s.dump(out);
	s.deposita(0x25); s.dump(out);
	uint32 cercat=0x11111125;
	out << "Cercant quelcom semblant a 0x" << hex << cercat << dec 
		<< (s.rastreja(cercat,0x100100FF)?" Ok":" Ko") << endl;
}

//////////////////////////////////////////////////////////////////////
// Serialització
//////////////////////////////////////////////////////////////////////

ostream & operator<< (ostream & stream, CSubstrat s) 
{
	CColor colorFons(s.numeroMollecules());
	if (s.esOcupat()) {
		stream << CColor((s.ocupant()>>3)&07).brillant().fons(colorFons) << char('0'+(s.ocupant()&07)); 
	}
	else {
		stream << blanc.fons(colorFons);
		stream << " ";
	}
 
	return stream;
}

void CSubstrat::dump(CMissatger & stream)
{
	if (!esOcupat())
		stream << "Lliure; ";
	else
		stream << "Ocupant: "<<ocupant()<<"; ";
	stream << "Nutrients: ";
	list<uint32>::iterator it;
	for (it=m_nutrients.begin(); it!=m_nutrients.end();it++)
		stream <<"("<<*it<<")";
	if (it==m_nutrients.begin()) stream << "Erma";
	stream << endl;
}

