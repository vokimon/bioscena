// Outputer.cpp: implementation for the COutputer class and derivated.
//
//////////////////////////////////////////////////////////////////////
// Change Log
// 19990619 VoK - Creat i provat unitariament
// 19990719 VoK - Implemetat CColorStreamOutputer a partir d'una meitat de l'antic Missatger

#include "Outputer.h"
#include "Color.h"

static unsigned int col=35, fil=2;

/////////////////////////////////////////////////////////////////////
// CMemoryOutputer
/////////////////////////////////////////////////////////////////////

using namespace AnsiCodes;

void CMemoryOutputer::print(char *msg, char *caption) {
	m_llista.push_back(msg);
	if (m_llista.size()>m_longitud)
		m_llista.pop_front();
	m_stream << push_cursor << blanc.fosc();
	int cont=fil;
	if (caption) {
		m_stream << gotoxy(++cont,col) << clrlin;
		m_stream << caption << ": " << endl;
	}
	for (deque<string>::iterator it=m_llista.begin(); it!=m_llista.end(); it++)
	{
		m_stream << gotoxy(++cont,col) << clrlin << *it << endl; 
	}
	m_stream << pop_cursor;
//	m_stream.flush();
}


