// Color.cpp: implementation of the CColor class and AnsiCodes namespace.
//
//////////////////////////////////////////////////////////////////////

#include "BioIncludes.h"
#include "Color.h"

const CColor negre(0), vermell(1), verd(2), groc(3), 
	blau(4), magenta(5), cyan(6), blanc(7);


namespace AnsiCodes {
	// Si el buffer a on els ostrstreams fan el format no el proveim
	// nosaltres, en cridar al 'str' congelem el buffer dinamic intern
	// i el destructor no l'allibera creant lagunes de memoria
	static char myBuffer[16];

	string set_mode(int mode) // Setmode
	{
		ostrstream str(myBuffer,16);
		str << "\033[=" << mode << 'h' << ends;
		return str.str();
	}
	const string conrep ("\033[6n"); // Imprimeix l'estat del dispositiu
	const string clrscr ("\033[2J"); // Clear screen
	const string clrlin ("\033[K"); // Clear line (a partir de la posicio del cursor

	string gotoxy(int col, int lin) // Posicionament del cursor
	{
		ostrstream str(myBuffer,16);
		str << "\033[" << lin << ';' << col << 'H' << ends;
//		str << "\033[" << lin << ';' << col << 'f' << ends; // Equivalent a la H
		return str.str();
	}
	string cursup(int lin) // Cursor Up
	{
		ostrstream str(myBuffer,16);
		str << "\033[" << lin << 'A' << ends;
		return str.str();
	}
	string cursdn(int lin) // Cursor Down
	{
		ostrstream str(myBuffer,16);
		str << "\033[" << lin << 'B' << ends;
		return str.str();
	}
	string cursfw(int lin) // Cursor Forward
	{
		ostrstream str(myBuffer,16);
		str << "\033[" << lin << 'C' << ends;
		return str.str();
	}
	string cursbw(int lin) // Cursor Backward
	{
		ostrstream str(myBuffer,16);
		str << "\033[" << lin << 'D' << ends;
		return str.str();
	}
	const string push_cursor ("\033[s"); // Push Cursor Position
	const string pop_cursor ("\033[u"); // Pop Cursor Position
}

