// Cella.h: interface for the CCella class.
//
//////////////////////////////////////////////////////////////////////
// TODO: Intermitencies colors de fons i xorrades d'aquestes que sempre
//       estas a temps de fer
#if !defined(_KKEP_CELLA_H_INCLUDED)
#define _KKEP_CELLA_H_INCLUDED

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CColor
{
public:
	CColor() {m_val=0x17;};
	CColor(int color) {m_val=color;};
	virtual ~CColor(){};
	CColor brillant() {return m_val|0x08;}
	CColor fosc() {return m_val&~0x08;}
	CColor fons(int color) {return (m_val&~0xF0)|((color&0x07)<<4)|0x80;}
	CColor& operator= (int i) {m_val=i; return *this;};
	operator int() {return m_val&0x80?m_val:m_val&~0xF0;};
private:
	int m_val;

};
inline ostream& operator << (ostream& aOut, CColor c)
{
	aOut <<"\033["<<(30+(c&0x07))<<";";
	if (c&0x80)
		aOut <<(40+((c>>4)&0x07))<<";";
	aOut <<((c&0x08)?1:0)<<"m";
	return aOut;
}
extern CColor negre, vermell, verd, groc, blau, magenta, cian, blanc;

#endif // !defined(_KKEP_CELLA_H_INCLUDED)
