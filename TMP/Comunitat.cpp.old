// Comunitat.cpp: implementation of the CComunitat class.
//
//////////////////////////////////////////////////////////////////////

#include "Comunitat.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComunitat::CComunitat(uint32 nIndividus)
	: individus(nIndividus) , txmist(nIndividus, 6)
{

}

CComunitat::~CComunitat()
{

}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

uint32 CComunitat::introdueix(CIndividu * pind)
{
	individus.insereixAlInici(pind);
	pind->taxo = txmist.nouTaxoIndependent();
	pind->posicio = 0;
	return individus.nodePrimer();
}

uint32 CComunitat::mitosi(uint32 ind)
{
	uint32 nouInd = individus.nodeOcupa();
	individus[nouInd] = new CIndividu(*(individus[ind]));
	txmist.naixement (taxo (ind));
	taxo (nouInd, taxo(ind));
	posicio (nouInd, 0);
	return nouInd;
}

void CComunitat::defuncio(uint32 ind)
{
//	individus[ind].individu->mor();
	bool extincio=txmist.defuncio(taxo(ind));
	delete individus[ind];
	individus[ind]= NULL;
	individus.nodeAllibera(ind);
}


bool CComunitat::creua(uint32 ind1, uint32 ind2)
	// retorna cert si ind2 era receptiu a ind1
{
	// Pendent: fer lo de la llavoreta que ind1 posa en ind2, que jo he entes pero ella no

	// Ajustar els taxons
	uint32 taxo1 = taxo(ind1);
	uint32 taxo2 = taxo(ind2);
	if (txmist.creuaTaxons(taxo1, taxo2))
		for (uint32 i=individus.nodePrimer(); !individus.nodeFinal(i); i=individus.nodeSeguent(i)) 
			if (taxo(i)==taxo2)
				taxo(i,taxo1);
	return true;
}

void CComunitat::envelleixTaxons()
{
	txmist.envelleixTaxons();
	for (uint32 i=individus.nodePrimer(); !individus.nodeFinal(i); i=individus.nodeSeguent(i))
		taxo(i, txmist.discrimina(taxo(i)) );
}

//////////////////////////////////////////////////////////////////////
// Debug
//////////////////////////////////////////////////////////////////////

ostream & CComunitat::dump(ostream & stream)
{
	for (uint32 i=individus.nodePrimer(); !individus.nodeFinal(i);i=individus.nodeSeguent(i))
		cout << i<<":\t"<< taxo(i)
			<< "\t" << *(txmist[taxo(i)].taxo)
			<< " \tCens:\t" << txmist[taxo(i)].cens<< endl;
	return stream;
}

void CComunitat::ProvaClasse()
{
	CComunitat com(30);
	cout << ">>Omplint 10 taxons" << endl;
	for (uint32 i=0; i<10; i++)
		com.introdueix(new CIndividu);
	bool keep = true;
	uint32 ind,ind2,opcio;
	while (keep)
	{
		com.dump(cout);
		cout << "1-Expontanea" << endl;
		cout << "2-Naixement" << endl;
		cout << "3-Defuncio" << endl;
		cout << "4-Creuament" << endl;
		cout << "5-Desplacament de marques i discretitzacio" << endl;
		cin >> opcio;
		switch (opcio) 
		{
		case 1:
			com.introdueix(new CIndividu);
			break;
		case 2:
			cout << ">>Individu?";
			cin >> ind;
			com.mitosi(ind);
			break;
		case 3:
			cout << ">>Individu?";
			cin >> ind;
			com.defuncio(ind);
			break;
		case 4:
			cout << ">>Individu i individu asimilat?";
			cin >> ind >>ind2;
			com.creua(ind,ind2);
			break;
		case 5:
			com.envelleixTaxons();
			break;
		default:
			keep=false;
		}
	}
}

