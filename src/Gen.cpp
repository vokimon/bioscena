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
	// Comportament amb zona operadora
	m_ip = 0;
	while (m_ip<m_instruccions.size() && esOperadora())
		if (!condicioOperadora(fenotip))
			return false;
	return true;

	// Comportament sense zona operadora
	m_ip = 0; 
	return true;
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
	// TODO: Donar-li coherencia
}

bool CGen::esOperadora()
// Retorna cert si la instruccio actual pertany a la zona operadora
{
	// Versio amb zona operadora
	return m_instruccions[m_ip]&0x01;
	// Versio sense zona operadora
	return false;
}

bool CGen::condicioOperadora(uint32 * fenotip)
// PRE: fenotip apunta a una array de 32 elements
{
	uint32 instr = m_instruccions[m_ip];
	uint32 param1 = fenotip[ instr      & 0x00000017];
	uint32 param2 = fenotip[(instr>>=5) & 0x00000360];
	uint32 param3 = fenotip[(instr>>=5) & 0x00007400];
	uint32 param4 = fenotip[(instr>>=5) & 0x00170000];
	return (param1 ^ param2) && param2 && param4;
}
