// Taxon.cpp: implementation of the CTaxon class.
//
//////////////////////////////////////////////////////////////////////

#include "Taxo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaxo::CTaxo(uint32 n) : nMarques(n)
{
	marques = new uint32 [nMarques];
	for (uint32 i=nMarques;i--;)
		marques[i]=1;
}

CTaxo::CTaxo(CTaxo &taxo) : nMarques(taxo.nMarques)
{
	marques = new uint32 [nMarques];
	for (uint32 i=nMarques;i--;)
		marques[i]=taxo.marques[i];
}

CTaxo::~CTaxo()
{

}

uint32 CTaxo::cosanguineitatAmb(CTaxo *t)
{
	uint32 i,a;
	// Tenen mes pes els de major index (mes antics)
	for (i=nMarques,a=0; i && marques[--i]==t->marques[i];a++); 
	return a;
}


void CTaxo::marca(uint32 nivell, uint32 valor)
{
	marques[nivell]=valor;
}


void CTaxo::lligaAmb(uint32 cosanguinietat, uint32 &diferencial, uint32 &valorSubstituit, uint32 &nouDiferencial)
	// Aquest metode treu feina a l'hora de lligar dos taxons
{
/*	for (uint32 i = nMarques; cosanguinietat;cosanguinietat--)
		marques[--i]=tx->marques[i];
*/
	uint32 dis = nMarques-cosanguinietat-1;
	uint32 postdis = dis-1;
	marques[dis]=diferencial;
	if (marques[postdis]!=valorSubstituit)
	{
		valorSubstituit=marques[postdis];
		marques[postdis]=++nouDiferencial;
	}
	else
	{
		marques[postdis]=nouDiferencial;
	}
}

			

void CTaxo::shiftaMarques(uint32 & mesAntiga, uint32 & menysAntiga, uint32 & nova)
{
	if (marques[nMarques-1]!=mesAntiga||marques[nMarques-2]!=menysAntiga) 
	{
			nova++;
		mesAntiga=marques[nMarques-1];
		menysAntiga=marques[nMarques-2];
	}
	marques[nMarques-1]=nova;
	for (uint32 i= nMarques - 2; i;i--)
		marques[i]=marques[i-1];
	marques[0]=1;
}

void CTaxo::ProvaClasse()
{
	CTaxo a(6),b(6);
	a[0]=b[0]=1;
	a[1]=b[1]=2;
	a[2]=b[2]=3;
	a[3]=b[3]=4;
	a[4]=b[4]=5;
	a[5]=b[5]=6;
//	a[3]=6;b[3]=3;
	cout << "a: " << a << endl;
	cout << "b: " << b << endl;
	cout<<(a<b?"a ":"b ")<<"Es mas pequeño"<<endl;
	cout<<"Cosanguineitat: "<< a.cosanguineitatAmb(&b)<<endl<<endl;
	uint32 x=9,y=4,z=1;
	cout << "x=" << x << "y=" << y << "z=" << z << endl;
	a.lligaAmb(4,x,y,z);
	cout << "a: " << a << endl;
	cout << "b: " << b << endl;
	cout << "x=" << x << "y=" << y << "z=" << z << endl;
}

