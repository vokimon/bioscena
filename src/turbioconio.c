/*
// conio.c - Implementacio d'algunes rutines compatibles amb la llibreria
//           CONIO.H del TurboC i Borland C.
//
// Copyright (C) 1998 Joan Garcia i Silano.
// Aquest codi es "freeware".
//
// Aquesta llibreria usa els caracters de control ANSI. Funciona si el
// terminal soporta aquests caracters de control.
///////////////////////////////////////////////////////////////////////
// No es la versio original, aquest fitxer ha estat retallat i adaptat
// a les meves necessitats, amb el permis d'en Joan.
//         David Garcia Garzon.
///////////////////////////////////////////////////////////////////////
*/

#ifdef POSIX

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

void conio_set_term(void)
{
	struct termio new_term_options;

	new_term_options.c_lflag &= !ICANON;
// VoK: Komentado por mi pa no desactivar el echo
//	new_term_options.c_lflag &= !ECHO; 
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

#endif /* POSIX */
