// Missatger.h: interface for the CMissatger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_MISSATGER_H_INCLUDED)
#define __KKEP_MISSATGER_H_INCLUDED

#include <strstream>
#include <iostream>
#include "Outputer.h"


using namespace std;

class CMissatger  
{
// Construccio
public:
	CMissatger(char * capcelera=NULL, char* final=NULL, 
		CBasicOutputer& str=defaultOut);
	virtual ~CMissatger();
	void sumari();

// Operacions (Activacio/Desactivacio)
public:
	void activa ();
	void desactiva ();
	bool activat ();
// Insercions
public:
	CMissatger & operator<<(ostream& (*pf)(ostream&));
	void operator() (char * str);
	template<class T> CMissatger & operator<<(T object)
	{
		if (!m_activat) return *this;
		if (m_newMsg)
		{
			m_newMsg = false;
			m_msgCount++;
		}
		m_stream << object;
		return *this;
	};
// Atributs
protected:
	CBasicOutputer &m_out;
	ostrstream m_stream;
	bool m_hiHaCapcelera; // A true si hem definit una capcelera
	char m_capcelera[40]; // Guarda una capcelera comuna a aquests missatges
	bool m_hiHaFinal; // A true si hem definit un missatge de sumari.
	char m_final[70]; // Missatge de sumari (pe: "El numero de errors es")
	bool m_newMsg; // A true quan no hem estrenat la linia.
	int m_msgCount; // Compta els missatges enviats d'aquest tipus
	bool m_activat; // A true vol dir que fa traces
// Implementacio
private:
	void print(char * capcelera);
// Proves
public:
	static void ProvaClasse();
};

// Missatgers de la aplicació
extern CMissatger error,warning,out,log,log1,log2,log3,log4,mem;

#endif // !defined(__KKEP_MISSATGER_H_INCLUDED)
