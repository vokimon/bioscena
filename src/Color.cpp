// Color.cpp: implementation of the CColor class and AnsiCodes namespace.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990724 VoK - Fix: Tots els foscos es pintaven blanc: el 0 no es 
//                no brillant si no tot normal, color inclos.
// 19990724 VoK - Els colors estandars son 'const'.
// 199908?? VoK - Creats els AnsiCodes
// 19991214 VoK - Optimitzat el serialitzat dels CColor
// 19991214 VoK - Afegides: colorbgfg & co. cursor_on/off i key_on/off
//////////////////////////////////////////////////////////////////////
// TODO: Intermitencies colors de fons i xorrades d'aquestes que sempre
//       estas a temps de fer
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
	const char conrep[] = "\033[6n"; // Imprimeix l'estat del dispositiu
	const char clrscr[] = "\033[2J"; // Clear screen
	const char clrlin[] = "\033[K"; // Clear line (a partir de la posicio del cursor

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
	const char push_cursor[] = "\033[s"; // Push Cursor Position
	const char pop_cursor[] = "\033[u"; // Pop Cursor Position

	// Aquests quatre no se ni que fan ni qui els soporta
	const char cursor_on[] = "\033[?25h"; // Cursor ON
	const char cursor_off[] = "\033[?25l"; // Cursor OFF
	const char key_on[] = "\033[2l"; // Key ON
	const char key_off[] = "\033[25h"; // Key OFF

	const char reset_color[] = "\033[0m"; // Posa el color normal
	// Pot ser tornar un string es poc optim pero ho fem multithread
	string color (int fg, int bg)
	{
		string ansiseq ("\033[0;30;40m");
		ansiseq[2]=(fg&0x08)?'1':'0';
		ansiseq[5]='0'+(fg&0x07);
		ansiseq[8]='0'+(bg&0x07);
		return ansiseq;
	}
	string colorfg (int fg)
	{
		string ansiseq ("\033[0;30m");
		ansiseq[2]=(fg&0x08)?'1':'0';
		ansiseq[5]='0'+(fg&0x07);
		return ansiseq;
	}
	string colorbg (int bg)
	{
		string ansiseq ("\033[40m");
		ansiseq[3]='0'+(bg&0x07);
		return ansiseq;
	}
}

