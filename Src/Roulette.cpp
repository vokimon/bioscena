// Roulette.cpp: implementation of the CRoulette class.
//
//////////////////////////////////////////////////////////////////////

#include "Roulette.h"
#include <time.h>


void CRoulette::ClassTest() {
	{
		uint32 histograma[50] = {};
		time_t t= time(NULL);
		for (int i = 0xffffff; i--;) {
			histograma[CRoulette()]++;
		}
		out << "Elapsed: " << (time(NULL)-t) << std::endl;
		for (int i = 50; i--;) {
			out << (double)histograma[i]/0xffffff << std::endl;;
		}
	}
	{
		uint32 histograma[50] = {};
		time_t t= time(NULL);
		CRoulette d;
		for (int i = 0xffffff; i--;) {
			histograma[d]++;
		}
		out << "Elapsed: " << (time(NULL)-t) << std::endl;
		for (int i = 50; i--;) {
			out << (double)histograma[i]/0xffffff << std::endl;;
		}
	}
}


