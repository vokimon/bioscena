#ifndef __BIOINCLUDES_H__
#define __BIOINCLUDES_H__

#include <iostream>
#include <assert.h>
//#include "Missatger.h"
#include "Portable.hxx"
#include "Assert.hxx"
#if 0
// Definim el nostre assert
//#if (defined(_MSVC)&&defined(_DEBUG))||(!defined(_MSVC)&&!defined(NDEBUG)
#define KKEPAssert(expr, msg) ((expr)||((error<<__FILE__<<":"<<__LINE__<<":"<<msg<<endl) ,true))
//#else
//#define KKEPAssert(expr, msg)
//#endif // 
#endif

// Para quitar el warning de que hay nombres de simbolos demasiado grandes
#ifdef  _MSC_VER
	#pragma warning(disable:4786)
#endif /* _MSC_VER */

#endif //__BIOINCLUDES_H__

