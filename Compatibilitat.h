// Compatibilitat.h: interface for the Compatibilitat templates.
//
//////////////////////////////////////////////////////////////////////
// Change Log: 
// 19990307 VoK - Integrada al projecte en forma de classe
// 19990307 VoK - Convertida en un template
// 19990308 VoK - Enmascarament precalculat 
// 19990308 VoK - Procediments inline
// 19990718 VoK - Es fa servir fstream en comptes de stdio
// 19990725 VoK - A pendre per cul la classe
//////////////////////////////////////////////////////////////////////
// Pendent Log:
//////////////////////////////////////////////////////////////////////

#if !defined(KKEP_COMPATIBILITAT_H_INCLUDED_)
#define KKEP_COMPATIBILITAT_H_INCLUDED_

//#include <time.h>
//#include <stdlib.h>
#include <fstream>
#include "GeneradorMascares.h"
#include "RandomStream.h"

//////////////////////////////////////////////////////////////////////
// Estatiques
//////////////////////////////////////////////////////////////////////

template <class KeyType> 
unsigned comptaUns (KeyType n)
{
	int res;
	for (res=0; n; n>>=1) 
		if (n&1) res++;
	return res;
};

//////////////////////////////////////////////////////////////////////
// Funcio de compatibilitat de claus no 1
//    Tolerancia al numero d'uns
//    Rendiment: un 'rand' i un ComptaUns
//    Funcio: ??
//    Comentari: ??
//////////////////////////////////////////////////////////////////////

template <class KeyType> 
inline bool compat1 (KeyType k1, KeyType k2, uint32 tolerancia) {
//	KeyType ruleta = (rand()>>10);
	KeyType ruleta = rnd.get();
	KeyType coincidencia = comptaUns( ~(k1 ^ k2));
//	return (ruleta>>tolerancia)<(coincidencia);
	return ruleta>>(tolerancia&0x7) < (coincidencia<<((tolerancia>>3)&0xf));
	}

//////////////////////////////////////////////////////////////////////
// Funcio de compatibilitat no 2
//    Tolerancia a la posicio dels uns
//    Rendiment: dos 'rand'
//    Funcio: Massa restrictiva i poc color
//    Comentari: Per lo optima que es no esta gens malament.
//////////////////////////////////////////////////////////////////////

template <class KeyType> 
inline bool compat2 (KeyType k1, KeyType k2, KeyType tolerancia) {
	KeyType ruleta = rnd.get();
	return (ruleta&(k1^k2)&~tolerancia) == 0;
	}

//////////////////////////////////////////////////////////////////////
// Funcio de compatibilitat 3
//    Tolerancia a la quantitat dels uns
//    Rendiment: un 'ComptaUns' i dos 'rand'
//    Comentari: La coincidencia i la tolerancia no son simetriques
//////////////////////////////////////////////////////////////////////

template <class KeyType> 
inline bool compat3 (KeyType k1, KeyType k2, uint32 tolerancia) {
	KeyType ruleta = rnd.get();
	return comptaUns(ruleta&(k1^k2))<tolerancia;
	}

//////////////////////////////////////////////////////////////////////
// Funcio de compatibilitat no 4
//    Tolerancia a la posicio dels uns
//    Rendiment: un 'ComptaUns' i tres 'rand'
//    Comentari: Modifica la 2 per ser menys restrictiva a la zona 
//////////////////////////////////////////////////////////////////////

template <class KeyType> 
inline bool compat4 (KeyType k1, KeyType k2, KeyType tolerancia) {
	KeyType ruleta = rnd.get();
	KeyType ruleta2 = rnd.get(0,6);
	return comptaUns(ruleta&(k1^k2)&~tolerancia) <= ruleta2;
	}

/*
template <class KeyType> 
void CEncaix<KeyType>::ProvaClasse () {

	unsigned int i,j;
	unsigned int matxes, encerts;
	uint32 z(0), matxer(0);
	CGeneradorMascares<KeyType> generador;
	CRandomStream rnd;
	ofstream fo("Estadisticas.xls",ios::out);

	fo << "\nCompatibilitat tipus 1\n";
	for (int tolerats=0; tolerats<=0xFFFF;tolerats<<=1) {
		fo << "\nTolerancia\t"<< tolerats << "\t";
		for (matxes=32; matxes--;) {
			encerts=0;
			for (j=100;j--;) {
				matxer=~generador.ambUns(matxes);
				for (i=10; i--;)
					if (compat1 (z,matxer,tolerats)) encerts++;
			}
			fo << encerts << '\t';
		}
		fo.flush();
	}
	fo.close();
}
*/

template <class KeyType> 
void ProvaTemplateCompatibilitat () {

	cout << "\n>>>> Proves per als templates de Compatibilitat" << endl;
	cout << "---- Generant fitxer amb les estadistiques..." << endl;
	cout << "---- Fitxer de sortida: Estadisticas.xls" << endl;

	CGeneradorMascares<KeyType> generador;
	unsigned int i,j;
	unsigned int matxes, encerts;
	unsigned int mitja=0;
	KeyType z=0, matxer=0;
	KeyType tolerancia;

	ofstream fo("Estadisticas.xls",ios::out);
	fo << "\nCompatibilitat tipus 2\n";
	for (KeyType tolerats=generador.uns+1; tolerats--;) {
		fo << "\nTolerancia\t"<< tolerats << "\t";
		tolerancia = generador.ambUns(tolerats); // Per tolerancia local
		for (matxes=generador.uns+1; matxes--;) {
			encerts=0;
			for (i=1000;i--;) {
				matxer=~generador.ambUns(matxes);
//				if (compat1 (z,matxer,tolerats)) encerts++; // Per tolerancia quantitativa
//				if (compat2 (z,matxer,tolerancia)) encerts++; // Per tolerancia local
//				if (compat3 (z,matxer,tolerats)) encerts++; // Per tolerancia quantitativa
				if (compat4 (z,matxer,tolerancia)) encerts++; // Per tolerancia local
			}
			fo << encerts << '\t';
			mitja+=encerts;
		}
		fo.flush();
	}
	fo.close();
	cout << "---- Mitja d'encerts: " << (mitja>>10) << endl;
}

// Em quedo de moment amb la 2
#define sonCompatibles compat2

#endif // !defined(KKEP_COMPATIBILITAT_H_INCLUDED_)
