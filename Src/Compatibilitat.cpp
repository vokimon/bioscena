// Compatibilitat.cpp: implementation of the Compatibilitat templates.
//
//////////////////////////////////////////////////////////////////////

#include "Compatibilitat.h"
#include "GeneradorMascares.h"

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void ProvaCompatibilitat () {
	cout << "\n>>>> Proves per als templates de Compatibilitat" << endl;
	cout << "---- Generant fitxer amb les estadistiques..." << endl;
	cout << "---- Fitxer de sortida: Estadisticas.xls" << endl;

	CGeneradorMascares<t_clau> generador;
	unsigned int i;
	unsigned int matxes, encerts;
	unsigned int mitja=0;
	t_clau z=0, matxer=0;
	t_clau tolerancia;

	ofstream fo("Estadisticas.xls",ios::out);
	fo << "\nCompatibilitat tipus 2\n";
	for (t_clau tolerats=generador.uns+1; tolerats--;) {
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

/*
void ProvaClasse () {

	unsigned int i,j;
	unsigned int matxes, encerts;
	t_clau z=0, matxer=0;
	CGeneradorMascares<t_clau> generador;
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

