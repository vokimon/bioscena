// Taxonomista.cpp: implementation of the CTaxonomista class.
//
//////////////////////////////////////////////////////////////////////

#include "BioIncludes.h"
#include "Taxonomista.h"

//////////////////////////////////////////////////////////////////////
// Variables estatiques
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaxonomista::CTaxonomista ()
{
	m_ultimTaxo=0;
	m_calEspeciar=true;
}

CTaxonomista::~CTaxonomista()
{
	map<uint32,CInfoTaxo>::iterator it = m_taxons.begin();
	for (; it!=m_taxons.end(); ++it) {
		// TODO: Destrosar el que calgui
//		it->cos(NULL);
		}
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
	// Aquesta implementació no considera creuaments
	return false;
}

bool CTaxonomista::defuncio(uint32 tx)
	// Fa el necessari quan mor un organisme del taxo indicat
	// Retorna si el taxo s'ha extingit
	// Pre: tx es un taxo existent
{
	--m_taxons[tx];
	if (!m_taxons.erase(tx))
		return true;
	return false;
}

uint32 CTaxonomista::naixement(uint32 taxoPare, bool mutat)
	// Pre: taxoPare es un taxo existent
{
	CInfoTaxo & infoPare = m_taxons[taxoPare];
	if (!mutat || !m_calEspeciar) {
		// TODO: Estadistiques
		++infoPare;
		return taxoPare;
	}
	if (!(m_ultimTaxo&070)) m_ultimTaxo+=010;
	CInfoTaxo taxo(infoPare.primigeni,taxoPare);
	++taxo;
	m_taxons.insert(make_pair(m_ultimTaxo,taxo));
	return m_ultimTaxo++;
}

uint32 CTaxonomista::nouTaxoIndependent()
{
	if (!(m_ultimTaxo&070)) m_ultimTaxo+=010;
	CInfoTaxo taxo(m_ultimTaxo,m_ultimTaxo);
	++taxo;
	m_taxons.insert(make_pair(m_ultimTaxo,taxo));
	return m_ultimTaxo++;
}

void CTaxonomista::envelleixTaxons()
{
	// Res
}

uint32 CTaxonomista::discrimina(uint32 taxo)
// Genera un nou taxo dintre d'un altre existent
{
	// No cal discretitzar
	return taxo;
}

uint32 CTaxonomista::parentesc(uint32 taxo1, uint32 taxo2) const
{
	return 0;	
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////
CMissatger & CTaxonomista::dump(CMissatger & msg)
{
/*
	for (uint32 i=nodePrimer(); !nodeFinal(i);i=nodeSeguent(i))
		msg << i<<":\t"<< *(valors[i].taxo) << " \tCens:\t" << valors[i].cens<< endl;
	dumpDisponibles(msg);
*/
	return msg;
}
//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CTaxonomista::ProvaClasse(void)
{
/*
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
*/
}

void CTaxonomista::activaEspeciacio(bool especiar)
{
	m_calEspeciar = especiar;
}

//////////////////////////////////////////////////////////////////////
// Implementacio
//////////////////////////////////////////////////////////////////////

