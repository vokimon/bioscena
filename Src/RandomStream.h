// RandomStream.h: interface for the CRandomStream class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_RANDOMSTREAM_H_INCLUDED)
#define __KKEP_RANDOMSTREAM_H_INCLUDED

#include <stdlib.h>
#include <time.h>
#include "BioIncludes.h"

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
	inline CRandomStream& operator>>(uint32 &i){i=seed=peekSeed();return *this;};
// Proves
public:
	static void ProvaClasse(void);
// Implementacio
private:
	uint32 peekSeed()
	{
		return (uint32)rand() ^ (uint32)rand()<<15 ^ (uint32)rand()<<30;
//		return (seed * 31415821 + 1) & 0xFFFFFFFF;
//		return (seed * 16807) % 0x7FFFFFFF;
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
	KKEPAssert(limitInferior <= limitSuperior, 
		"Interval creuat per a un numero aleatori.");
	uint32 diferencia=limitSuperior-limitInferior+1;
	uint32 mascara=0xFFFFFFFF;
	while (~mascara<diferencia) mascara<<=1;
	// La mascara ens permet reduir l'interval total del random
	// mantenint la distribucio plana. Per acabar d'ajustar-ho
	// al que volem destriem les que no ens agraden.
	uint32 escollit;
	do escollit=get(); while ((escollit&~mascara)>=diferencia);
	return limitInferior + (escollit&~mascara);
}

extern CRandomStream rnd;

#endif // !defined(__KKEP_RANDOMSTREAM_H_INCLUDED)
