#ifndef __BIOINCLUDES_H__
#define __BIOINCLUDES_H__

#include <iostream>
#include <assert.h>
#include "Missatger.h"
#include "portable.h"

// Definim el nostre assert
//#if (defined(_MSVC)&&defined(_DEBUG))||(!defined(_MSVC)&&!defined(NDEBUG)
#define KKEPAssert(expr, msg) (expr||((error<<__FILE__<<":"<<__LINE__<<":"<<msg<<endl) ,true))
//#else
//#define KKEPAssert(expr, msg)
//#endif // 

// Esto a ver donde se pone!!!
#define NMem 10

// Para quitar el warning de que hay nombres de simbolos demasiado grandes
#pragma warning(disable:4786)

#endif //__BIOINCLUDES_H__

