// Encaix.h: interface for the CEncaix template class.
//
//////////////////////////////////////////////////////////////////////
// Change Log: 
// 19990307 VoK - Integrada al projecte en forma de classe
// 19990307 VoK - Convertida en un template
// 19990308 VoK - Enmascarament precalculat 
// 19990308 VoK - Procediments inline
// 19990718 VoK - Es fa servir fstream en comptes de stdio
//////////////////////////////////////////////////////////////////////
// Pendent Log:
//////////////////////////////////////////////////////////////////////

#if !defined(KKEP_ENCAIX_H__FC0FCE01_D41D_11D2_A87F_2CDF02C10000__INCLUDED_)
#define KKEP_ENCAIX_H__FC0FCE01_D41D_11D2_A87F_2CDF02C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <time.h>
#include <stdlib.h>
#include <fstream>
#include "GeneradorMascares.h"

template <class KeyType> class CEncaix  
{
	KeyType m_mask, m_key, m_masked;
public:
	CEncaix (KeyType i=0, KeyType m=0) {m_key=i;m_mask=m;m_masked = m_key ^ m_mask;};;
	virtual ~CEncaix() {};
	KeyType key () {return m_key;};
	KeyType mask () {return m_mask;};
	KeyType masked () {return m_masked;};
	void key (KeyType key) {m_key=key; m_masked = m_key ^ m_mask;};
	void mask (KeyType mask) {m_mask=mask; m_masked = m_key ^ m_mask;};
	void unmask (void) {m_mask=0;};
	inline bool compatibleAmb (CEncaix &k2, KeyType tolerancia=~0);
	static void ProvaClasse ();
	inline static unsigned ComptaUns (KeyType n);
};

//////////////////////////////////////////////////////////////////////
// Estatiques
//////////////////////////////////////////////////////////////////////

template <class KeyType> 
unsigned CEncaix<KeyType>::ComptaUns (KeyType n)
{
	int res;
	for (res=0; n; n>>=1) 
		if (n&1) res++;
	return res;
};

//////////////////////////////////////////////////////////////////////
// Funcio de compatibilitat de claus no 1
//////////////////////////////////////////////////////////////////////

template <class KeyType> 
bool CEncaix<KeyType>::compatibleAmb (CEncaix &k2, KeyType tolerancia) {
//	KeyType ruleta = (rand()>>10);
	KeyType ruleta = rand();
	KeyType coincidencia = ComptaUns( ~(masked() ^ k2.masked()));
//	return (ruleta>>tolerancia)<(coincidencia);
	return ruleta>>(tolerancia&0x7) < (coincidencia<<((tolerancia>>3)&0xf));
	}
/*
template <class KeyType> 
void CEncaix<KeyType>::ProvaClasse () {

	unsigned int i,j;
	unsigned int matxes, encerts;
	Encaje z(0), matxer(0);

	FILE *fo;
	fo=fopen("Estadisticas.txt","w");
	fprintf(fo,"Compatibilitat tipus 3\n");
	for (int tolerats=0; tolerats<=0xFFFF;tolerats++) {
		fprintf(fo,"%d\n",tolerats);
		for (matxes=0; matxes<=32; matxes++) {
			encerts=0;
			for (j=1000;j--;) {
				matxer.key = ~Mascares::AmbUns(matxes);
				for (i=10; i--;)
					if (compatible1 (z,matxer,tolerats)) encerts++;
			}
			fprintf(fo,"%8d\n",encerts);
		}
		fflush(fo);
	}
	fclose(fo);
}
*/

//////////////////////////////////////////////////////////////////////
// Funcio de compatibilitat no 2
//    Tolerancia a la posicio dels uns
//    Rendiment: dos 'rand'
//    Funcio: Massa restrictiva i poc color
//    Comentari: Per lo optima que es no esta gens malament.
//////////////////////////////////////////////////////////////////////
/*
template <class KeyType> 
bool CEncaix<KeyType>::compatibleAmb (CEncaix &k2, KeyType tolerancia) {
	KeyType ruleta = (rand()<<16)^(rand()&0x00ffff);
	return (ruleta & (masked()^k2.masked()) & ~tolerancia) == 0;
	}
*/

//////////////////////////////////////////////////////////////////////
// Funcio de compatibilitat 3
//    Tolerancia a la quantitat dels uns
//    Rendiment: un 'ComptaUns' i dos 'rand'
//    Comentari: La coincidencia i la tolerancia no son simetriques
//////////////////////////////////////////////////////////////////////
/*
template <class KeyType> 
bool CEncaix<KeyType>::compatibleAmb (CEncaix &k2, KeyType tolerancia) {
	KeyType ruleta = (rand()<<16)^rand();
	return ComptaUns(ruleta & (masked()^k2.masked()) )<tolerancia;
	}
*/

//////////////////////////////////////////////////////////////////////
// Funcio de compatibilitat no 4
//    Tolerancia a la posicio dels uns
//    Rendiment: un 'ComptaUns' i tres 'rand'
//    Comentari: Modifica la 2 per ser menys restrictiva a la zona 
//////////////////////////////////////////////////////////////////////
/*
template <class KeyType> 
bool CEncaix<KeyType>::compatibleAmb (CEncaix &k2, KeyType tolerancia) {
	KeyType ruleta = (rand()<<16)^(rand()&0x00ffff);
	KeyType ruleta2 = rand()&3;
	return ComptaUns(ruleta & (masked()^k2.masked()) & ~tolerancia) <= ruleta2;
	}
*/

template <class KeyType> 
void CEncaix<KeyType>::ProvaClasse () {

	cout << "\n>>>> Proves per a CEncaix" << endl;
	cout << "---- Generant fitxer amb les estadistiques..." << endl;
	cout << "---- Fitxer de sortida: Estadisticas.xls" << endl;
	CGeneradorMascares<KeyType> generador;
	CEncaix z(0), matxer(0);
	unsigned int i,j;
	unsigned int matxes, encerts;
	unsigned int mitja=0;
	KeyType tolerancia;

	srand((unsigned)time(NULL));

	ofstream fo("Estadisticas.xls",ios::out);
	fo << "\nCompatibilitat tipus 2\n";
	for (int tolerats=generador.uns+1; tolerats--;) {
		fo << "\nTolerancia\t"<< tolerats << "\t";
//		tolerancia = generador.ambUns(tolerats); // Per tolerancia local
		for (matxes=generador.uns+1; matxes--;) {
			encerts=0;
			for (i=1000;i--;) {
				matxer.key(~(generador.ambUns(matxes)));
//				if (z.compatibleAmb (matxer,tolerancia)) encerts++; // Per tolerancia local
				if (z.compatibleAmb (matxer,tolerats)) encerts++; // Per tolerancia quantitativa
			}
			fo << encerts << '\t';
			mitja+=encerts;
		}
		fo.flush();
	}
	fo.close();
	cout << "---- Mitja d'encerts: " << (mitja>>10) << endl;
}


#endif // !defined(KKEP_ENCAIX_H__FC0FCE01_D41D_11D2_A87F_2CDF02C10000__INCLUDED_)
