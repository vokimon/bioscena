// Cromosoma.cpp: implementation of the CCromosoma class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990906 VoK - Creat. Fent servir trosos de l'antic CCromosoma del
//                prototip tot reestructurant sustanciament totes les 
//                funcions.
// 19990907 VoK - Traduit nova semantica Gen->Codo Bit->Base
// 19990907 VoK - Fixes: Limits dels rnd de mutacioDesplacament
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
#include "Cromosoma.h"
#include "RandomStream.h"
#include "Color.h"

//////////////////////////////////////////////////////////////////////
// Variables estatiques
//////////////////////////////////////////////////////////////////////

static bool tracePrivate=false;
static bool traceMutacions=true;

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

uint32& CCromosoma::operator [ ](uint32 n)
// PRE: n<m_nCodons
{
	return m_codons[n];
}

uint32 CCromosoma::tamany(void)
{
	return m_nCodons;
}

uint32 CCromosoma::codoAleatori(void)
// PRE: Existeixen codons
{
	return rnd.get(0,m_nCodons-1);
}

//////////////////////////////////////////////////////////////////////
// Operadors Unaris (Mutacions)
//////////////////////////////////////////////////////////////////////

CCromosoma& CCromosoma::mutacioPuntualDrastica(void)
// Substitucio total d'un codo
// (Granularitat Codo)
{
	if (traceMutacions)
		out << "Mutacio Puntual de tot el codo: "
			<< endl;
	if (m_nCodons)
		m_codons[codoAleatori()]=rnd.get();
	return *this;
}

CCromosoma& CCromosoma::mutacioPuntualBinaria(void)
// Invertim nomes un Bit
// (Granularitat Bit)
{
	if (traceMutacions)
		out << "Mutacio Puntual de d'una base: "
			<< endl;
	if (m_nCodons)
		m_codons[codoAleatori()]^= 0x00000001<<rnd.get(0,31);
	return *this;
}

CCromosoma& CCromosoma::mutacioPuntualBinariaGaussiana(void)
// El numero d'uns invertits es una distribucio de gauss
// (Granularitat codo)
{
	if (traceMutacions)
		out << "Mutacio Gaussiana de tot el codo: "
			<< endl;
	if (m_nCodons)
		m_codons[codoAleatori()]^=(rnd.get()&rnd.get()&rnd.get());
	return *this;
}

CCromosoma& CCromosoma::mutacioDesplacament(void)
// Desplaca una sequencia aleatoria del cromosoma 
// (Anellat) (Longitud Variable)
{
	if (m_nCodons<=1) return *this;
	uint32 inici = rnd.get(0,m_nCodons-1);
	uint32 longitud = rnd.get(1,m_nCodons-1);
	uint32 desplacament = rnd.get(0,m_nCodons-longitud-1);
	if (traceMutacions)
		out << "Mutacio per desplacament: "
			<< "Codons:" << m_nCodons
			<< " Primer:" << inici
			<< " Total a moure:" << longitud 
			<< " Posicions a moure:" << desplacament 
			<< endl;

	uint32 * tmp = new uint32[longitud];

	KKEPAssert (tmp, "CCromosoma: Error de memoria mutant per desplacament");
	//if (!tmp) return *this; // Sortida rapida si hi ha falta de memoria
	uint32 idxDesti, idxOrigen, cont;
	// Primer copiem el tros desplacat
	idxDesti=0;
	idxOrigen=inici;
	for (cont=longitud; cont--;)
	{
		tmp[idxDesti++]=m_codons[idxOrigen++];
		if (idxOrigen>=m_nCodons) idxOrigen = 0;
	}
	// Despres pasem el tros que queda fins el punt de desplacament
	idxDesti=inici;
	for (cont=desplacament; cont--;)
	{
		m_codons[idxDesti++]=m_codons[idxOrigen++];
		if (idxOrigen>=m_nCodons) idxOrigen = 0;
		else if (idxDesti>=m_nCodons) idxDesti = 0;
	}
	// I tornem a copiar el tros desplacat des del temporal
	idxOrigen=0;
	for (cont=longitud;cont--;)
	{
		m_codons[idxDesti++]=tmp[idxOrigen++];
		if (idxDesti>=m_nCodons) idxDesti = 0;
	}
	delete[] tmp;
	return *this;
}

