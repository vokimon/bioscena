// ControlOrganisme.cpp: implementation of the CControlOrganisme class.
//
//////////////////////////////////////////////////////////////////////

#include <iomanip>
#include "turbioconio.h" 
#include "ControlOrganisme.h"
#include "Genotip.h"
#include "Configuracio.h"
#include "Color.h"

//////////////////////////////////////////////////////////////////////
// Operacions (Factoria)
//////////////////////////////////////////////////////////////////////

uint32 CControlOrganisme::Nombre(void)
{
	return 1;
	return 2;
}

CControlOrganisme * CControlOrganisme::Crea(uint32 n)
{
	switch (n)
	{
	case 0: return new CGenotip;
//	case 1: return new CControlAleatori;
	default: return NULL;
	}
}

CControlOrganisme * CControlOrganisme::Crea(string tipus)
{
	if (tipus=="ControlOrganisme/ExpressioGenica") return new CGenotip;
//	if (tipus=="ControlOrganisme/Aleatori") return new CGenotip;

	// Alguns exemples de posibles tipus de control, per fer boca :-)
	// Un com als LEE's
//	if (tipus=="ControlOrganisme/XarxaNeuronal/Predictiva") return new CXNPredictiva;
	// Un de controlat per l'usuari
//	if (tipus=="ControlOrganisme/Usuari/Consola1") return new CConsola1;

	return NULL;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CControlOrganisme::ProvaClasse()
{
	using namespace AnsiCodes;
	out << clrscr;
	out << blanc.brillant() << "Provant ControlOrganisme" << blanc.fosc() << endl;
/*
	CCariotip cariotip;
	cariotip.init(8); // Creem un cariotip de 8 cromosomes
	cariotip.dump(out); // El mostrem
	cin.get();

	CControlOrganisme * control = CControlOrganisme::Crea(rnd.get(0, CControlOrganisme::Nombre()-1)); // Creem un genotip
	if (!control) {
		err << "Ops! M'he quedat sense memoria creant el sistema de control" << endl;
		return;
		}
	control->init(cariotip); // L'omplim de la traduccio del cariotip
	control->dump(out); // El mostrem

	uint32 fenotip[16];
	for (uint32 i=16; i--;) fenotip[i]=0xFFFFFFFF<<i;

	while (cin.get()!='q') {
		uint32 instr = control->seguentInstruccio(fenotip);
		out << hex << setw(8) << instr << dec << endl;
	}

	out << "Provant creacio masiva de genotips fins que premis una tecla" << endl;
	while (!kbhit()) {
		CCariotip cariotip;
		cariotip.init(8); // Creem un cariotip de 8 cromosomes
		CGenotip genotip; // Creem un genotip
		genotip.init(cariotip); // L'omplim de la traduccio del cariotip
		// Al final de la volta es destrueixen.
	}
*/
	cin.get();
}

