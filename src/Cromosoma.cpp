// Cromosoma.cpp: implementation of the CCromosoma class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990906 VoK - Creat. Fent servir trosos de l'antic CCromosoma del
//                prototip tot reestructurant sustanciament totes les 
//                funcions.
// 19990907 VoK - Traduit nova semantica Gen->Codo Bit->Base
// 19990907 VoK - Fixes: Limits dels rnd de mutacioDesplacament
// 19990911 VoK - Mutacions a fora!
// 19990917 VoK - Mutacions a dins pero deterministes (parteix, 
//                fusiona, treuCodons
//////////////////////////////////////////////////////////////////////
// Invariant: 
// - El contingut de codons pot ser valid o invalid
//   - Si es valid m_codons apunta a memoria valida i m_nCodons!=0
//   - Si es invalid m_codons i m_nCodons valen NULL i 0 respectivament
// - Si el contingut de codons ha sigut allocat per l'objecte, la
//   m_enSocPropietari val true i vol dir que es responsable del seu
//   alliberament.

#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Cromosoma.h"
#include "RandomStream.h"
#include "Color.h"

using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Variables estatiques
//////////////////////////////////////////////////////////////////////

static bool tracePrivate=false;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCromosoma::CCromosoma()
{
	initCodons();	
}

CCromosoma::~CCromosoma()
{
	alliberaCodons();
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

void CCromosoma::dump(CMissatger & msg)
{
	msg << hex << setfill('0');
	msg << "[";
	if (m_nCodons) 
		msg << setw(8) << m_codons[0];
	for (uint32 i=1; i<m_nCodons; i++)
		msg << "." << setw(8) << m_codons[i];
	msg << "]";
	msg << dec << setfill(' ');
	msg << endl;
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

bool CCromosoma::init(CCromosoma & c)
{
	if (!ocupaCodons(c.m_nCodons))
		return false;
	ompleCodonsCopiant(c.m_codons);
	return true;
}

bool CCromosoma::init(uint32 nCodons)
{
	if (!ocupaCodons(nCodons))
		return false;
	ompleCodonsAleatoriament();
	return true;
}

bool CCromosoma::init(uint32 primer, uint32 ultim)
{
	if (ultim<primer)
		return false;
	if (!ocupaCodons(ultim-primer+1))
		return false;
	ompleCodonsSequencialment(primer);
	return true;		
}

CCromosoma & CCromosoma::operator= (CCromosoma & c)
{
	alliberaCodons();
	init(c);
	return *this;
}

uint32& CCromosoma::operator[] (uint32 n)
// PRE: n<m_nCodons
{
	return m_codons[n];
}

uint32 CCromosoma::tamany(void) const
{
	return m_nCodons;
}

uint32 CCromosoma::codoAleatori(void) const
// PRE: Existeixen codons
{
	return rnd.get(0,m_nCodons-1);
}

void CCromosoma::fusiona(CCromosoma const & c)
{
	if (!c.tamany()) return;
	uint32 tamanyFinal = tamany() + c.tamany();
	uint32 * tmp = new uint32[tamanyFinal];
	copy(m_codons, m_codons+tamany(), tmp);
	copy(c.m_codons, c.m_codons+c.tamany(), tmp+tamany());
	alliberaCodons();
	copiaCodonsSuperficialment(tmp, tamanyFinal);
	m_enSocPropietari=true;	
}

void CCromosoma::parteix(CCromosoma & c, uint32 centromer)
{
	if (tamany()<2) return;
	if (!centromer) return;
	if (centromer>=tamany()) return;
	c.alliberaCodons();
	c.ocupaCodons(tamany()-centromer);
	c.ompleCodonsCopiant(m_codons+centromer);
	treuCodons(centromer, tamany()-centromer);
}


//////////////////////////////////////////////////////////////////////
// Implementacio
//////////////////////////////////////////////////////////////////////

void CCromosoma::initCodons(void)
{
	if (tracePrivate)
		out << "CCromosoma: Initant les variables relatives als codons" << endl;
	m_codons=NULL;
	m_nCodons=0;
	m_enSocPropietari=false;
}
	
bool CCromosoma::ocupaCodons(uint32 nCodons)
// PRE: m_codons=NULL, no hi hauria error pero es perdria la memoria
{
	if (tracePrivate)
		out << "CCromosoma: Ocupant espai per " << nCodons << " codons" << endl;
	if (!nCodons) 
		return false;
	uint32 * codons=new uint32[nCodons];
	if (!codons) 
		return false;
	m_codons=codons;
	m_nCodons=nCodons;
	m_enSocPropietari=true;
	return true;
}

void CCromosoma::copiaCodonsSuperficialment(uint32 * codons, uint32 nCodons)
// PRE: m_codons=NULL, no hi hauria error pero es perdria memoria
{
	if (tracePrivate)
		out << "CCromosoma: Referenciant a un altre cromosoma" << endl;
	m_codons=codons;
	m_nCodons=nCodons;
	m_enSocPropietari=false;
}

void CCromosoma::alliberaCodons(void)
{
	if (tracePrivate)
		out << "CCromosoma: Alliberant els codons" << endl;
	if (m_codons&&m_enSocPropietari)
		delete[] m_codons;
	initCodons();
}

void CCromosoma::ompleCodonsCopiant(uint32 * codons)
// PRE: sizeof(codons)>=m_nCodons
{
	uint32 nCodons=m_nCodons;
	while (nCodons--)
		m_codons[nCodons]=codons[nCodons];
}

void CCromosoma::ompleCodonsAleatoriament()
{
	uint32 nCodons=m_nCodons;
	while (nCodons--)
		rnd >> m_codons[nCodons];
}

void CCromosoma::ompleCodonsSequencialment(uint32 primer)
// PRE: MAXuint32-primer<m_nCodons
{
	uint32 nCodons=m_nCodons;
	while (nCodons--)
		m_codons[nCodons]=nCodons+primer;
}

void CCromosoma::treuCodons(uint32 primer, uint32 longitud)
{
	uint32 *tmp= new uint32[m_nCodons-longitud];
	if (!tmp) return; // Fugida discreta
	uint32 idxDesti=0;
	if (m_nCodons-primer<longitud) 
	{
		uint32 idxOrigen=longitud-(m_nCodons-primer);
		while (idxOrigen<primer)
			tmp[idxDesti++]=m_codons[idxOrigen++];
	}
	else 
	{
		uint32 idxOrigen=0;
		while (idxOrigen<primer)
			tmp[idxDesti++]=m_codons[idxOrigen++];
		idxOrigen+=longitud;
		while (idxOrigen<m_nCodons)
			tmp[idxDesti++]=m_codons[idxOrigen++];
	}
	delete [] m_codons;
	m_codons = tmp;
	m_nCodons-=longitud;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CCromosoma::ProvaClasse(void)
{
	out << clrscr;
	out << blanc.brillant() << "Provant Cromosomes" << blanc.fosc() << endl;
	CCromosoma c;
	c.dump(out);
	c.init(5);
	c.dump(out);
	// Creem cromosomes iguals a l'atzar
	CCromosoma crm0, crm1, crm2, crm3, crm4;
	crm0.init(3);
	crm1.init(3);
	crm2.init(crm1);
	crm0.fusiona(crm1);
	crm0.fusiona(crm0);
	out << "0: "; crm0.dump(out);
	out << "1: "; crm1.dump(out);
	out << "2: "; crm2.dump(out);
	out << "3: "; crm3.dump(out);
	out << "4: "; crm4.dump(out);
	cin.get();
}

/*
int main (void)
{
	CCromosoma::ProvaClasse();
	return 0;
}
*/

