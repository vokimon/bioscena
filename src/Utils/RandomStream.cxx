// RamdomStream.cpp: implementation of the CRamdomStream class.
//
//////////////////////////////////////////////////////////////////////

#include <iomanip>
#include "RandomStream.h"
#include "Missatger.h"

//////////////////////////////////////////////////////////////////////
// Inicialitzacio variables estatiques
//////////////////////////////////////////////////////////////////////
uint32 CRandomStream::seed=0;
//////////////////////////////////////////////////////////////////////
// Inicialitzacio variables globals
//////////////////////////////////////////////////////////////////////
CRandomStream rnd;

//////////////////////////////////////////////////////////////////////
// Debuging
//////////////////////////////////////////////////////////////////////
void CRandomStream::ProvaClasse(void)
{
	CRandomStream rnd;
	uint32 i;
	std::cout << "Generant numeros aleatoris entre 2 i 10" << endl;
	for (i=20; i--;)
		std::cout << rnd.get(20,10) << endl;
	cin.get();
	rnd >> i;
	uint32 max=i, min=i, j=0;
	while (true)//cin>>c, c!='n')
	{
		j++;
		rnd >> i;
		if (i>max) max=i;
		else if (i<min) min=i;
		else continue;
		std::cout 
			<< "Num: " 
			<< hex << setw(8) << setiosflags(ios::internal) << setfill('0') << j
			<< "  max:" 
			<< hex << setw(8) << setiosflags(ios::internal) << setfill('0') << max 
			<< "  random:" 
			<< hex << setw(8) << setiosflags(ios::internal) << setfill('0') << i 
			<< "  min:" 
			<< hex << setw(8) << setiosflags(ios::internal) << setfill('0') << min
			<< endl;
	}
	std::cout << dec << setfill(' ');
}


