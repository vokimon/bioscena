// RamdomStream.cpp: implementation of the CRamdomStream class.
//
//////////////////////////////////////////////////////////////////////

#include <iomanip>
#include <iostream>
#include "RandomStream.hxx"

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
	using std::hex;
	using std::dec;
	using std::ios;
	using std::setw;
	using std::setiosflags;
	using std::setfill;
	CRandomStream rnd;
	std::cout << "Generant numeros aleatoris entre 2 i 10" << std::endl;
	for (uint32 i=20; i--;)
		std::cout << rnd.get(20,10) << std::endl;
	std::cin.get();
	uint32 value = rnd.get();
	uint32 max=value, min=value, j=0;
	while (true)//cin>>c, c!='n')
	{
		j++;
		value = rnd.get();
		if (value>max) max=value;
		else if (value<min) min=value;
		else continue;
		std::cout 
			<< "Num: " 
			<< hex << setw(8) << setiosflags(ios::internal) << setfill('0') << j
			<< "  max:" 
			<< hex << setw(8) << setiosflags(ios::internal) << setfill('0') << max 
			<< "  random:" 
			<< hex << setw(8) << setiosflags(ios::internal) << setfill('0') << value 
			<< "  min:" 
			<< hex << setw(8) << setiosflags(ios::internal) << setfill('0') << min
			<< std::endl;
	}
	std::cout << dec << setfill(' ');
}


