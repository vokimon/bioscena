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
// 20000526 VoK - MutacioPerInsercioAleatoria fa servir afegeixCodons
// 20000526 VoK - MutacioPerInsercioReplicada fa servir afegeixCodons
// 20000526 VoK - Es pot inserir al final del cromosoma
// 20000526 VoK - MutacioPerDesplacament no fa servir les 'm_'
// 20000526 VoK - Eliminades funcions agafarInfo i fixarInfo
// 20000527 VoK - Mutacions retornen si han anat be
//////////////////////////////////////////////////////////////////////

#include <algorithm>
#include "MutacioCromosomica.h"
#include "RandomStream.hxx"
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

bool CMutacioCromosomica::muta(CCariotip & ca)
// Generalitzacio Cariotip -> Estructural
{
	uint32 cromosoma = ca.cromosomaAleatori();
	if (!ca.tamany()) return false;
	return muta(*(ca[cromosoma]));
}

bool CMutacioDesplacament::muta(CCromosoma & c)
// Desplaca una sequencia aleatoria del cromosoma 
// (Granularitat Cromosoma) (Anellat) (Longitud Variable)
{
	uint32 nCodons=c.tamany();
	if (nCodons<2) return false; // Un desplacament no te sentit amb menys de 2 codons
	uint32 inici = rnd.get(0,nCodons-1);
	uint32 longitud = rnd.get(1,nCodons-1);
	uint32 desplacament = rnd.get(1,nCodons-longitud);
//  Nota de manteniment: Si es vol permetre un desplacament de m_nCodons cal revisar l'algorisme
	if (traceMutacions)
		out << tipus()
			<< " Codons:" << nCodons
			<< " Primer:" << inici
			<< " Longitud:" << longitud 
			<< " Desplacament:" << desplacament 
			<< endl;
	uint32 * tmp = new uint32[longitud];
	if (!tmp) {
		error << "CMutacioDesplacament: Error de memoria mutant" << endl;
		cin.get();
		return false;
	}
	uint32 idxDesti, idxOrigen, cont;
	// Primer copiem el tros desplacat
	idxDesti=0;
	idxOrigen=inici;
	for (cont=longitud; cont--;)
	{
		tmp[idxDesti++]=c[idxOrigen++];
		if (idxOrigen>=nCodons) idxOrigen = 0;
	}
	// Despres pasem el tros que queda fins el punt de desplacament
	idxDesti=inici;
	for (cont=desplacament; cont--;)
	{
		c[idxDesti++]=c[idxOrigen++];
		if (idxOrigen>=nCodons) idxOrigen = 0;
		else if (idxDesti>=nCodons) idxDesti = 0;
	}
	// I tornem a copiar el tros desplacat des del temporal
	idxOrigen=0;
	for (cont=longitud;cont--;)
	{
		c[idxDesti++]=tmp[idxOrigen++];
		if (idxDesti>=nCodons) idxDesti = 0;
	}
	delete[] tmp;
	return true;
}

bool CMutacioDeleccio::muta(CCromosoma & c)
// Elimina una sequencia aleatoria del cromosoma
// (Granularitat Cromosoma) (Anellat) (Longitud Variable)
{
	uint32 nCodons=c.tamany();
	if (nCodons<=1) return false; // Fugida discreta
	uint32 inici = rnd.get(0,nCodons-1);
	uint32 longitud = rnd.get(1,nCodons-1);
	if (traceMutacions)
		out << tipus()
			<< " Codons:" << nCodons
			<< " Primer:" << inici
			<< " Total a esborrar:" << longitud 
			<< endl;
	if (!c.treuCodons(inici,longitud)) {
		error << "CMutacioDeleccio: Error de memoria mutant" << endl;
		cin.get();
		return false;
		}
	return true;
}

bool CMutacioInsercioAleatoria::muta(CCromosoma & c)
// Insereix un nombre aleatori de gens aleatoris en 
// un punt aleatori
// (Granularitat Cromosoma) (Anellat) (Longitud Variable)
{
	uint32 nCodons = c.tamany();
	if (!nCodons) return false;
	uint32 iInsercio = rnd.get(0,nCodons);
	uint32 longitud = rnd.get(1,nCodons);
	if (!c.afegeixCodons(iInsercio,longitud)) {
		error << "CMutacioInsercioAleatoria: Error de memoria mutant" << endl;
		cin.get();
		return false;
		}
	if (traceMutacions)
		out << tipus()
			<< " Codons:" << nCodons
			<< " Inici Insercio:" << iInsercio
			<< " Longitud:" << longitud 
			<< endl;

	for (uint32 cont=longitud; cont--;)
		c[iInsercio++]=rnd.get();
	return true;
}

bool CMutacioInsercioReplicada::muta(CCromosoma & c)
// Insereix un nombre aleatori de gens replicats del
// mateix cromosoma en un punt aleatori
// (Granularitat Cromosoma) (Anellat) (Longitud Variable)
{
	uint32 nCodons = c.tamany();
	if (!nCodons) return false;
	uint32 iInsercio = rnd.get(0,nCodons-1);
	uint32 iReplica = rnd.get(0,nCodons-1);
	uint32 longitud = rnd.get(1,nCodons);
	if (!c.afegeixCodons(iInsercio,longitud)) {
		error << "CMutacioInsercioReplicada: Error de memoria mutant" << endl;
		cin.get();
		return false;
		}
	if (traceMutacions)
		out << tipus()
			<< " Codons:" << nCodons
			<< " Inici Insercio:" << iInsercio
			<< " Inici Replica:" << iReplica
			<< " Longitud:" << longitud 
			<< endl;
	// Despres fem la insercio replicada
	uint32 iDesti=iInsercio;
	nCodons+=longitud;
	if (iReplica>iInsercio) iReplica+=longitud;
	for (uint32 cont=longitud; cont--;)
	{
		if (iReplica==iInsercio) iReplica+=longitud;
		if (iReplica>=nCodons) iReplica-=nCodons;
		c[iDesti++]=c[iReplica++];
	}
	return true;
}


bool CMutacioInversio::muta(CCromosoma & c)
// Inverteix l'ordre d'una subsequencia de gens
// (Granularitat Cromosoma) (Anellat) (Longitud Variable)
{
	uint32 nCodons = c.tamany();
	if (!nCodons) return false;
	uint32 inici = rnd.get(0, nCodons-1);
	uint32 longitud = rnd.get(1, nCodons);
	if (traceMutacions)
		out << tipus()
			<< " Codons:" << nCodons
			<< " Primer:" << inici
			<< " Total a invertir:" << longitud 
			<< endl;
	uint32 idxInici, idxFinal, cont;
	idxInici=inici;
	idxFinal= (longitud>nCodons-inici)? // Si la zona invertida sobrepassa el final...
		longitud-(nCodons-inici)-1:     // El punt final es calcula
		inici+longitud-1;                 // Sino es calcula sumant-li la longitud al inici
	for (cont=longitud;cont>1;cont-=2)
	{
		swap(c[idxInici],c[idxFinal]);
		if (idxFinal) idxFinal--;
		else idxFinal=nCodons-1;
		idxInici++;
		if (idxInici==nCodons) idxInici=0;
	}
	return true;
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

