// Taxonomista.cpp: implementation of the CTaxonomista class.
//
//////////////////////////////////////////////////////////////////////

#include "Taxonomista.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaxonomista::CTaxonomista (uint32 nTaxons, uint32 nNivells)
	: CLlistaEstatica<CTaxonomistaNode>(nTaxons)
{
	nivells=nNivells;
}

CTaxonomista::~CTaxonomista()
{
}

//////////////////////////////////////////////////////////////////////
// Manteniment dels taxons al dia
//////////////////////////////////////////////////////////////////////

bool CTaxonomista::creuaTaxons(uint32 tx1, uint32 tx2)
	// Implementa el canvis necessaris quan es creuen
	// dos individus de taxons tx1 i tx2
	// Retorna cert si com a resultat tx2 ha estat asimilat per tx1
	// Pre: Els taxons son vàlids
{
	// Pre: Els taxons son vàlids
	KKEPAssert(valors[tx1].taxo, "El primer taxo no esta en us");
	KKEPAssert(valors[tx2].taxo, "El segon taxo no esta en us");
	// Si es creuen individus del mateix taxo, no passa res
	if (tx1==tx2) return false;

	CTaxo* ptaxo1 = valors[tx1].taxo;
	CTaxo* ptaxo2 = valors[tx2].taxo;
	// Troba la correspondencia entre les marques dels dos patrons
	uint32 csng= ptaxo1->cosanguineitatAmb(ptaxo2);

	// Si nomes es diferencien en l'ultima marca elimina tx2 i retorna true
	if (csng >= nivells-1) 
	{
		valors[tx1].cens+=valors[tx2].cens;
		delete ptaxo2;
		valors[tx2].taxo=NULL;
		nodeAllibera(tx2);
		return true;
	}
	// Si no:
	uint32 tx,iniciFamilia2, finalFamilia2, finalFamilia1;

	// Localitzar l'ultim de la familia de tx1
	for (tx=tx1; 
		!nodeFinal(tx) && ptaxo1->cosanguineitatAmb(valors[tx].taxo)>=csng+1;
		tx=nodeSeguent(tx) );
	finalFamilia1=nodeAnterior(tx);

	// Determina la familia de tx2
	for (tx=tx2; 
		!nodeInici(tx) && ptaxo2->cosanguineitatAmb(valors[tx].taxo)>=csng+1;
		tx=nodeAnterior(tx) );
	iniciFamilia2=nodeSeguent(tx);

	// Adapta les marques de la familia tx2 a tx1
	// i determina el final de la familia de tx2
	CTaxo* ptaxo = valors[finalFamilia1].taxo;
	for (tx=iniciFamilia2; 
		!nodeFinal(tx) && ptaxo2->cosanguineitatAmb(valors[tx].taxo)>=csng+1;
		tx=nodeSeguent(tx) );
	finalFamilia2=nodeAnterior(tx);
	// Adaptem les marques
	uint32 diferencial = nivells-csng-1;
	uint32 valorALligar = ptaxo->marca(diferencial);
	uint32 nouDiferencial = ptaxo->marca(diferencial-1);
	// Esta expresion es peligrosa si se permiten marcas=0
	uint32 substituit = valors[iniciFamilia2].taxo->marca(diferencial-1)-1;
	uint32 txx=tx; // El posterior al final de Familia
	for (tx=iniciFamilia2; 
		tx!=txx;
		tx=nodeSeguent(tx) )
	{
		valors[tx].taxo->lligaAmb(csng, valorALligar, substituit, nouDiferencial);
	}
	// Extreu la familia de tx2
	// i inserta-la darrera de tx1
	nodeDesplacaGrup(finalFamilia1, iniciFamilia2, finalFamilia2);
	// Retorna tx2
	return false;
}

bool CTaxonomista::defuncio(uint32 tx)
{
	if (!--valors[tx].cens)
	{
		delete valors[tx].taxo;
		valors[tx].taxo = NULL;
		nodeAllibera(tx);
		return true; // Extincio
	}
	else return false;
}

void CTaxonomista::naixement(uint32 tx)
{
	valors[tx].cens++;
	valors[tx].naixements++;
}

