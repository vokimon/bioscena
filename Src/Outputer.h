// Outputer.h: interface for the COutputer class.
//
//////////////////////////////////////////////////////////////////////
// Change Log
// 19990619 VoK - Creat i provat unitariament
// 19990719 VoK - Implemetat CColorStreamOutputer a partir d'una meitat de l'antic Missatger

#if !defined(__KKEP_OUTPUTER_H_INCLUDED)
#define __KKEP_OUTPUTER_H_INCLUDED

#ifdef  _MSC_VER
	#pragma warning(disable:4786)
#endif /* _MSC_VER */

#include <iostream>
#include <deque>
#include <string>

// Aquesta definicio es la declaracio del COutputer per defecte.
// Si vols una diferent defineix-la a fora abans de cridar aquest fitxer
#ifndef KKEP_DEFAULT_OUTPUTER
#define KKEP_DEFAULT_OUTPUTER(varname) CColorOutputer varname
//#define KKEP_DEFAULT_OUTPUTER(varname) CColorOutputer varname(cerr,9)
//#define KKEP_DEFAULT_OUTPUTER(varname) CBoxOutputer varname 
//#define KKEP_DEFAULT_OUTPUTER(varname) CBoxOutputer varname (MB_ICONERROR)
#endif

#ifdef WIN32
#include <windows.h>
#endif

using namespace std;

/////////////////////////////////////////////////////////////////////
// CBasicOutputer
/////////////////////////////////////////////////////////////////////
class CBasicOutputer {
public:
	CBasicOutputer() {};
	virtual void print(char *msg, char *caption)=0;
};

/////////////////////////////////////////////////////////////////////
// CColorOutputer
/////////////////////////////////////////////////////////////////////
class CColorOutputer: public CBasicOutputer {
private:
	ostream & m_stream;
	int m_color; // Color d'aquests missatges
public:
	CColorOutputer(ostream &str=cout, int color=7)
		: m_stream(str), m_color(color) 
	{}
	virtual void print(char *msg, char *caption) {
		posaColor();
		if (caption) m_stream << caption << ": ";
		m_stream << msg;
		treuColor();
		// L'ultim return cal fer-ho aqui pq a alguns 
		// COutputers els fa nosa
		m_stream << endl; 
	}
private:
	void posaColor()
	{
		if (m_color!=7)
			m_stream 
				<< "\033["
				<< ((0x08&m_color)?1:0) <<";" 
				<< ((m_color&0x07)+30) << "m";
	}
	void treuColor()
	{
		if (m_color!=7)
			m_stream << "\033[0;37m";
	}
};

/////////////////////////////////////////////////////////////////////
// CMemoryOutputer
/////////////////////////////////////////////////////////////////////
class CMemoryOutputer: public CBasicOutputer {
private:
	ostream & m_stream;
	deque<string> m_llista;
	unsigned int m_longitud; // Longitud maxima de la llista
public:
	CMemoryOutputer(ostream &str=clog, unsigned int longitud=7)
		: m_stream(str)
	{
		m_longitud=longitud;
	}
	virtual ~CMemoryOutputer(){};
	virtual void print(char *msg, char *caption);
};

/////////////////////////////////////////////////////////////////////
// CBoxOutputer
/////////////////////////////////////////////////////////////////////
// Un Outputer mediante un cuadro de dialogo para windows
#ifdef _WIN32
class CBoxOutputer : public CBasicOutputer {
	unsigned int m_estil;
public:
	CBoxOutputer(unsigned int icona = MB_ICONINFORMATION){
		m_estil = MB_OK | MB_TOPMOST | (icona&MB_ICONMASK);
	};
	virtual void print(char *msg, char *caption) {
		MessageBox(NULL, msg, caption, m_estil);
	}
};

/////////////////////////////////////////////////////////////////////
// CEditOutputer
/////////////////////////////////////////////////////////////////////
// Un Outputer para un control Edit de Windows
// TODO: Provar-ho, es clar
#ifdef _MFC_VER
class CEditOutputer : public CBoxOutputer {
	CEdit *m_editControl;
public:
	CEditOutputer(CEdit *edit=NULL){
		m_editControl = edit;
	};
	virtual void print(char *msg, char *caption) {
		// Potser volem enviar un missatge abans de crear el control
		// o, si s'ens ha colat attachar-ho que no peti
		if (!m_editControl) {
			CBoxOutputer::print(msg,caption);
			return;
		}
		// Si si que esta attachat, doncs a la aventura
		CString strTemp = "";
		if (caption) {
			strTemp += caption;
			strTemp += ": ";
		}
		strTemp += msg;
		strTemp += _T("\r\n");
		m_editControl->SetSel(-1,-1);
		m_editControl->ReplaceSel(strTemp);
		m_editControl->ReplaceSel(strTemp);
	}
};
#endif // _MFC_VER
#endif // _WIN32

/////////////////////////////////////////////////////////////////////
// CCursesOutputer
/////////////////////////////////////////////////////////////////////
// TODO: Un Outputer para curses


/////////////////////////////////////////////////////////////////////
// COutputer: Alias per al que s'escull per defecte
/////////////////////////////////////////////////////////////////////

// No pretenc que aixo ja valgui per a tot. Caldria definir-ho
// d'una forma mes elaborada
#ifdef _WIN32
#ifdef _MFC_VER
typedef CEditOutputer COutputer;
#else 
typedef CBoxOutputer COutputer;
#endif // _MFC_VER
#else
typedef CColorOutputer COutputer;
#endif // _WIN32

extern KKEP_DEFAULT_OUTPUTER(defaultOut);

#endif // !defined(__KKEP_OUTPUTER_H_INCLUDED)
