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

#include "TopologiaToroidal.h"
#include "Substrat.h"
#include "Agent.h"
#include "Itineraris.h"
#include "Cumulador.h"
#include "Temporitzador.h"

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

// Proves eines d'implementacio
//	CMissatger::ProvaClasse();
//	ProvaTemplateCompatibilitat<uint32>(); // OK
//	CEncaix<uint32>::ProvaClasse();
//	CRandomStream::ProvaClasse(); // OK
//	CLlistaEstatica<int>::ProvaClasse(); // Ei aixo no rula :-)
//	CLlistaDinamica<int>::ProvaClasse();
//	CGeneradorMascares<uint32>::ProvaClasse();

// Proves Biotop
//	CTopologia<CColor>::ProvaClasse(); //OK
//	CTopologiaToroidal<CColor>::ProvaClasse(); //OK
//	CSubstrat::ProvaClasse(); //OK
//	CTopologiaToroidal<CSubstrat>::ProvaClasse(); //OK

// Proves Agents diversos
//	CAgentEnmascarador<void *()>::ProvaClasse(); //OK
//	CTemporitzador::ProvaClasse(); //OK
//	CCumulador::ProvaClasse();//OK
//	CClarejador::ProvaClasse();//Ok
//	CPosicionadorAleatori::ProvaClasse(); //OK
//	CDireccionadorAleatori::ProvaClasse(); //OK
//	CPosicionadorZonal::ProvaClasse(); //OK
	CMultiAgent::ProvaClasse();

//	CTaxo::ProvaClasse();
//	CTaxonomista::ProvaClasse();
//	CCromosoma::ProvaClasse();
//	CComunitat::ProvaClasse();
//	CBiosistema::ProvaClasse();

//	cout << ">> Final de proves. Pulsa una tecla...";
//	char a;
//	cin >> a;
}
