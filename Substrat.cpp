// Substrat.cpp: implementation of the CSubstrat class.
//
//////////////////////////////////////////////////////////////////////

#include "Substrat.h"
#include "Color.h"

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

void CSubstrat::addNutrient(CNutrient & n)
{
	m_nutrients.push_back(n);
}

bool CSubstrat::getNutrient(CNutrient&n, CEncaix<uint32> clau, uint32 tolerancia)
{
	list<CNutrient>::iterator it;
	for (it=m_nutrients.begin(); it!=m_nutrients.end(); it++) {
		if (clau.compatibleAmb(it->m_idonietat,tolerancia)) {
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
	CNutrient a(1,3);
	s.addNutrient(a);
	out << s << endl;
	CNutrient b(2,3);
	s.addNutrient(b);
	out << s << endl;
	for (int i=10; i--;) {
		CNutrient n;
		out << (s.getNutrient(n,0xFFFF,0xFFF3)?"True ":"False ")<<endl;
		CEncaix<uint32> lala(5), foo(5);
		out << "Dos iguals son "<< (lala.compatibleAmb(foo,~0)?"True ":"False ")<<endl;
	}
}

//////////////////////////////////////////////////////////////////////
// Serialització
//////////////////////////////////////////////////////////////////////

ostream & operator<< (ostream & stream, CSubstrat s) 
{
/*
	if (s.esOcupat())
		stream << CColor(s.ocupant()&0x07) << "#"; 
	else 
		stream << white.fosc() << "_";
*/
	if (!s.esOcupat())
		stream << "Lliure; ";
	else
		stream << "Ocupant: "<<s.ocupant()<<"; ";
	stream << "Nutrients: ";
	list<CNutrient>::iterator it;
	for (it=s.m_nutrients.begin(); it!=s.m_nutrients.end();it++)
		stream << it->m_idonietat <<"("<<it->m_quantitat<<");";
	if (it==s.m_nutrients.begin()) stream << "Erma";
	return stream;
}
