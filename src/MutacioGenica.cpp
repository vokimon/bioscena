// MutacioGenica.cpp: implementation of the CMutacioGenica class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990911 VoK - Creat a partir del que hi havia a CCromosoma
// 19990911 VoK - Afegits tipus i factories
// 19990911 VoK - Fix: Algunos bugs que havia en los operadores
//////////////////////////////////////////////////////////////////////

#include <algorithm>
#include "MutacioGenica.h"
#include "RandomStream.h"
#include "Color.h"

using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Variables estatiques
//////////////////////////////////////////////////////////////////////

static bool traceMutacions=true;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMutacioGenica::CMutacioGenica()
{
}

CMutacioGenica::~CMutacioGenica()
{
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

void CMutacioPuntualDrastica::muta(CCromosoma & c)
// Substitucio total d'un codo
// (Granularitat Codo)
{
	agafaInformacio(c);
	if (traceMutacions)
		out << "Mutacio Puntual de tot el codo: "
			<< endl;
	if (m_nCodons)
		m_codons[c.codoAleatori()]=rnd.get();
}

void CMutacioPuntualBinaria::muta(CCromosoma & c)
// Invertim nomes un Bit
// (Granularitat Bit)
{
	agafaInformacio(c);
	if (traceMutacions)
		out << "Mutacio Puntual de d'una base: "
			<< endl;
	if (m_nCodons)
		m_codons[c.codoAleatori()]^= 0x00000001<<rnd.get(0,31);
}

void CMutacioPuntualBinariaGaussiana::muta(CCromosoma & c)
// El numero d'uns invertits es una distribucio de gauss amb mitja 4=32bits*1/(2^3)
// (Granularitat codo)
{
	agafaInformacio(c);
	if (traceMutacions)
		out << "Mutacio Gaussiana de tot el codo: "
			<< endl;
	if (m_nCodons)
		m_codons[c.codoAleatori()]^=(rnd.get()&rnd.get()&rnd.get());
}

void CMutacioDesplacament::muta(CCromosoma & c)
// Desplaca una sequencia aleatoria del cromosoma 
// (Anellat) (Longitud Variable)
{
	agafaInformacio(c);
	if (!m_nCodons) return;
	uint32 inici = rnd.get(0,m_nCodons-1);
	uint32 longitud = rnd.get(1,m_nCodons);
	uint32 desplacament = rnd.get(1,m_nCodons-longitud+1);
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
}

void CMutacioDeleccio::muta(CCromosoma & c)
// Elimina una sequencia aleatoria del cromosoma
// (Anellat) (Longitud Variable)
{
	uint32 nCodons=c.tamany();
	if (nCodons<=1) return; // Fugida discreta
	uint32 inici = rnd.get(0,nCodons-1);
	uint32 longitud = rnd.get(1,nCodons-1);
	if (traceMutacions)
		out << "Mutacio per Deleccio: "
			<< "Codons:" << nCodons
			<< " Primer:" << inici
			<< " Total a esborrar:" << longitud 
			<< endl;
	c.treuCodons(inici,longitud);
}

void CMutacioInsercioAleatoria::muta(CCromosoma & c)
// Insereix un nombre aleatori de gens aleatoris en 
// un punt aleatori
// (Anellat) (Longitud Variable)
{
	agafaInformacio(c);
	if (!m_nCodons) return;
	uint32 iInsercio = rnd.get(0,m_nCodons-1);
	uint32 longitud = rnd.get(0,m_nCodons-1);
	uint32 *tmp= new uint32[m_nCodons+longitud];
	KKEPAssert (tmp, "CCromosoma: Error de memoria mutant per insercio aleatoria");
	if (traceMutacions)
		out << "Mutacio per Insercio aleatoria:" 
			<< " Codons:" << m_nCodons
			<< " Inici Insercio:" << iInsercio
			<< " Longitud:" << longitud 
			<< endl;
	uint32 iDesti, iOrigen, cont;
	for (iDesti=iOrigen=0; iOrigen<iInsercio;)
		tmp[iDesti++]=m_codons[iOrigen++];
	for (cont=longitud; cont--;)
		tmp[iDesti++]=rnd.get();	
	while (iOrigen<m_nCodons)
		tmp[iDesti++]=m_codons[iOrigen++];
	delete [] m_codons;
	m_codons = tmp;
	m_nCodons = iDesti;
	fixaInformacio(c);
}

void CMutacioInsercioReplicada::muta(CCromosoma & c)
// Insereix un nombre aleatori de gens replicats del
// mateix cromosoma en un punt aleatori
// (Anellat) (Longitud Variable)
{
	agafaInformacio(c);
	if (!m_nCodons) return;
	uint32 iInsercio = rnd.get(0,m_nCodons-1);
	uint32 iReplica = rnd.get(0,m_nCodons-1);
	uint32 longitud = rnd.get(1,m_nCodons);
	uint32 *tmp= new uint32[m_nCodons+longitud];

	KKEPAssert (tmp, "CCromosoma: Error de memoria, mutant per insercio aleatoria");
	if (traceMutacions)
		out << "Mutacio per Insercio replicada:" 
			<< " Codons:" << m_nCodons
			<< " Inici Insercio:" << iInsercio
			<< " Inici Replica:" << iReplica
			<< " Longitud:" << longitud 
			<< endl;
	uint32 iDesti, iOrigen, cont;
	for (iDesti=iOrigen=0; iOrigen<iInsercio;)
		tmp[iDesti++]=m_codons[iOrigen++];
	for (cont=longitud; cont--;)
	{
		tmp[iDesti++]=m_codons[iReplica++];
		if (iReplica==m_nCodons) iReplica=0;
	}
	while (iOrigen<m_nCodons)
		tmp[iDesti++]=m_codons[iOrigen++];
	delete [] m_codons;
	m_codons = tmp;
	m_nCodons=iDesti;
	fixaInformacio(c);
}


void CMutacioInversio::muta(CCromosoma & c)
// Inverteix l'ordre d'una subsequencia de gens
// (Anellat) (Longitud Variable)
{
	agafaInformacio(c);
	if (!m_nCodons) return;
	uint32 inici = rnd.get(0,m_nCodons-1);
	uint32 longitud = rnd.get(1,m_nCodons);
	if (traceMutacions)
		out << "Mutacio per Inversio:" 
			<< " Codons:" << m_nCodons
			<< " Primer:" << inici
			<< " Total a invertir:" << longitud 
			<< endl;
	uint32 idxInici, idxFinal, cont;
	idxInici=inici;
	idxFinal= (longitud>m_nCodons-inici)? // Si la zona invertida sobrepassa el final...
		longitud-(m_nCodons-inici)-1:     // El punt final es calcula
		inici+longitud-1;                 // Sino es calcula sumant-li la longitud al inici
	for (cont=longitud;cont>1;cont-=2)
	{
		swap(m_codons[idxInici],m_codons[idxFinal]);
		if (idxFinal) idxFinal--;
		else idxFinal=m_nCodons-1;
		idxInici++;
		if (idxInici==m_nCodons) idxInici=0;
	}
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////
string CMutacioPuntualBinaria::tipus()
{
	return CMutacioPuntualBinaria::Tipus();
}

string CMutacioPuntualBinariaGaussiana::tipus()
{
	return CMutacioPuntualBinariaGaussiana::Tipus();
}

string CMutacioPuntualDrastica::tipus()
{
	return CMutacioPuntualDrastica::Tipus();
}

string CMutacioDesplacament::tipus()
{
	return CMutacioDesplacament::Tipus();
}

string CMutacioInversio::tipus()
{
	return CMutacioInversio::Tipus();
}

string CMutacioDeleccio::tipus()
{
	return CMutacioDeleccio::Tipus();
}

string CMutacioInsercioAleatoria::tipus()
{
	return CMutacioInsercioAleatoria::Tipus();
}

string CMutacioInsercioReplicada::tipus()
{
	return CMutacioInsercioReplicada::Tipus();
}

//////////////////////////////////////////////////////////////////////
// Implementacio
//////////////////////////////////////////////////////////////////////

void CMutacioGenica::agafaInformacio(CCromosoma &c)
{
	m_nCodons=c.m_nCodons;
	m_codons=c.m_codons;
}

void CMutacioGenica::fixaInformacio(CCromosoma &c)
{
	c.m_nCodons=m_nCodons;
	c.m_codons=m_codons;
}

//////////////////////////////////////////////////////////////////////
// Membres estatics
//////////////////////////////////////////////////////////////////////

uint32 CMutacioGenica::Nombre(void)
{
	return 8;
}

CMutacioGenica * CMutacioGenica::Nova(uint32 n)
{
	switch (n)
	{
	case 0: return new CMutacioPuntualBinaria;
	case 1: return new CMutacioPuntualBinariaGaussiana;
	case 2: return new CMutacioPuntualDrastica;
	case 3: return new CMutacioDesplacament;
	case 4: return new CMutacioInversio;
	case 5: return new CMutacioDeleccio;
	case 6: return new CMutacioInsercioAleatoria;
	case 7: return new CMutacioInsercioReplicada;
	default: return NULL;
	}
}

CMutacioGenica * CMutacioGenica::Nova(string tipus)
{
	if (tipus==CMutacioPuntualBinaria::Tipus()) return new CMutacioPuntualBinaria;
	if (tipus==CMutacioPuntualBinariaGaussiana::Tipus()) return new CMutacioPuntualBinariaGaussiana;
	if (tipus==CMutacioPuntualDrastica::Tipus()) return new CMutacioPuntualDrastica;
	if (tipus==CMutacioDesplacament::Tipus()) return new CMutacioDesplacament;
	if (tipus==CMutacioInversio::Tipus()) return new CMutacioInversio;
	if (tipus==CMutacioDeleccio::Tipus()) return new CMutacioDeleccio;
	if (tipus==CMutacioInsercioAleatoria::Tipus())return new CMutacioInsercioAleatoria;
	if (tipus==CMutacioInsercioReplicada::Tipus()) return new CMutacioInsercioReplicada;
	else return NULL;
}


string CMutacioPuntualBinaria::Tipus()
{
	return "Mutacio/Puntual/Binaria";
}

string CMutacioPuntualBinariaGaussiana::Tipus()
{
	return "Mutacio/Puntual/Gaussiana";
}

string CMutacioPuntualDrastica::Tipus()
{
	return "Mutacio/Puntual/Total";
}

string CMutacioDesplacament::Tipus()
{
	return "Mutacio/Estructural/Desplacament";
}

string CMutacioInversio::Tipus()
{
	return "Mutacio/Estructural/Inversio";
}

string CMutacioDeleccio::Tipus()
{
	return "Mutacio/Estructural/Deleccio";
}

string CMutacioInsercioAleatoria::Tipus()
{
	return "Mutacio/Estructural/Insercio/Aleatoria";
}

string CMutacioInsercioReplicada::Tipus()
{
	return "Mutacio/Estructural/Insercio/Replicada";
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CMutacioGenica::ProvaClasse(void)
{
	out << clrscr;
	out << blanc.brillant() << "Provant MutacioGenica" << blanc.fosc() << endl;
	out << "Entra un numero entre 1 i " << CMutacioGenica::Nombre() << " per veure una mutacio" << endl;
	out << "Entra un numero fora de l'interval per sortir" << endl;
	uint32 a;
	while (cin>>a && a && a<=CMutacioGenica::Nombre())
	{
		CCromosoma * crm5 = new CCromosoma;
		crm5->init(0, rnd.get(0,15));
		crm5->dump(out);
		CMutacioGenica * mutacio = CMutacioGenica::Nova(a-1);
		mutacio->muta(*crm5);
		out << mutacio->tipus() << endl; 
		crm5->dump(out);
		delete crm5;
		delete mutacio;
	}
}