CCromosoma& CCromosoma::mutacioDeleccio()
// Elimina una sequencia aleatoria del cromosoma
// (Anellat) (Longitud Variable)
{
	if (m_nCodons<1) return *this;
	uint32 inici = rnd.get(0,m_nCodons-1);
	uint32 longitud = rnd.get(0,m_nCodons-1);
	uint32 *tmp= new uint32[m_nCodons-longitud];
	if (traceMutacions)
		out << "Mutacio per Deleccio: "
			<< "Codons:" << m_nCodons
			<< " Primer:" << inici
			<< " Total a esborrar:" << longitud 
			<< endl;
	
	KKEPAssert (tmp, "CCromosoma: Error de memoria mutant per deleccio");

	uint32 idxDesti, idxOrigen;
	idxDesti=0;
	if (m_nCodons-inici<longitud) 
	{
		idxOrigen=longitud-(m_nCodons-inici);
		while (idxOrigen<inici)
			tmp[idxDesti++]=m_codons[idxOrigen++];
	}
	else 
	{
		idxOrigen=0;
		while (idxOrigen<inici)
			tmp[idxDesti++]=m_codons[idxOrigen++];
		idxOrigen+=longitud;
		while (idxOrigen<m_nCodons)
			tmp[idxDesti++]=m_codons[idxOrigen++];
	}
	delete [] m_codons;
	m_codons = tmp;
	m_nCodons-=longitud;
	return *this;
}

CCromosoma & CCromosoma::mutacioInsercioAleatoria()
// Insereix un nombre aleatori de gens aleatoris en 
// un punt aleatori
// (Anellat) (Longitud Variable)
{
	if (!m_nCodons) return *this;
	uint32 inici = rnd.get(0,m_nCodons-1);
	uint32 longitud = rnd.get(0,m_nCodons-1);
	uint32 *tmp= new uint32[m_nCodons+longitud];
	KKEPAssert (tmp, "CCromosoma: Error de memoria mutant per insercio aleatoria");
	if (traceMutacions)
		out << "Mutacio per Insercio aleatoria: " 
			<< "Codons:" << m_nCodons
			<< " Primer:" << inici
			<< " Total a inserir:" << longitud 
			<< endl;
	uint32 idxDesti, idxOrigen, cont;
	for (idxDesti=idxOrigen=0; idxOrigen<inici;)
		tmp[idxDesti++]=m_codons[idxOrigen++];
	for (cont=longitud; cont--;)
		tmp[idxDesti++]=rnd.get();	
	while (idxOrigen<m_nCodons)
		tmp[idxDesti++]=m_codons[idxOrigen++];
	delete [] m_codons;
	m_codons = tmp;
	m_nCodons+=longitud;
	return *this;
}

CCromosoma& CCromosoma::mutacioInsercioReplicada()
// Insereix un nombre aleatori de gens replicats del
// mateix cromosoma en un punt aleatori
// (Anellat) (Longitud Variable)
{
	if (!m_nCodons) return *this;
	uint32 inici = rnd.get(0,m_nCodons-1);
	uint32 idxReplica = rnd.get(0,m_nCodons-1);
	uint32 longitud = rnd.get(0,m_nCodons-1);
	uint32 *tmp= new uint32[m_nCodons+longitud];

	KKEPAssert (tmp, "CCromosoma: Error de memoria mutant per insercio aleatoria");
	if (traceMutacions)
		out << "Mutacio per Insercio replicada: " 
			<< "Codons:" << m_nCodons
			<< " Primer:" << inici
			<< " Primer Replicat:" << idxReplica
			<< " Total a inserir:" << longitud 
			<< endl;
	uint32 idxDesti, idxOrigen, cont;
	for (idxDesti=idxOrigen=0; idxOrigen<inici;)
		tmp[idxDesti++]=m_codons[idxOrigen++];
	for (cont=longitud; cont--;)
	{
		tmp[idxDesti++]=m_codons[idxReplica++];
		if (idxReplica==m_nCodons) idxReplica=0;
	}
	while (idxOrigen<m_nCodons)
		tmp[idxDesti++]=m_codons[idxOrigen++];
	delete [] m_codons;
	m_codons = tmp;
	m_nCodons+=longitud;
	return *this;
}


