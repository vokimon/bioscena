// MutacioCromosomica.cpp: implementation of the CMutacioCromosomica class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990911 VoK - Creat a partir del que hi havia a CCromosoma
// 19990911 VoK - Afegits tipus i factories
// 19990911 VoK - Fix: Algunos bugs que havia en los operadores
// 19991209 VoK - Fix: Una insercio aleatoria no pot ser de len=0
// 19991209 VoK - Escisio: Geniques i Estructurals
// 19991209 VoK - Deriven de la factoria CMutacio
//////////////////////////////////////////////////////////////////////

#include <algorithm>
#include "MutacioCromosomica.h"
#include "RandomStream.h"
#include "Color.h"

using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Variables estatiques
//////////////////////////////////////////////////////////////////////

static bool traceMutacions=false;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMutacioCromosomica::CMutacioCromosomica()
{
}

CMutacioCromosomica::~CMutacioCromosomica()
{
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

void CMutacioCromosomica::muta(CCariotip & ca)
// Generalitzacio Cariotip -> Estructural
{
	uint32 cromosoma = ca.cromosomaAleatori();
//	ca[cromosoma]->dump(out);
	if (ca.tamany())
		muta(*(ca[cromosoma]));
//	ca[cromosoma]->dump(out);
}

void CMutacioDesplacament::muta(CCromosoma & c)
// Desplaca una sequencia aleatoria del cromosoma 
// (Granularitat Cromosoma) (Anellat) (Longitud Variable)
{
	agafaInformacio(c);
	if (m_nCodons<2) return; // Un desplacament no te sentit amb menys de 2 codons
	uint32 inici = rnd.get(0,m_nCodons-1);
	uint32 longitud = rnd.get(1,m_nCodons-1);
	uint32 desplacament = rnd.get(1,m_nCodons-longitud);
//  Nota de manteniment: Si es vol permetre un desplacament de m_nCodons cal revisar l'algorisme
	if (traceMutacions)
		out << tipus()
			<< " Codons:" << m_nCodons
			<< " Primer:" << inici
			<< " Longitud:" << longitud 
			<< " Desplacament:" << desplacament 
			<< endl;
	uint32 * tmp = new uint32[longitud];
	if (!tmp) {
		error << "CCromosoma: Error de memoria mutant per desplacament" << endl;
		cin.get();
		return;
	}
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
// (Granularitat Cromosoma) (Anellat) (Longitud Variable)
{
	uint32 nCodons=c.tamany();
	if (nCodons<=1) return; // Fugida discreta
	uint32 inici = rnd.get(0,nCodons-1);
	uint32 longitud = rnd.get(1,nCodons-1);
	if (traceMutacions)
		out << tipus()
			<< " Codons:" << nCodons
			<< " Primer:" << inici
			<< " Total a esborrar:" << longitud 
			<< endl;
	c.treuCodons(inici,longitud);
}

void CMutacioInsercioAleatoria::muta(CCromosoma & c)
// Insereix un nombre aleatori de gens aleatoris en 
// un punt aleatori
// (Granularitat Cromosoma) (Anellat) (Longitud Variable)
{
	agafaInformacio(c);
	if (!m_nCodons) return;
	uint32 iInsercio = rnd.get(0,m_nCodons-1);
	uint32 longitud = rnd.get(1,m_nCodons);
	uint32 *tmp= new uint32[m_nCodons+longitud];
	if (!tmp) {
		error << "CCromosoma: Error de memoria mutant per insercio aleatoria" << endl;
		cin.get();
	}
	if (traceMutacions)
		out << tipus()
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
// (Granularitat Cromosoma) (Anellat) (Longitud Variable)
{
	agafaInformacio(c);
	if (!m_nCodons) return;
	uint32 iInsercio = rnd.get(0,m_nCodons-1);
	uint32 iReplica = rnd.get(0,m_nCodons-1);
	uint32 longitud = rnd.get(1,m_nCodons);
	uint32 *tmp= new uint32[m_nCodons+longitud];
	if (!tmp) {
		error << "CCromosoma: Error de memoria, mutant per insercio aleatoria" << endl;
		cin.get();
	}
	if (traceMutacions)
		out << tipus()
			<< " Codons:" << m_nCodons
			<< " Inici Insercio:" << iInsercio
			<< " Inici Replica:" << iReplica
			<< " Longitud:" << longitud 
			<< endl;
	uint32 iDesti, iOrigen, cont;
	// Primer copiem la part d'abans de la insercio
	for (iDesti=iOrigen=0; iOrigen<iInsercio;)
		tmp[iDesti++]=m_codons[iOrigen++];
	// Despres fem la insercio replicada
	for (cont=longitud; cont--;)
	{
		tmp[iDesti++]=m_codons[iReplica++];
		if (iReplica==m_nCodons) iReplica=0;
	}
	// I acabem de copiar la part de despres
	while (iOrigen<m_nCodons)
		tmp[iDesti++]=m_codons[iOrigen++];
	delete [] m_codons;
	m_codons = tmp;
	m_nCodons=iDesti;
	fixaInformacio(c);
}


void CMutacioInversio::muta(CCromosoma & c)
// Inverteix l'ordre d'una subsequencia de gens
// (Granularitat Cromosoma) (Anellat) (Longitud Variable)
{
	agafaInformacio(c);
	if (!m_nCodons) return;
	uint32 inici = rnd.get(0,m_nCodons-1);
	uint32 longitud = rnd.get(1,m_nCodons);
	if (traceMutacions)
		out << tipus()
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

// Aquestes dos funcions membres son per permetre que les derivades de 
// CMutacioCromosomica puguin accedir als membres protegits de 
// CCromosoma sense que cada cop que afegim un nou operador de mutacio, 
// haguem de posar-ho com a friend.

void CMutacioCromosomica::agafaInformacio(CCromosoma &c)
// Es crida si cal consultar els codons del cromosoma
{
	m_nCodons=c.m_nCodons;
	m_codons=c.m_codons;
}

void CMutacioCromosomica::fixaInformacio(CCromosoma &c)
// Es crida si cal modificar els codons del cromosoma (principalment
// si cal substituir l'espai de memoria, modificacions del tipus
// m_codons[i]=x no ho requereixen)
{
	c.m_nCodons=m_nCodons;
	c.m_codons=m_codons;
}

//////////////////////////////////////////////////////////////////////
// Membres estatics
//////////////////////////////////////////////////////////////////////


uint32 CMutacioCromosomica::Nombre(void)
{
	return 5;
}

CMutacioCromosomica * CMutacioCromosomica::Nova(uint32 n)
{
	switch (n)
	{
	case 0: return new CMutacioDesplacament;
	case 1: return new CMutacioInversio;
	case 2: return new CMutacioDeleccio;
	case 3: return new CMutacioInsercioAleatoria;
	case 4: return new CMutacioInsercioReplicada;
	default: return NULL;
	}
}

CMutacioCromosomica * CMutacioCromosomica::Nova(string tipus)
{
	if (tipus==CMutacioDesplacament::Tipus()) return new CMutacioDesplacament;
	if (tipus==CMutacioInversio::Tipus()) return new CMutacioInversio;
	if (tipus==CMutacioDeleccio::Tipus()) return new CMutacioDeleccio;
	if (tipus==CMutacioInsercioAleatoria::Tipus())return new CMutacioInsercioAleatoria;
	if (tipus==CMutacioInsercioReplicada::Tipus()) return new CMutacioInsercioReplicada;
	else return NULL;
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

void CMutacioCromosomica::ProvaClasse(void)
{
	out << clrscr;
	out << blanc.brillant() << "Provant MutacioGenica" << blanc.fosc() << endl;
	out << "Entra un numero entre 1 i " << CMutacioCromosomica::Nombre() << " per veure una mutacio" << endl;
	out << "Entra un numero fora de l'interval per sortir" << endl;
	uint32 a;
	while (cin>>a && a && a<=CMutacioCromosomica::Nombre())
	{
		CCromosoma * crm5 = new CCromosoma;
		crm5->init(0, rnd.get(0,15));
		crm5->dump(out);
		CMutacioCromosomica * mutacio = CMutacioCromosomica::Nova(a-1);
		mutacio->muta(*crm5);
		out << mutacio->tipus() << endl; 
		crm5->dump(out);
		delete crm5;
		delete mutacio;
	}
}

