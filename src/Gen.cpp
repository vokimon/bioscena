// Gen.cpp: implementation of the CGen class.
//
//////////////////////////////////////////////////////////////////////

#include "Gen.h"
#include "Color.h"
#include "RandomStream.h" // Nomes per proves

using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGen::CGen()
{

}

CGen::~CGen()
{

}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

bool CGen::init(CCromosoma & crm, uint32 & pos)
{
	while (pos<crm.tamany() && !esPromotor(crm[pos])) 
		pos++;
	if (pos<crm.tamany()) 
		return false;
	while (pos<crm.tamany()) {
		if (!esIntro(crm[pos]))
			m_instruccions.push_back (traduccio(crm[pos]));
		pos++;
	}
	return tamany();
}

uint32 CGen::tamany()
{
	return m_instruccions.size();
}

bool CGen::finalitzat()
// Diu si l'ip ha arribat al final
{
	return m_ip>=m_instruccions.size();
}

bool CGen::traduible(uint32 * fenotip)
// Si la zona d'operador es compleix reseteja l'ip i retorna true
{
	m_ip = 0;
	return true;
	// TODO: Cal executar l'operador i retornar true nomes si es compleix
	// TODO: Posar l'ip despres de l'operador o a fora si no es compleix
}

bool CGen::seguentInstruccio(t_instruccio & valor)
{
	valor = m_instruccions[m_ip];
	return true;
}

bool CGen::parametres(uint32 & valor)
{
	valor = rnd.get();
	return finalitzat();
	// TODO: Determinar la semantica del retorn
}

bool CGen::salt(uint32 patro)
{
	return finalitzat();
	// TODO: Determinar la semantica del retorn
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CGen::ProvaClasse(void)
{
	out << clrscr;
	out << blanc.brillant() << "Provant Gens" << blanc.fosc() << endl;
	CGen gen;
}

//////////////////////////////////////////////////////////////////////
// Implementacio
//////////////////////////////////////////////////////////////////////

bool CGen::esIntro(uint32 codo)
{
	return false;
	// TODO: Donar-li coherencia
}

bool CGen::esPromotor(uint32 codo)
{
	return true;
	// TODO: Donar-li coherencia
}

CGen::t_instruccio CGen::traduccio(uint32 codo)
{
	return codo;
}