uint32 CTaxonomista::nouTaxoIndependent()
{
	uint32 marcaMajor=
		nodeBuida()?0:valors[nodeUltim()].taxo->marca(nivells-1);
	uint32 nouNode = nodeOcupaDespresDe(nodeUltim());
	CTaxo * ptx = new CTaxo (nivells);
	ptx->marca(nivells-1,marcaMajor+1);
	valors[nouNode].cens=1;
	valors[nouNode].naixements=0;
	valors[nouNode].defuncions=0;
	valors[nouNode].taxo=ptx;
	return nouNode;
}

void CTaxonomista::envelleixTaxons()
{
	uint32 m1=0,m2=0,m3=0;
	for (uint32 tx=nodePrimer();!nodeFinal(tx);tx=nodeSeguent(tx))
		valors[tx].taxo->shiftaMarques(m1,m2,m3);
}

uint32 CTaxonomista::discrimina(uint32 taxo)
{
	KKEPAssert (valors[taxo].cens > 0,
		"Discretitzant un taxo sense poblacio.");
	// Si nomes hi ha un; no cal discretitzar
	if (valors[taxo].cens <= 1)
		return taxo;
	// Crea un taxo
	uint32 nouNode = nodeOcupaDespresDe(taxo);
	CTaxo * ptx = new CTaxo (*(valors[taxo].taxo));
	ptx->marca(0,valors[taxo].cens);
	valors[nouNode].cens=1;
	valors[nouNode].taxo=ptx;
	valors[taxo].cens--;
	return nouNode;
}

uint32 CTaxonomista::parentesc(uint32 tx1, uint32 tx2)
{
	return valors[tx1].taxo->cosanguineitatAmb(valors[tx2].taxo);
}

ostream & CTaxonomista::dump(ostream & stream)
{
	for (uint32 i=nodePrimer(); !nodeFinal(i);i=nodeSeguent(i))
		cout << i<<":\t"<< *(valors[i].taxo) << " \tCens:\t" << valors[i].cens<< endl;
	return stream;
}

void CTaxonomista::ProvaClasse(void)
{
	CTaxonomista txmist(30,20);
	uint32 taxons[20];
	cout << ">>Omplint 10 taxons" << endl;
	for (uint32 i=0; i<10; i++)
		taxons[i]=txmist.nouTaxoIndependent();
	cout << ">>uns naixements al " << taxons[3] << endl;
	txmist.naixement(taxons[3]);
	txmist.naixement(taxons[3]);
	txmist.naixement(taxons[3]);
	cout << ">>una mort al " << taxons[9] << " (amb nomes un -> extincio) i al " << taxons [3] << " mes poblat" << endl;
	txmist.defuncio(taxons[9]);
	txmist.defuncio(taxons[3]);
	bool keep=true;
	uint32 tx,tx2,opcio;
	while (keep)
	{
		txmist.dump(cout);
		cout << "1-Expontanea" << endl;
		cout << "2-Naixement" << endl;
		cout << "3-Defuncio" << endl;
		cout << "4-Creuament" << endl;
		cout << "5-Desplacament de marques" << endl;
		cout << "6-Discretitzacio" << endl;
		cin >> opcio;
		switch (opcio) 
		{
		case 1:
			txmist.nouTaxoIndependent();
			break;
		case 2:
			cout << ">>Taxo?";
			cin >> tx;
			txmist.naixement(tx);
			break;
		case 3:
			cout << ">>Taxo?";
			cin >> tx;
			txmist.defuncio(tx);
			break;
		case 4:
			cout << ">>Taxo i taxo asimilat?";
			cin >> tx >>tx2;
			txmist.creuaTaxons(tx,tx2);
			cout << ">>Parentesc entre " << tx << " i " << tx2 
				<< " = " << txmist.parentesc(tx, tx2) << endl;
			break;
		case 5:
			txmist.envelleixTaxons();
			break;
		case 6:
			cout << ">>Taxo a discretitzar?";
			cin >> tx;
			tx2 = txmist.discrimina(tx);
			cout << "Nou taxo asociat: "<< tx2 << endl;
			break;
		default:
			keep=false;
		}
	}
		
}
