// Substrat.cpp: implementation of the CSubstrat class.
//
//////////////////////////////////////////////////////////////////////

#include "Substrat.h"
#include "Color.h"
#include "Encaix.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSubstrat::CSubstrat()
{
	m_ocupat=false;
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

void CSubstrat::afegeixMollecula(t_mol n)
{
	m_nutrients.push_back(n);
}

void CSubstrat::limitaMollecules(uint32 n)
{
	if (n<m_nutrients.size()) {
		list<t_mol>::iterator it;
		for (it=m_nutrients.begin(); n--; it++);
		m_nutrients.erase(m_nutrients.begin(), it);
	}
	// TODO: Optimizar el caso en que hay mas que se quedan que no que se van
}

uint32 CSubstrat::numeroMollecules()
{
	return m_nutrients.size();
}

bool CSubstrat::agafaMollecula(uint32 &n, uint32 clau, uint32 tolerancia)
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

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CSubstrat::ProvaClasse () 
{
	CSubstrat s;
	out << s << endl;
	s.ocupa(4);
	out << s << endl;
	s.desocupa();
	out << s << endl;
	s.afegeixMollecula(3);
	s.afegeixMollecula(23);
	s.afegeixMollecula(43);
	s.afegeixMollecula(234);
	s.afegeixMollecula(234);
	s.afegeixMollecula(0xFFF1);
	out << s << endl;
	s.afegeixMollecula(2);
	out << s << endl;
	uint32 estomac=0;
	for (int i=100; i--;) {
		uint32 n;
		if (s.agafaMollecula(n,0x0,0x0)) {
			out << "Hum, Menjar!!"<<endl;
			estomac++;
			}
		}
	out << "M'he menjat "<< estomac << " coses!" << endl;
	out << s << endl;
	s.limitaMollecules(4);
	out << s << endl;
}

//////////////////////////////////////////////////////////////////////
// Serialització
//////////////////////////////////////////////////////////////////////

ostream & operator<< (ostream & stream, CSubstrat s) 
{
	char representacio;
	stream 
		<< CColor(1+s.numeroMollecules()) << "o";
/*	if (s.esOcupat()) {
		stream 
			<< blanc.fons(groc) << char('A'+(s.ocupant()&0x1F)); 
	}
	else 
		stream << blanc << "_";
*/
 /*
	if (!s.esOcupat())
		stream << "Lliure; ";
	else
		stream << "Ocupant: "<<s.ocupant()<<"; ";
	stream << "Nutrients: ";
	list<uint32>::iterator it;
	for (it=s.m_nutrients.begin(); it!=s.m_nutrients.end();it++)
		stream <<"("<<*it<<")";
	if (it==s.m_nutrients.begin()) stream << "Erma";
*/
	return stream;
}
