// MutacioGenica.cpp: implementation of the CMutacioGenica class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990911 VoK - Creat a partir del que hi havia a CCromosoma
// 19990911 VoK - Afegits tipus i factories
// 19990911 VoK - Fix: Algunos bugs que havia en los operadores
// 19991209 VoK - No fan servir agafa/fixaCromosoma sino el protocol
//                public de CCromosoma.
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

void CMutacioGenica::muta(CCromosoma & cr)
// Generalitzacio Estructural -> Genica
{
	uint32 codo = cr.codoAleatori();
	if (cr.tamany())
		muta(cr[codo]);
}

void CMutacioPuntualDrastica::muta(t_codo & c)
// Substitucio total d'un codo
// (Granularitat Codo)
{
	if (traceMutacions)
		out << "Mutacio Puntual de tot el codo: "
			<< endl;
	c=rnd.get();
}

void CMutacioPuntualBinaria::muta(t_codo & c)
// Invertim nomes un Bit
// (Granularitat Base)
{
	if (traceMutacions)
		out << "Mutacio Puntual de d'una base: "
			<< endl;
	c^= 0x00000001<<rnd.get(0,31);
}

void CMutacioPuntualBinariaGaussiana::muta(t_codo & c)
// El numero d'uns invertits es una distribucio de gauss amb mitja 4=32bits*1/(2^3)
// (Granularitat Codo)
{
	if (traceMutacions)
		out << "Mutacio Gaussiana de tot el codo: "
			<< endl;
	c^=(rnd.get()&rnd.get()&rnd.get());
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

//////////////////////////////////////////////////////////////////////
// Membres estatics
//////////////////////////////////////////////////////////////////////

uint32 CMutacioGenica::Nombre(void)
{
	return 3;
}

CMutacioGenica * CMutacioGenica::Nova(uint32 n)
{
	switch (n)
	{
	case 0: return new CMutacioPuntualBinaria;
	case 1: return new CMutacioPuntualBinariaGaussiana;
	case 2: return new CMutacioPuntualDrastica;
	default: return NULL;
	}
}

CMutacioGenica * CMutacioGenica::Nova(string tipus)
{
	if (tipus==CMutacioPuntualBinaria::Tipus()) return new CMutacioPuntualBinaria;
	if (tipus==CMutacioPuntualBinariaGaussiana::Tipus()) return new CMutacioPuntualBinariaGaussiana;
	if (tipus==CMutacioPuntualDrastica::Tipus()) return new CMutacioPuntualDrastica;
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

