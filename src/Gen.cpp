// Gen.cpp: implementation of the CGen class.
//
//////////////////////////////////////////////////////////////////////

#include <iomanip>
#include "Gen.h"
#include "Color.h"
#include "RandomStream.h" // Nomes per proves
#include "Configuracio.h"

using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGen::CGen()
{
	m_ip=1; // m_instruccions.size();
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
	if (pos>=crm.tamany()) 
		return false;
	while (pos<crm.tamany() && !esTerminador(crm[pos])) {
		if (!esIntro(crm[pos]))
			m_instruccions.push_back (traduccio(crm[pos]));
		pos++;
	}
	m_ip=tamany();
	return m_ip!=0;
}

uint32 CGen::tamany()
{
	return m_instruccions.size();
}

bool CGen::finalitzat()
// Diu si l'ip ha arribat al final
{
	return !(m_ip<m_instruccions.size());
}

bool CGen::traduible(uint32 * fenotip)
// Si la zona d'operador es compleix reseteja l'ip i retorna true
{
	// Comportament amb zona operadora
	m_ip = 0;
	while (m_ip<m_instruccions.size() && esOperadora())	{
		if (!condicioOperadora(fenotip))
			return false;
		m_ip++;
	}
	return m_ip<m_instruccions.size();

	// Comportament sense zona operadora
	m_ip = 0; 
	return true;
}

bool CGen::seguentInstruccio(t_instruccio & valor)
{
	valor = m_instruccions[m_ip++];
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
	// TODO: Donar-li sentit
	return (codo&Config.get("Organisme/Genotip/Intro/Mascara"))!=0;
	return false; // La versio simple
}

bool CGen::esPromotor(uint32 codo)
{
	// Ha de ser molt probable
	// TODO: Donar-li sentit
	return (codo&Config.get("Organisme/Genotip/Promotor/Mascara"))!=0;
	return true; // La versio simple (Qualsevol es Promotor)
}

bool CGen::esTerminador(uint32 codo)
{
	// Ha de ser poc probable
	// TODO: Donar-li sentit
	return ~(codo|Config.get("Organisme/Genotip/Terminador/Mascara"))==0;
	return false; // La versio simple (Sense Terminador)
}

CGen::t_instruccio CGen::traduccio(uint32 codo)
{
	// TODO: Donar-li sentit
	return codo; // La versio simple (Sense traduccio)
}

bool CGen::esOperadora()
// Retorna cert si la instruccio actual pertany a la zona operadora
{
	return (m_instruccions[m_ip]&Config.get("Organisme/Genotip/ZonaOperadora/Mascara"))!=0;
	return false; // La versio simple (Sense zona operadora)
}

bool CGen::condicioOperadora(uint32 * fenotip)
// PRE: fenotip apunta a una array de 32 elements
{
	uint32 instr = m_instruccions[m_ip];
	uint32 param1 = fenotip[(instr&0x0000000F)>> 0];
	uint32 param2 = fenotip[(instr&0x000000F0)>> 4];
	uint32 param3 = fenotip[(instr&0x00000F00)>> 8];
	uint32 param4 = fenotip[(instr&0x0000F000)>>12];
	return (param1 ^ param2) && param2 && param4;
}



void CGen::dump(CMissatger & msg)
{
	uint32 oldIp = m_ip;
	m_ip=0;
	bool colorOperadora = true;
	msg << '[' << vermell;
	for (t_instruccions::iterator it = m_instruccions.begin(); it != m_instruccions.end(); it++)
	{
		if (colorOperadora && !esOperadora()) {
			msg << blanc.fosc();
			colorOperadora = false;
		}
		msg << hex << setw(8) << *it << dec << '.';
		m_ip++;
	}
	if (colorOperadora)
		msg << blanc.fosc();
	msg << ']' << endl;
	m_ip = oldIp;
}
