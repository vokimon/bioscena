// MainBio.cpp: Main function for debuging
//
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include "BioIncludes.h"
#include "AppInfo.h"
#include "Encaix.h"
#include "Missatger.h"
#include "RandomStream.h"
#include "Color.h"

#include "TopologiaToroidal.h"
#include "Substrat.h"

//#include "Biosistema.h"
//#include "Comunitat.h"
//#include "Taxonomista.h"
//#include "LlistaEstatica.h"
//#include "LlistaDinamica.h"
//#include "Cromosoma.h"

using namespace std;

void main () 
{
	out << cyan.brillant() << CAppInfo::MyAppInfo << blanc << endl;

// Proves classes d'implementacio

//	CMissatger::ProvaClasse();
	CEncaix<uint32>::ProvaClasse();
//	CRandomStream::ProvaClasse(); // OK
//	CLlistaEstatica<int>::ProvaClasse(); // Ei aixo no rula :-)
//	CLlistaDinamica<int>::ProvaClasse();
//	CGeneradorMascares<uint32>::ProvaClasse();

// Proves classes  Genetix

//	CTopologia<CColor>::ProvaClasse();
//	CTopologiaToroidal<CColor>::ProvaClasse();
	CSubstrat::ProvaClasse();

//	CTaxo::ProvaClasse();
//	CTaxonomista::ProvaClasse();
//	CCromosoma::ProvaClasse();
//	CComunitat::ProvaClasse();
//	CBiosistema::ProvaClasse();

//	cout << ">> Final de proves. Pulsa una tecla...";
//	char a;
//	cin >> a;

}
