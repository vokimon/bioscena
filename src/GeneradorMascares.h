// GeneradorMascares.h: interface for the CGeneradorMascares class.
//
//////////////////////////////////////////////////////////////////////
// Change Log: 
// 19990307 VoK - Integrada al projecte en forma de classe
// 19990307 VoK - Convertida en un template
// 19990307 VoK - He hagut de eliminar variables estaticas per no haver
//                d'initarles explicitament al main.
// 19990307 VoK - Realitzat el metode estatic de proves i provada
//////////////////////////////////////////////////////////////////////
// Pendent Log:
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENERADORMASCARES_H__487D83E2_D4FF_11D2_A87F_2CDF02C10000__INCLUDED_)
#define AFX_GENERADORMASCARES_H__487D83E2_D4FF_11D2_A87F_2CDF02C10000__INCLUDED_

#ifdef  _MSC_VER
	#pragma warning(disable:4786)
#endif /* _MSC_VER */

#include <iostream>
#include <iomanip>

#include "BioIncludes.h"

template <class T>
class CGeneradorMascares {
public:
	T * bit;
	unsigned int uns;
	CGeneradorMascares (void) 
	{
		T bitter;
		unsigned int i;
		for (uns=0, bitter=1; bitter; uns++, bitter<<=1);
		bit = new T[uns];
		KKEPAssert(bit, "CGeneradorMascares: Falta memoria");
		for (i=0, bitter=1; i<uns; i++, bitter<<=1)	bit[i]=bitter;
	}

	inline T ambUns (unsigned int n) {
		KKEPAssert(n<=uns,"CGeneradorMascares: Intentant crear una mascara amb mes uns");
		unsigned int r=0,i,temp;
		for (i=n;n--;) {
			while ((temp=bit[rand()&0x001F])&r);
			r |= temp;
		}
		return r;
	}

	static void ProvaClasse (void) {
		CGeneradorMascares gen;
		unsigned int n;
		cout << "\n>> Provant CGeneradorMascares" << endl;
		cout << "---- Return per obtenir una mascara" << endl;
		cout << "---- Espai per incrementar nombre d'uns" << endl;
		cout << setfill('0');
		for (n=0; n<=gen.uns; n++)
			while (getchar()!=' ')
				cout << n << " " << setw((gen.uns+3)>>2) << hex << gen.ambUns(n) << dec << endl;
		cout << setfill(' ');
	}
};


#endif // !defined(AFX_GENERADORMASCARES_H__487D83E2_D4FF_11D2_A87F_2CDF02C10000__INCLUDED_)
