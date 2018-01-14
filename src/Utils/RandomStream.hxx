// RandomStream.h: interface for the CRandomStream class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_RANDOMSTREAM_H_INCLUDED)
#define __KKEP_RANDOMSTREAM_H_INCLUDED

#include <stdlib.h>
#include <time.h>
#include "BioIncludes.h"
#include <fstream>
#include <iostream>

class CRandomStream
{
// Cosntruccio
public:
	CRandomStream() {if (!seed) setSeed();};
	CRandomStream(uint32 _seed) {setSeed(_seed);};
	virtual ~CRandomStream() {};
// Operacions
public:
	inline uint32 get (void) {return seed=peekSeed();};
	inline uint32 get (uint32 limitInferior, uint32 limitSuperior);
	inline uint32 peek(void) {return peekSeed();};
// Proves
public:
	static void ProvaClasse(void);
// Implementacio
private:
	uint32 peekSeed()
	{
		/*
		static std::fstream randomStream("/dev/random");
		unsigned int result;
		std::cout << "Peaking" << std::endl;
		randomStream.read((char*)&result,sizeof(result));
		std::cout << "Peaked" << std::endl;
		return result;
*/
//		return (uint32)rand() ^ (uint32)rand()<<15 ^ (uint32)rand()<<30;
//		return (seed * 31415821 + 1) & 0xFFFFFFFF;
		return (seed * 16807) % 0x7FFFFFFF;
		//^(seed * ##((7 raisedTo: 5) asInteger)) \\ 16r7FFFFFFF
	};
	void setSeed(uint32 nova=0) {
		if (!nova) {
			time_t t;
			time(&t);
			seed=(uint32)t;
		}
		else seed=nova;
		srand(seed);
	};
// Static data
private:
	static uint32 seed;
};

uint32 CRandomStream::get(uint32 limitInferior, uint32 limitSuperior)
// Donen un numero LI<=n<=LS segons una distribucio uniforme
// Pre: Els limits estan ordenats.
{
	KKEP_ASSERT(limitInferior <= limitSuperior, 
		"Interval creuat per a un numero aleatori.");
	uint32 toZeroBound=limitSuperior-limitInferior;
	uint32 mascara=0xFFFFFFFF;
	while (~mascara<toZeroBound) mascara<<=1;
	// La mascara ens permet reduir l'interval total del random
	// mantenint la distribucio plana. Per acabar d'ajustar-ho
	// al que volem destriem les que no ens agraden.
	while (true)
	{
		uint32 escollit=get();
		if ((escollit&~mascara)>toZeroBound) continue;
		return limitInferior + (escollit&~mascara);
	}
}

extern CRandomStream rnd;

namespace Bioscena {
	
class Random
{
public:
	/**
	 * Returns an integer number inside the interval
	 * [lowerBound,upperBound] (both bounds are included).
	 */
	static uint32 Get(uint32 lowerBound, uint32 upperBound)
	{
		KKEP_ASSERT(lowerBound <= upperBound, 
			"Random: Upperbound can't be higher than lowerbound.");
		CRandomStream & randomObject = GetGenerator();
		return randomObject.get(lowerBound,upperBound);
	}
private:
	static CRandomStream & GetGenerator()
	{
		static CRandomStream randomObject;
		return randomObject;
	}
};

}


#endif // !defined(__KKEP_RANDOMSTREAM_H_INCLUDED)
