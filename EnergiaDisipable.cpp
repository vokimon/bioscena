// EnergiaDisipable.cpp: implementation of the CEnergiaDisipable class.
//
//////////////////////////////////////////////////////////////////////

#include "EnergiaDisipable.h"
#include "Missatger.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnergiaDisipable::CEnergiaDisipable(uint32 slots)
{
	m_nSlots=slots;
	m_slots=new uint32[m_nSlots];
	for (int i=m_nSlots; i--;)
		m_slots[i]=0;
	m_slotActual=0;
	m_total=0;
}

CEnergiaDisipable::~CEnergiaDisipable()
{

}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

uint32 CEnergiaDisipable::consumeix(uint32 demandaEnergia)
{
	uint32 i=m_slotActual;
	while (m_total) 
	{
		if (!i) i=m_nSlots;
		i--;
		if (m_slots[i]>=demandaEnergia)
		{
			m_slots[i]-=demandaEnergia;
			m_total-=demandaEnergia;
			demandaEnergia=0;
		}
		else 
		{
			demandaEnergia-=m_slots[i];
			m_total-=m_slots[i];
			m_slots[i]=0;
		}
	}
	return demandaEnergia;
}

void CEnergiaDisipable::afegeix(uint32 quantitat)
{
	m_slots[m_slotActual]+=quantitat;
	m_total+=quantitat;
}

void CEnergiaDisipable::operator () (void)
{
	if (!m_slotActual) 
		m_slotActual=m_nSlots;
	m_slotActual--;
	m_total-=m_slots[m_slotActual];
	m_slots[m_slotActual]=0;
}

uint32 CEnergiaDisipable::total()
{
	return m_total;
}

CEnergiaDisipable::operator uint32()
{
	return m_total;
}
//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CEnergiaDisipable::ProvaClasse()
{
	CEnergiaDisipable e(4);
	out << e << endl;
	e.afegeix(23);
	out << e << endl;
	e.afegeix(43);
	out << e << endl;
	e();
	out << e << endl;
	e.afegeix(21);
	out << e << endl;
	e();
	out << e << endl;
	e.afegeix(25);
	out << e << endl;
	e.afegeix(11);
	out << e << endl;
	e();
	cout << e << endl;
	e.afegeix(41);
	cout << e << endl;
//	e();
	out << e << endl;
	e.afegeix(27);
	out << e << endl;
	out << "Encara tinc " << e.consumeix(100) << " unitats de gana" << endl;
	out << e << endl;
}

ostream& operator<< (ostream& stream, CEnergiaDisipable e)
{
	uint32 i;
	stream << '[';
	for (i=e.m_nSlots; ((stream << e.m_slots[--i]),1) && i; ) stream << '.';
	stream << "] = " << uint32(e);
	return stream;
}

