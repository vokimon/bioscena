// Color.h: interface for the CColor class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990724 VoK - Fix: Tots els foscos es pintaven blanc: el 0 no es 
//                no brillant si no tot normal, color inclos.
// 19990724 VoK - Els colors estandars son 'const'.
//////////////////////////////////////////////////////////////////////
// TODO: Intermitencies colors de fons i xorrades d'aquestes que sempre
//       estas a temps de fer
#if !defined(_KKEP_COLOR_H_INCLUDED)
#define _KKEP_COLOR_H_INCLUDED


// 00SPBbbbFfff
class CColor
{
public:
	CColor() {m_val=0x17;};
	CColor(int color) {m_val=color;};
	virtual ~CColor() {};
	CColor brillant() const {return m_val|0x08;}
	CColor fosc() const {return m_val&~0x08;}
	CColor fons(int color) const {return (m_val&~0xF0)|((color&0x07)<<4)|0x80;}
	CColor& operator= (int i) {m_val=i; return *this;};
	operator int() const {return m_val&0x80?m_val:m_val&~0xF0;};
private:
	int m_val;
};

inline ostream& operator << (ostream& stream, CColor c)
{
	stream 
		<<"\033["<<((c&0x08)?1:0)
		<<";"<<(30+(c&0x07));
	if (c&0x80)
		stream <<";"<<(40+((c>>4)&0x07));
	stream <<"m";
	return stream;
}
extern const CColor negre, vermell, verd, groc, blau, magenta, cyan, blanc;

namespace AnsiCodes {

	extern const string clrscr; // Clear screen
	extern const string clrlin; // Clear line (a partir de la posicio del cursor
	extern const string conrep; // Imprimeix l'estat del dispositiu

	string gotoxy(int col, int lin); // Posicionament del cursor
	string cursup(int lin); // Cursor Up
	string cursdn(int lin); // Cursor Down
	string cursfw(int col); // Cursor Forward
	string cursbw(int col); // Cursor Backward
	extern const string push_cursor; // Push Cursor Position
	extern const string pop_cursor; // Pop Cursor Position
	string set_mode(int mode); // Setmode
}
#endif // !defined(_KKEP_COLOR_H_INCLUDED)
