// Outputer.cpp: implementation for the COutputer class and derivated.
//
//////////////////////////////////////////////////////////////////////
// Change Log
// 19990619 VoK - Creat i provat unitariament
// 19990719 VoK - Implemetat CColorStreamOutputer a partir d'una meitat de l'antic Missatger

#include "Outputer.h"
#include "Color.h"

static unsigned int col=15, fil=2;

/////////////////////////////////////////////////////////////////////
// CMemoryOutputer
/////////////////////////////////////////////////////////////////////

using namespace AnsiCodes;

void CMemoryOutputer::print(char *msg, char *caption) {
/*	m_llista.push_back(msg);
	if (m_llista.size()>m_longitud)
		m_llista.pop_front();
	m_stream << push_cursor << blanc.fosc();
	int cont=fil;
	if (caption) {
		m_stream << gotoxy(col,++cont) << clrlin;
		m_stream << caption << ": " << endl;
	}
	for (deque<string>::iterator it=m_llista.begin(); it!=m_llista.end(); it++)
	{
		m_stream << gotoxy(col,++cont) << clrlin << *it << endl; 
	}
	m_stream << pop_cursor;
//	m_stream.flush();
*/
	static unsigned int pos=0;
	m_llista.push_back(msg);
	if (m_llista.size()>m_longitud)
		m_llista.pop_front();
	m_stream << push_cursor << blanc.fosc();
	m_stream << gotoxy(col,fil+pos) << clrlin << msg << endl;
	pos++; pos%=m_longitud;
	m_stream << gotoxy(col,fil+pos) << clrlin << endl;
	m_stream << pop_cursor;
//	m_stream.flush();

}


