/*
// conio.c - Implementacio d'algunes rutines compatibles amb la llibreria
//           CONIO.H del TurboC i Borland C.
//
// Copyright (C) 1998 Joan Garcia i Silano.
// Aquest codi es "freeware".
//
// Aquesta llibreria usa els caracters de control ANSI. Funciona si el
// terminal soporta aquests caracters de control.
*/

#ifndef __TURBOC__
#ifndef __DJGPP__
#ifndef _MSC_VER

#include "conio.h"
#include <stdio.h>
#include <unistd.h>
#include <termio.h>
#include <sys/time.h>
#include <sys/types.h>

#define STDIN_DESCRIPTOR	1
#define chESC			0x1B

#ifdef __linux__
	#define FDCAST (fd_set *)
#endif

struct termio old_term_options;
unsigned short color_table[] = {
	30, /* BLACK / DARKGRAY */
        34, /* BLUE / LIGHTBLUE */
        32, /* GREEN / LIGHTGREEN */
        36, /* CYAN / LIGHTCYAN */
        31, /* RED / LIGHTRED */
        35, /* MAGENTA / LIGHTMAGENTA */
        33, /* BROWN / YELLOW */
        37, /* LIGHTGRAY / WHITE */
};


void clrscr(void)
{
	printf("%c[2J%c[%i;%iH", chESC, chESC, 1, 1);
}


void gotoxy(int x, int y)
{
	printf("%c[%i;%iH", chESC, y, x);
}


void textcolor(int color)
{
	/* Si el color esta entre el 8 i el 16 activem la negreta; sino la
	   desactivem. */
	color %= 16;
	color < DARKGRAY ?
		printf("%c[0m%c[%dm", chESC, chESC, color_table[color]) :
		printf("%c[1m%c[%dm", chESC, chESC, color_table[color-8]);
}


void textbackground(int color)
{
	/* De fons nomes tenim del color 0 al color 7. */
	printf("%c[%dm", chESC, 10 + color_table[color % 8]);
}


void conio_set_term(void)
{
	struct termio new_term_options;

	new_term_options.c_lflag &= !ICANON;
	new_term_options.c_lflag &= !ECHO; 
	new_term_options.c_cc[VTIME] = 0;
	new_term_options.c_cc[VMIN] = 0;
	ioctl(STDIN_DESCRIPTOR, TCGETA, &old_term_options);
	ioctl(STDIN_DESCRIPTOR, TCSETA, &new_term_options);
}


void conio_restore_term(void)
{
	ioctl(STDIN_DESCRIPTOR, TCSETA, &old_term_options);
}


char getch(void)
{
	int desc = 1;
	char ch;

	conio_set_term();
	if (select(2, FDCAST &desc, NULL, NULL, NULL) == -1) ch = 0; else
	if (read(STDIN_DESCRIPTOR, &ch, sizeof(char)) != sizeof(char)) ch = 0;
	conio_restore_term();
	return ch;
}


int kbhit(void)
{
	struct timeval timeout = { 0, 0 };
	int desc = 1;

	conio_set_term();
	if (select(2, FDCAST &desc, NULL, NULL, &timeout) == -1) return 0;
	conio_restore_term();
	return desc;
}


void cursor_off(void)
{
	printf("%c[?25l", chESC);
}


void cursor_on(void)
{
	printf("%c[?25h", chESC);
}


void key_off(void)
{
	printf("%c[2h", chESC);
}


void key_on(void)
{
	printf("%c[2l", chESC);
}


#endif /* _MSC_VER */
#endif /* __DJGPP__ */
#endif /* __TURBOC__ */
