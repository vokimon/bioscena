// FuncioAgent.cpp: implementation of the CFuncioAgent class.
//
//////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <fstream>
#include "FuncioAgent.h"
#include "Missatger.h"
#include "Color.h"

using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Construccio/Desctruccio
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

static void hola(){out << "Hola mon!"<<endl;}

void CFuncioAgent::ProvaClasse ()
{
	out << clrscr;
	out << blanc.brillant() << "Provant Agent Funcio Usuari" << blanc.fosc() << endl;
	CFuncioAgent ag(hola);
	ag();
	// De fet es una prova molt simpleta :-)
	ag.dumpAll(out);
	cin.get();
}

