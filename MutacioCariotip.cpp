// MutacioCariotip.cpp: implementation of the CMutacioCariotip class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990914 VoK - Creat a imatge i semblant de CMutacioGenica
// 19991126 VoK - Fix: No esborraven el material genetic residual
//                a les Mutacions per Fusio
//////////////////////////////////////////////////////////////////////

#include <algorithm>
#include "MutacioCariotip.h"
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

CMutacioCariotip::CMutacioCariotip()
{
}

CMutacioCariotip::~CMutacioCariotip()
{
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

void CMutacioPerFusio::muta(CCariotip & car)
// Fusio de dos cromosomes d'un cariotip
// (Granularitat Cariotip)
{
	if (!car.tamany()) return; // Fugida discreta
	uint32 idxOrigen=car.cromosomaAleatori();
	uint32 idxDesti=car.cromosomaAleatori();
	if (traceMutacions)
		out << "Mutacio per Fusio: "
			<< " Cromosoma Origen: " << idxOrigen
			<< " Cromosoma Desti: " << idxDesti
			<< endl;
	CCariotip::t_cromosoma crm1 = car[idxOrigen];
	CCariotip::t_cromosoma crm2 = car[idxDesti];
	crm2->fusiona(*crm1);
	if (idxDesti!=idxOrigen) {
		crm1 = car.extreu(idxOrigen);
		if (crm1) delete crm1;
	}
}

void CMutacioPerEscisio::muta(CCariotip & car)
// Escisio d'un cromosoma del cariotip
// (Granularitat Cariotip)
{
	if (!car.tamany()) return; // Fugida discreta
	uint32 idxOrigen=car.cromosomaAleatori();
	uint32 idxDesti=rnd.get(0,car.tamany());

	CCariotip::t_cromosoma crm1 = car[idxOrigen];
	if (crm1->tamany()<2) return; // Fugida discreta

	uint32 idxCentromer = rnd.get(1,crm1->tamany()-1);

	if (traceMutacions)
		out << "Mutacio per Escisio: " 
			<< " Escindit: " << idxOrigen
			<< " Desti: " << idxDesti
			<< " Centromer: " << idxCentromer
		    << endl;
	CCariotip::t_cromosoma crm2 = new CCromosoma;
	if (!crm2) {
		error << "Mutant per Escisio: Error de memoria" << endl;
		cin.get();
	}
	crm1->parteix(*crm2,idxCentromer);
	car.afegeix(crm2,idxDesti);
}

void CAneuploidiaPositiva::muta(CCariotip & car)
// Duplicacio total d'un cromosoma del cariotip
// (Granularitat Cariotip)
{
	uint32 cromosomaDuplicat = car.cromosomaAleatori();
	uint32 posicioFinal = rnd.get(0,car.tamany());
	if (traceMutacions)
		out << "Mutacio per Duplicacio Aneuplodica: "
			<< " Duplicat: " << cromosomaDuplicat
			<< " Posicio: " << posicioFinal
			<< endl;
	if (!car.tamany())
		return;
	CCariotip::t_cromosoma crm1 = car[cromosomaDuplicat];
	CCariotip::t_cromosoma crm2 = new CCromosoma;
	if (!crm2) {
		error << "Mutant AneuploidiaPositiva: Error de memoria" << endl;
		cin.get();
	}

	crm2->init(*crm1);
	car.afegeix(crm2,posicioFinal);
}

void CAneuploidiaNegativa::muta(CCariotip & car)
// Duplicacio total d'un cromosoma del cariotip
// (Granularitat Cromosoma)
{
	if (car.tamany()<2)	return; // Fugida Discreta
	uint32 cromosomaEliminat = car.cromosomaAleatori();
	if (traceMutacions)
		out << "Mutacio per Eliminacio Aneuplodica: "
			<< " Eliminat: " << cromosomaEliminat
			<< endl;
	CCariotip::t_cromosoma crm1 = car.extreu(cromosomaEliminat);
	if (crm1) delete crm1;
}

void CEuploidiaPositiva::muta(CCariotip & car)
// Duplicacio total del cariotip
// (Granularitat Cariotip)
{
	if (traceMutacions)
		out << "Mutacio per Duplicacio Euplodica: "
			<< endl;
	if (!car.tamany()) return;
	uint32 ultim=car.tamany()-1;
	uint32 restants=car.tamany();
	while(restants--) {
		CCariotip::t_cromosoma crm1 = car[ultim];
		CCariotip::t_cromosoma crm2 = new CCromosoma;
		if (!crm2) {
			error << "Mutant EuploidiaPositiva: Error de memoria" << endl;
			cin.get();
		}
		crm2->init(*crm1);
		car.afegeix(crm2,0);
	}
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

string CAneuploidiaPositiva::tipus()
{
	return CAneuploidiaPositiva::Tipus();
}

string CAneuploidiaNegativa::tipus()
{
	return CAneuploidiaNegativa::Tipus();
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
	return 5;
}

CMutacioCariotip * CMutacioCariotip::Nova(uint32 n)
{
	switch (n)
	{
	case 0: return new CMutacioPerFusio;
	case 1: return new CMutacioPerEscisio;
	case 2: return new CAneuploidiaPositiva;
	case 3: return new CAneuploidiaNegativa;
//	case 4: return new CEuploidiaPositiva;
	default: return NULL;
	}
}

CMutacioCariotip * CMutacioCariotip::Nova(string tipus)
{
	if (tipus==CMutacioPerFusio::Tipus()) return new CMutacioPerFusio;
	if (tipus==CMutacioPerEscisio::Tipus()) return new CMutacioPerEscisio;
	if (tipus==CAneuploidiaPositiva::Tipus()) return new CAneuploidiaPositiva;
	if (tipus==CAneuploidiaNegativa::Tipus()) return new CAneuploidiaNegativa;
//	if (tipus==CEuploidiaPositiva::Tipus()) return new CEuploidiaPositiva;
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

string CAneuploidiaPositiva::Tipus()
{
	return "Mutacio/Cariotip/Aneuploidia/Positiva";
}

string CAneuploidiaNegativa::Tipus()
{
	return "Mutacio/Cariotip/Aneuploidia/Negativa";
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

