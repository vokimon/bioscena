// MainBio.cpp: Main function for debuging
//
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include "BioIncludes.h"
#include "AppInfo.h"
#include "Compatibilitat.h"
#include "Missatger.h"
#include "RandomStream.h"
#include "Color.h"
#include "Configuracio.h"

#include "TopologiaToroidal.h"
#include "Substrat.h"

#include "Agent.h"
#include "FuncioAgent.h"
#include "Itineraris.h"
#include "Actuadors.h"
#include "MultiAgent.h"
#include "Temporitzador.h"
#include "Aleaturitzador.h"
#include "Iterador.h"
#ifdef WIN32
#include <wincon.h>
#endif

//#include "Taxo.h"
//#include "Taxonomista.h"
//#include "LlistaEstatica.h"
#include "EnergiaDisipable.h"
#include "Organisme.h"
#include "Comunitat.h"
#include "Cromosoma.h"
#include "MutacioGenica.h"
#include "Cariotip.h"
#include "MutacioCariotip.h"
#include "Biosistema.h"
//#include "LlistaDinamica.h"

using namespace std;

int main (int argc, char**argv) 
{
//try{
	out << cyan.brillant() << CAppInfo::MyAppInfo << blanc << endl;
#ifdef WIN32
	// A fotres, nomes volia una forma estandard de canviar les files i columnes
	_COORD tamany={80,50};
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),tamany);
#endif

	if (argc>1) {
		Config.dump(out);
		exit(0);
	}

// Proves eines d'implementacio
//	CMissatger::ProvaClasse(); //Ok
//	ProvaTemplateCompatibilitat<uint32>(); // OK
//	CEncaix<uint32>::ProvaClasse();
//	CRandomStream::ProvaClasse(); // OK
//	CLlistaEstatica<int>::ProvaClasse();
//	CLlistaDinamica<int>::ProvaClasse();
//	CGeneradorMascares<uint32>::ProvaClasse();
//	CConfiguracio::ProvaClasse();

// Proves Biotop
//	CTopologia<CColor>::ProvaClasse(); //OK TODO: Provar tipus propis i scan
//	CTopologiaToroidal<CColor>::ProvaClasse(); //OK
//	CSubstrat::ProvaClasse(); //
//	CTopologiaToroidal<CSubstrat>::ProvaClasse(); //OK

// Proves Agents
//	CFuncioAgent::ProvaClasse(); //OK
//	CTemporitzador::ProvaClasse(); //OK
//	CAleaturitzador::ProvaClasse(); //Ok
//	CIterador::ProvaClasse(); //Ok
//	CNutridor::ProvaClasse(); //OK
//	CDesnutridor::ProvaClasse(); //OK
//	CPosicionadorAleatori::ProvaClasse(); //OK
//	CItinerari::ProvaClasse(); //OK
//	CDireccionadorAleatori::ProvaClasse(); //OK
//	CPosicionadorZonal::ProvaClasse(); //OK
//	CMultiAgent::ProvaClasse(); //OK
//	CAgent::ProvaClasse(); //OK

// Proves Comunitat
//	CEnergiaDisipable::ProvaClasse(); // OK 20000218
//	COrganisme::ProvaClasse(); // OK 20000218
//	CTaxo::ProvaClasse();
//	CTaxonomista::ProvaClasse();
//	CCromosoma::ProvaClasse(); // OK
//	CMutacioGenica::ProvaClasse(); // OK
//	CCariotip::ProvaClasse();
//	CMutacioCariotip::ProvaClasse();
//	CGenotip::ProvaClasse(); // OK 19991202
//	CComunitat::ProvaClasse(); // OK
	CBiosistema::ProvaClasse();

//	cout << ">> Final de proves. Pulsa la tecla de retorn...";
//	cin.get();
/*}
catch(exception& e)
{
	out << "Recogida excepcion: '" << e.what() << "'" << endl;
}
catch(...)
{
	error << "Ostia patxi, la he cogido" << endl;
	throw;
}*/
	return 0;
}
