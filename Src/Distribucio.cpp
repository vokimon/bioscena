// Distribucio.cpp: implementation of the CDistribucio class.
//
//////////////////////////////////////////////////////////////////////

#include "Distribucio.h"
#include <time.h>


void CDistribucio::ClassTest() {
	{
		uint32 histograma[50] = {};
		time_t t= time(NULL);
		for (int i = 0xffffff; i--;) {
			histograma[CDistribucio(0,10,5)]++;
		}
		out << "Elapsed: " << (time(NULL)-t) << std::endl;
		for (int i = 50; i--;) {
			out << (double)histograma[i]/0xffffff << std::endl;;
		}
	}
	{
		uint32 histograma[50] = {};
		time_t t= time(NULL);
		CDistribucio d(0,10,5);
		for (int i = 0xffffff; i--;) {
			histograma[d]++;
		}
		out << "Elapsed: " << (time(NULL)-t) << std::endl;
		for (int i = 50; i--;) {
			out << (double)histograma[i]/0xffffff << std::endl;;
		}
	}
}