CCromosoma& CCromosoma::mutacioInversio()
// Inverteix l'ordre d'una subsequencia de gens
// (Anellat) (Longitud Variable)
{
	if (!m_nCodons) return *this;
	uint32 inici = rnd.get(0,m_nCodons-1);
	uint32 longitud = rnd.get(0,m_nCodons-1);
	if (traceMutacions)
		out << "Mutacio per Inversio: " 
			<< "Codons:" << m_nCodons
			<< " Primer:" << inici
			<< " Total a invertir:" << longitud 
			<< endl;
	if (!longitud) return *this;
	uint32 idxInici, idxFinal, cont;
	idxInici=inici;
	if (longitud>m_nCodons-inici) idxFinal=longitud-(m_nCodons-inici)-1;
	else idxFinal=inici+longitud-1;
	for (cont=longitud;cont>1;cont-=2)
	{
		uint32 tmp=m_codons[idxInici];
		m_codons[idxInici]=m_codons[idxFinal];
		m_codons[idxFinal]=tmp;
		if (idxFinal) idxFinal--;
		else idxFinal=m_nCodons-1;
		idxInici++;
		if (idxInici==m_nCodons) idxInici=0;
	}
	return *this;
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
// PRE: MAXuin32-primer<m_nCodons
{
	uint32 nCodons=m_nCodons;
	while (nCodons--)
		m_codons[nCodons]=nCodons+primer;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CCromosoma::ProvaClasse(void)
{
	out << "\033[J";// Un clrscr xapuser pero standard (ANSI)
	out << blanc.brillant() << "Provant Cromosomes" << blanc.fosc() << endl;
	CCromosoma c;
	c.dump(out);
	c.init(5);
	c.dump(out);
	// Creem cromosomes iguals a l'atzar
	CCromosoma crm0, crm1, crm2, crm3, crm4;
	crm0.init(8);
	crm1.init(8);
	crm2.init(crm1);
	out << "0: "; crm0.dump(out);
	out << "1: "; crm1.dump(out);
	out << "2: "; crm2.dump(out);
	out << "3: "; crm3.dump(out);
	out << "4: "; crm4.dump(out);
	out << "Entra q per acabar o return per veure una mutacio" << endl;
	// TODO: Provar d'una vegada que tots els operators funcionen!!!
	typedef CCromosoma & (CCromosoma::*memberFunc) (void);
	memberFunc f[8] = {
		&mutacioPuntualBinariaGaussiana,
		&mutacioPuntualBinaria,
		&mutacioPuntualDrastica,
		&mutacioDesplacament,
		&mutacioInversio,
		&mutacioInsercioReplicada,
		&mutacioInsercioAleatoria,
		&mutacioDeleccio
	};

//	while (cin.get()!='q') 
	uint32 a;
	while (cin>>a && a && a<sizeof(f))
	{
		CCromosoma * crm5 = new CCromosoma;
		crm5->init(0, rnd.get(0,14));
		crm5->dump(out);
		(crm5->*f[a-1])();
		crm5->dump(out);
		delete crm5;
	}
}

/*
int main (void)
{
	CCromosoma::ProvaClasse();
	return 0;
}
*/
