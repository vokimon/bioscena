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

	#define BLACK		0
	#define BLUE		1
	#define GREEN		2
	#define CYAN		3
	#define RED		4
	#define MAGENTA		5
	#define BROWN		6
	#define LIGHTGRAY	7
	#define DARKGRAY	8
	#define LIGHTBLUE	9
	#define LIGHTGREEN	10
	#define LIGHTCYAN	11
	#define LIGHTRED	12
	#define LIGHTMAGENTA	13
	#define YELLOW		14
	#define WHITE		15

#ifdef __cplusplus
extern "C" {
#endif
	void	clrscr(void);
	void	gotoxy(int x, int y);
	void	textcolor(int color);
	void	textbackground(int color);
	int	kbhit(void);
	char	getch(void);

	/* Les rutines seguents no son compatibles amb Borland. */
	void	cursor_off(void);
	void	cursor_on(void);
	void	key_off(void);
	void	key_on(void);
#ifdef __cplusplus
};
#endif

#endif /* __CONIO_H__ */
#endif /* __TURBOC__ */

