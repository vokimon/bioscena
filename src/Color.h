// Color.h: interface for the CColor class.
//
//////////////////////////////////////////////////////////////////////
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
	if (c&0x80) {
		char *ansiseq = "\033[0;30;40m";
		ansiseq[2]=(c&0x08)?'1':'0';
		ansiseq[5]='0'+(c&0x07);
		ansiseq[8]='0'+((c>>4)&0x07);
		stream << ansiseq;
		}
	else {
		char *ansiseq = "\033[0;30m";
		ansiseq[2]=(c&0x08)?'1':'0';
		ansiseq[5]='0'+(c&0x07);
		stream << ansiseq;
	}
	return stream;
}
extern const CColor negre, vermell, verd, groc, blau, magenta, cyan, blanc;

namespace AnsiCodes {

	extern const char clrscr[]; // Clear screen
	extern const char clrlin[]; // Clear line (a partir de la posicio del cursor
	extern const char conrep[]; // Imprimeix l'estat del dispositiu

	string gotoxy(int col, int lin); // Posicionament del cursor
	string cursup(int lin); // Cursor Up
	string cursdn(int lin); // Cursor Down
	string cursfw(int col); // Cursor Forward
	string cursbw(int col); // Cursor Backward
	extern const char push_cursor[]; // Push Cursor Position
	extern const char pop_cursor[]; // Pop Cursor Position
	extern const char cursor_on[]; // Cursor ON
	extern const char cursor_off[]; // Cursor OFF
	extern const char key_on[]; // Key ON
	extern const char key_off[]; // Key OFF
	string set_mode(int mode); // Setmode
	string colorbg(int bg); // Color de fons
	string colorfg(int fg); // Color del davant
	string color(int fg, int bg); // Color del davant i fons
}
#endif // !defined(_KKEP_COLOR_H_INCLUDED)
