// Genotip.cpp: implementation of the CGenotip class.
//
//////////////////////////////////////////////////////////////////////

#include "Genotip.h"
#include "RandomStream.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGenotip::CGenotip()
{

}

CGenotip::~CGenotip()
{

}

uint32 CGenotip::seguentInstruccio(uint32 * fenotip)
{
	return rnd.get(0,32);
}

bool CGenotip::init(CCariotip & c)
{
	return true;
}
