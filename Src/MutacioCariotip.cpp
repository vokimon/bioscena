// MutacioCariotip.cpp: implementation of the CMutacioCariotip class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990914 VoK - Creat a imatge i semblant de CMutacioGenica
// 19991126 VoK - Fix: No esborraven el material genetic residual
//                a les Mutacions per Fusio
// 19991214 VoK - Afegida insercio d'un cromosoma aleatori
// 20000111 VoK - Afegits prototips de translocacio i trans. reciproca
// 20000526 VoK - Implementada translocacio simple i activada
// 20000527 VoK - Fix: No es podia translocar cap al final d'un cromosoma
// 20000527 VoK - Mutacions retornen si han anat be
//////////////////////////////////////////////////////////////////////
// TODO: Implementar les mutacions per translocacio i activar-les perque puguin ser usades.

#include <algorithm>
#include "MutacioCariotip.h"
#include "RandomStream.h"
#include "Color.h"
#include "Configuracio.h"

using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Variables estatiques
//////////////////////////////////////////////////////////////////////

static bool traceMutacions=false;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMutacioCariotip::CMutacioCariotip()
{
}

CMutacioCariotip::~CMutacioCariotip()
{
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

bool CMutacioPerFusio::muta(CCariotip & car)
// Fusio de dos cromosomes d'un cariotip
// (Granularitat Cariotip)
{
	if (!car.tamany()) return false; // Fugida discreta
	uint32 idxOrigen=car.cromosomaAleatori();
	uint32 idxDesti=car.cromosomaAleatori();
	if (traceMutacions)
		out << tipus()
			<< " Cromosoma Origen: " << idxOrigen << "(" << car[idxOrigen]->tamany() << ")"
			<< " Cromosoma Desti: " << idxDesti << "(" << car[idxDesti]->tamany() << ")"
			<< endl;
	CCariotip::t_cromosoma crm1 = car[idxOrigen];
	CCariotip::t_cromosoma crm2 = car[idxDesti];
	crm2->fusiona(*crm1);
	if (idxDesti!=idxOrigen) {
		crm1 = car.extreu(idxOrigen);
		if (crm1) delete crm1;
	}
	return true;
}

bool CMutacioPerEscisio::muta(CCariotip & car)
// Escisio d'un cromosoma del cariotip en dos cromosomes
// (Granularitat Cariotip)
{
	if (!car.tamany()) return false; // Fugida discreta
	uint32 idxOrigen=car.cromosomaAleatori();
	uint32 idxDesti=rnd.get(0,car.tamany());

	CCariotip::t_cromosoma crm1 = car[idxOrigen];
	if (crm1->tamany()<2) return false; // Fugida discreta

	uint32 idxCentromer = rnd.get(1,crm1->tamany()-1);

	if (traceMutacions)
		out << tipus() 
			<< " Escindit: " << idxOrigen << "(" << car[idxOrigen]->tamany() << ")"
			<< " Desti: " << idxDesti 
			<< " Centromer: " << idxCentromer
		    << endl;
	CCariotip::t_cromosoma crm2 = new CCromosoma;
	if (!crm2) {
		error << "Mutant per Escisio: Error de memoria" << endl;
		cin.get();
		return false;
	}
	crm1->parteix(*crm2,idxCentromer);
	// TODO: Controlar memoria
	car.afegeix(crm2,idxDesti);
	return true;
}

bool CMutacioPerTranslocacio::muta(CCariotip & car)
// Migració d'un fragment cromosomic d'un cromosoma a un altre
// (Granularitat Cariotip)
{
	if (!car.tamany()) return false; // Fugida discreta
	uint32 idxOrigen=car.cromosomaAleatori();
	uint32 idxDesti=car.cromosomaAleatori();
	if (idxOrigen==idxDesti) return false; // Fugida discreta

	CCariotip::t_cromosoma crm1 = car[idxOrigen];
	CCariotip::t_cromosoma crm2 = car[idxDesti];
	uint32 nCodonsOrig = crm1->tamany();
	uint32 nCodonsDesti = crm2->tamany();
	if (nCodonsOrig<=1) return false; // Fugida discreta

	uint32 iniciOrig = rnd.get(0,nCodonsOrig-1);
	uint32 longitud = rnd.get(1,nCodonsOrig-1);
	uint32 iniciDesti = rnd.get(0,nCodonsDesti);

	if (!crm2->afegeixCodons(iniciDesti,longitud)) {
		error << "CMutacioPerTranslocacio: error de memoria mutant" << endl;
		cin.get();
		return false;
		}
	if (traceMutacions)
		out << tipus()
			<< " Origen: " << idxOrigen << '(' << nCodonsOrig << ')'
			<< " Inici:" << iniciOrig
			<< " Desti:" << idxDesti << '(' << nCodonsDesti << ')'
			<< " Inici:" << iniciDesti
			<< " Longitud:" << longitud 
			<< endl;
	uint32 i1 = iniciOrig;
	uint32 i2 = iniciDesti;
	for (uint32 cont=longitud; cont--;) {
		(*crm2)[i2++]=(*crm1)[i1++];
		if (i1>=nCodonsOrig) i1=0;
		}
	if (!crm1->treuCodons(iniciOrig,longitud)) {
		error << "CMutacioPerTranslocacio: error de memoria mutant" << endl;
		cin.get();
		return false;
		}
	return true;
}

bool CMutacioPerTranslocacioReciproca::muta(CCariotip & car)
// Intercanvi mutu de dos fragments entre dos cromosomes
// (Granularitat Cariotip)
{
	// TODO: Mutacio per Translocacio Reciproca
	if (!car.tamany()) return false; // Fugida discreta
	return false;
}

bool CAneuploidiaPositiva::muta(CCariotip & car)
// Duplicacio total d'un cromosoma del cariotip
// (Granularitat Cariotip)
{
	uint32 cromosomaDuplicat = car.cromosomaAleatori();
	uint32 posicioFinal = rnd.get(0,car.tamany());
	if (traceMutacions)
		out << tipus()
			<< " Duplicat: " << cromosomaDuplicat << "(" << car[cromosomaDuplicat]->tamany() << ")"
			<< " Posicio: " << posicioFinal 
			<< endl;
	if (!car.tamany())
		return false;
	CCariotip::t_cromosoma crm1 = car[cromosomaDuplicat];
	CCariotip::t_cromosoma crm2 = new CCromosoma;
	if (!crm2) {
		error << "Mutant AneuploidiaPositiva: Error de memoria" << endl;
		cin.get();
		return false;
	}

	crm2->init(*crm1);
	car.afegeix(crm2,posicioFinal);
	return true;
}

bool CAneuploidiaAleatoria::muta(CCariotip & car)
// Duplicacio total d'un cromosoma del cariotip
// (Granularitat Cariotip)
{
	uint32 posicioFinal = rnd.get(0,car.tamany());
	if (traceMutacions)
		out << tipus()
			<< " Posicio: " << posicioFinal 
			<< endl;
	if (!car.tamany())
		return false;
	CCariotip::t_cromosoma crm2 = new CCromosoma;
	if (!crm2) {
		error << "Mutant AneuploidiaPositiva: Error de memoria" << endl;
		cin.get();
		return false;
	}

	crm2->init(rnd.get(
		Config.get("Organisme/Cariotip/LongitudMinima"),
		Config.get("Organisme/Cariotip/LongitudMaxima")));
	car.afegeix(crm2,posicioFinal);
	return true;
}

bool CAneuploidiaNegativa::muta(CCariotip & car)
// Duplicacio total d'un cromosoma del cariotip
// (Granularitat Cromosoma)
{
	if (car.tamany()<2) return false; // Fugida Discreta
	uint32 cromosomaEliminat = car.cromosomaAleatori();
	if (traceMutacions)
		out << tipus()
			<< " Eliminat: " << cromosomaEliminat << "(" << car[cromosomaEliminat]->tamany() << ")"
			<< endl;
	CCariotip::t_cromosoma crm1 = car.extreu(cromosomaEliminat);
	if (crm1) delete crm1;
	return true;
}

bool CEuploidiaPositiva::muta(CCariotip & car)
// Duplicacio total del cariotip
// (Granularitat Cariotip)
{
	if (traceMutacions)
		out << tipus()
			<< endl;
	if (!car.tamany()) return false;
	uint32 ultim=car.tamany()-1;
	uint32 restants=car.tamany();
	while(restants--) {
		CCariotip::t_cromosoma crm1 = car[ultim];
		CCariotip::t_cromosoma crm2 = new CCromosoma;
		if (!crm2) {
			error << "Mutant EuploidiaPositiva: Error de memoria" << endl;
			cin.get();
			return false;
		}
		crm2->init(*crm1);
		car.afegeix(crm2,0);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

string CMutacioPerFusio::tipus()
{
	return CMutacioPerFusio::Tipus();
}

string CMutacioPerEscisio::tipus()
{
	return CMutacioPerEscisio::Tipus();
}

string CMutacioPerTranslocacio::tipus()
{
	return CMutacioPerTranslocacio::Tipus();
}

string CMutacioPerTranslocacioReciproca::tipus()
{
	return CMutacioPerTranslocacioReciproca::Tipus();
}

string CAneuploidiaPositiva::tipus()
{
	return CAneuploidiaPositiva::Tipus();
}

string CAneuploidiaNegativa::tipus()
{
	return CAneuploidiaNegativa::Tipus();
}

string CAneuploidiaAleatoria::tipus()
{
	return CAneuploidiaAleatoria::Tipus();
}

string CEuploidiaPositiva::tipus()
{
	return CEuploidiaPositiva::Tipus();
}

//////////////////////////////////////////////////////////////////////
// Implementacio
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Membres estatics
//////////////////////////////////////////////////////////////////////

uint32 CMutacioCariotip::Nombre(void)
{
	return 6;
}

CMutacioCariotip * CMutacioCariotip::Nova(uint32 n)
{
	switch (n)
	{
	case 0: return new CMutacioPerFusio;
	case 1: return new CMutacioPerEscisio;
	case 2: return new CAneuploidiaPositiva;
	case 3: return new CAneuploidiaNegativa;
	case 4: return new CAneuploidiaAleatoria;
	case 5: return new CMutacioPerTranslocacio;
//	case 6: return new CEuploidiaPositiva;
//	case 7: return new CMutacioPerTranslocacioReciproca;
	default: return NULL;
	}
}

CMutacioCariotip * CMutacioCariotip::Nova(string tipus)
{
	if (tipus==CMutacioPerFusio::Tipus()) return new CMutacioPerFusio;
	if (tipus==CMutacioPerEscisio::Tipus()) return new CMutacioPerEscisio;
	if (tipus==CAneuploidiaPositiva::Tipus()) return new CAneuploidiaPositiva;
	if (tipus==CAneuploidiaNegativa::Tipus()) return new CAneuploidiaNegativa;
	if (tipus==CAneuploidiaAleatoria::Tipus()) return new CAneuploidiaAleatoria;
	if (tipus==CMutacioPerTranslocacio::Tipus()) return new CMutacioPerTranslocacio;
//	if (tipus==CEuploidiaPositiva::Tipus()) return new CEuploidiaPositiva;
//	if (tipus==CMutacioPerTranslocacioReciproca::Tipus()) return new CMutacioPerTranslocacioReciproca;
	else return NULL;
}

//////////////////////////////////////////////////////////////////////
// Membres estatics (especialitzacions)
//////////////////////////////////////////////////////////////////////

string CMutacioPerFusio::Tipus()
{
	return "Mutacio/Cariotip/Fusio";
}

string CMutacioPerEscisio::Tipus()
{
	return "Mutacio/Cariotip/Escisio";
}

string CMutacioPerTranslocacio::Tipus()
{
	return "Mutacio/Cariotip/Translocacio";
}

string CMutacioPerTranslocacioReciproca::Tipus()
{
	return "Mutacio/Cariotip/TranslocacioReciproca";
}

string CAneuploidiaPositiva::Tipus()
{
	return "Mutacio/Cariotip/Aneuploidia/Positiva";
}

string CAneuploidiaNegativa::Tipus()
{
	return "Mutacio/Cariotip/Aneuploidia/Negativa";
}

string CAneuploidiaAleatoria::Tipus()
{
	return "Mutacio/Cariotip/Aneuploidia/Aleatoria";
}

string CEuploidiaPositiva::Tipus()
{
	return "Mutacio/Cariotip/Euploidia/Positiva";
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CMutacioCariotip::ProvaClasse(void)
{
	out << clrscr;
	out << blanc.brillant() << "Provant MutacioCariotip" << blanc.fosc() << endl;
	out << "Entra un numero entre 1 i " << CMutacioCariotip::Nombre() << " per veure una mutacio" << endl;
	out << "Entra un numero fora de l'interval per sortir" << endl;
	CCariotip * car = new CCariotip;
	car->init(0, rnd.get(0,15));
	car->dump(out);
	uint32 a;
	while (cin>>a && a && a<=CMutacioCariotip::Nombre())
	{
		CMutacioCariotip * mutacio = CMutacioCariotip::Nova(a-1);
		out << mutacio->tipus() << endl; 
		mutacio->muta(*car);
		car->dump(out);
		delete mutacio;
	}
	delete car;
}

