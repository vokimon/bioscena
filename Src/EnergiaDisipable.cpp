// EnergiaDisipable.cpp: implementation of the CEnergiaDisipable class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 199908?? VoK - Creat
// 19991126 VoK - Fix: No feiem delete dels slots d'energia
// 20000406 VoK - La disipacio ja no es un operador sino una funcio
//////////////////////////////////////////////////////////////////////

#include <iomanip>
#include "EnergiaDisipable.h"
#include "Missatger.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnergiaDisipable::CEnergiaDisipable(uint32 slots)
{
	m_nSlots=slots;
	m_slots=new uint32[m_nSlots];
	if (!m_slots) {
		error << "CEnergiaDisipable: Falta memoria per crear pila" << endl;
		cin.get();
	}
	for (int i=m_nSlots; i--;)
		m_slots[i]=0;
	m_slotActual=0;
	m_total=0;
}

CEnergiaDisipable::~CEnergiaDisipable()
{
	if (m_slots) delete [] m_slots;
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

void CEnergiaDisipable::dump(CMissatger & msg)
{
	msg << hex << setfill('0');
	msg << "[";
	if (m_nSlots) 
		msg << setw(8) << m_slots[0];
	for (uint32 i=1; i<m_nSlots; i++)
		msg << "." << setw(8) << m_slots[i];
	msg << "]";
	msg << dec << setfill(' ');
	msg << endl;
}

ostream & CEnergiaDisipable::store(ostream & str) {
	// TODO: Serializar el indicador de slot actual
	str.write((char*)&m_nSlots, sizeof(uint32));
	for (uint32 i=0; i<m_nSlots; i++)
		str.write((char*)&(m_slots[i]),sizeof(uint32));
	return str;
}

istream & CEnergiaDisipable::load(istream & str) {
	// TODO: Serializar el indicador de slot actual
	if (m_slots) delete [] m_slots;
	m_total=0;
	str.read((char*)&m_nSlots, sizeof(uint32));
	m_slots=new uint32[m_nSlots];
	for (uint32 i=0; i<m_nSlots; i++) {
		uint32 nouSlot;
		str.read((char*)&nouSlot,sizeof(uint32));
		if (m_slots) {
			m_slots[i]=nouSlot;
			m_total+=nouSlot;
		}
	}
	return str;
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

uint32 CEnergiaDisipable::consumeix(uint32 demandaEnergia)
{
	uint32 i=m_slotActual;
	while (m_total&&demandaEnergia) 
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

void CEnergiaDisipable::disipa (void)
{
	if (!m_slotActual) 
		m_slotActual=m_nSlots;
	m_slotActual--;
	m_total-=m_slots[m_slotActual];
	m_slots[m_slotActual]=0;
}

uint32 CEnergiaDisipable::total() const
{
	return m_total;
}

CEnergiaDisipable::operator uint32() const
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
	e.disipa();
	out << e << endl;
	e.afegeix(21);
	out << e << endl;
	e.disipa();
	out << e << endl;
	e.afegeix(25);
	out << e << endl;
	e.afegeix(11);
	out << e << endl;
	e.disipa();
	cout << e << endl;
	e.afegeix(41);
	cout << e << endl;
//	e.disipa();
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

