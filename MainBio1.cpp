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
#include "Actuadors.h"
#include "Temporitzador.h"
#include "MultiAgent.h"
#include "Aleaturitzador.h"

#include "Taxo.h"
#include "Taxonomista.h"
#include "LlistaEstatica.h"
#include "EnergiaDisipable.h"
#include "Organisme.h"
//#include "Comunitat.h"
//#include "Biosistema.h"
//#include "LlistaDinamica.h"
//#include "Cromosoma.h"

using namespace std;

void main () 
{
	out << cyan.brillant() << CAppInfo::MyAppInfo << blanc << endl;

// Proves eines d'implementacio
//	CMissatger::ProvaClasse(); //Ok
//	ProvaTemplateCompatibilitat<uint32>(); // OK
//	CEncaix<uint32>::ProvaClasse();
//	CRandomStream::ProvaClasse(); // OK
//	CLlistaEstatica<int>::ProvaClasse();
//	CLlistaDinamica<int>::ProvaClasse();
//	CGeneradorMascares<uint32>::ProvaClasse();

// Proves Biotop
//	CTopologia<CColor>::ProvaClasse(); //OK TODO: Provar tipus propis i scan
//	CTopologiaToroidal<CColor>::ProvaClasse(); //OK
//	CSubstrat::ProvaClasse(); //
//	CTopologiaToroidal<CSubstrat>::ProvaClasse(); //OK

// Proves Agents diversos
//	CFuncioAgent::ProvaClasse(); //OK
//	CAleaturitzador::ProvaClasse(); //Ok
//	CTemporitzador::ProvaClasse(); //OK
//	CNutridor::ProvaClasse();//OK
//	CDesnutridor::ProvaClasse();//Ok
//	CPosicionadorAleatori::ProvaClasse(); //OK
	CDireccionadorAleatori::ProvaClasse(); //OK
//	CPosicionadorZonal::ProvaClasse(); //OK
//	CMultiAgent::ProvaClasse(); //Ok

// Proves Comunitat
//	CEnergiaDisipable::ProvaClasse();
//	COrganisme::ProvaClasse();
//	CTaxo::ProvaClasse();
//	CTaxonomista::ProvaClasse();
//	CCromosoma::ProvaClasse();
//	CComunitat::ProvaClasse();
//	CBiosistema::ProvaClasse();

//	cout << ">> Final de proves. Pulsa una tecla...";
//	char a;
//	cin >> a;
}
