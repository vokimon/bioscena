// Genotip.cpp: implementation of the CGenotip class.
//
//////////////////////////////////////////////////////////////////////

#include "Genotip.h"
#include "RandomStream.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGenotip::CGenotip()
{

}

CGenotip::~CGenotip()
{

}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

uint32 CGenotip::seguentInstruccio(uint32 * fenotip)
{
	if (!m_gens.size())
		return rnd.get(0,32);
	if (m_gens[m_genActual].finalitzat()) {
		int i=5; // TODO: Convertir-ho en constant configurable
		do {
			m_genActual=rnd.get(1,m_gens.size())-1;
		}
		while (i--&& m_gens[m_genActual].traduible(fenotip));
	}

	if (m_gens[m_genActual].finalitzat())
		return rnd.get(0,32);
	uint32 instr;
	if (m_gens[m_genActual].seguentInstruccio(instr))
		return instr;
	return rnd.get(0,32);
}

bool CGenotip::init(CCariotip & car)
{
	for (uint32	crm=car.tamany(); crm--;) {
		if (!car[crm]) continue;
		for (uint32 pos=0; pos>=car[crm]->tamany();) {
			CGen gen;
			gen.init(*(car[crm]),pos);
			if (gen.tamany())
				m_gens.push_back(gen);
		}
	}
	return true;
}
