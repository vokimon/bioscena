/*
// conio.h - Algunes macros compatibles amb la llibreria CONIO.H del
//           TurboC i Borland C.
//
// Versio 1.0.0 - 13 Set 1998
// Copyright (C) 1998 Joan Garcia i Silano.
// Aquest codi es "freeware".
*/

#if defined _MSC_VER || defined __TURBOC__ || defined __DJGPP__
// El namespace es per solucionar un conflicte amb el gotoxy de AnsiCodes.h
namespace ops {
	#include <conio.h>
	}
#if defined _MSC_VER 
#define kbhit ops::_kbhit
#else
using ops::kbhit;
#endif

#else

	#ifndef __CONIO_H__
	#define __CONIO_H__


#ifdef __cplusplus
extern "C" {
#endif
	int	kbhit(void);
	char	getch(void);
#ifdef __cplusplus
};
#endif

#endif /* __CONIO_H__ */
#endif /* __TURBOC__ */

