// Missatger.cpp: implementation of the CMissatger class.
//
//////////////////////////////////////////////////////////////////////
// Change Log
// 19990623 VoK - Creat i provat unitariament
// 19990623 VoK - Afegit l'operador () per no haver de canviar codi
//                anterior del compilador.
// 19990623 VoK - Afegit el mecanisme de sumari per destruccio
// 19990630 VoK - Membre sumari per fer-ho explicitament quan es
//                una global que no se per que no crida al destructor.
// 19990718 VoK - Afegit a Bioscena: Eliminat el comptador de linies
// 19990719 VoK - Fa servir la classe abstracta Outputer
// 19990719 VoK - Implemetat CColorStreamOutputer a partir d'una meitat de l'antic Missatger
// 19990719 VoK - Ara el cpp te xixa (Donava uns problemes inmensos linkant al GCC
// 19990722 VoK - L'ordre dels parametres ha cambiat
// 19990722 VoK - Permetem no posar cap capcelera
// 19990722 VoK - Construits els objectes missatger globals (error, warning..)

#include "Missatger.h"

/////////////////////////////////////////////////////////////////////
// Els Missatgers per la meva aplicacio
/////////////////////////////////////////////////////////////////////

KKEP_DEFAULT_OUTPUTER(defaultOut);
CColorOutputer errorOutputer(cerr, 9);
CColorOutputer warningOutputer(cerr, 13);
CColorOutputer log1Outputer(clog, 15);
CColorOutputer log2Outputer(clog, 11);
CColorOutputer log3Outputer(clog, 10);
CColorOutputer log4Outputer(clog, 14);

// Els noms son curts per a ser facils d'usar, pero, es facil
// que hi hagi conflictes amb altres variables.

CMissatger error("Error", NULL, errorOutputer);
CMissatger warning("Warning", NULL, warningOutputer);
CMissatger out(NULL);
CMissatger log("Log");
CMissatger log1("\n>>>>>", NULL, log1Outputer);
CMissatger log2("--", NULL, log2Outputer);
CMissatger log3("  ", NULL, log3Outputer);
CMissatger log4("    ", NULL, log4Outputer);

/////////////////////////////////////////////////////////////////////
// Construccio/Desctruccio
/////////////////////////////////////////////////////////////////////

CMissatger::CMissatger(char * capcelera, char* final, CBasicOutputer& str)
	:m_out(str)
{
	m_hiHaCapcelera = (capcelera!=NULL);
	if (capcelera) strcpy(m_capcelera,capcelera);

	m_hiHaFinal = (final!=NULL);
	if (final) strcpy(m_final, final);

	m_msgCount=0;
	m_newMsg=true;
	m_activat=true;
}

CMissatger::~CMissatger()
// Aprofitem per generar el missatge de fi
{
	sumari();
}

/////////////////////////////////////////////////////////////////////
// Operacions (Activacio/Desactivacio)
/////////////////////////////////////////////////////////////////////
void CMissatger::sumari()
{
	if (m_msgCount&&m_hiHaFinal) {
		m_stream << m_final <<": " << m_msgCount;
		print("Sumari");
	}
}
void CMissatger::activa () {m_activat=true;}
void CMissatger::desactiva () {m_activat=false;}
bool CMissatger::activat () {return m_activat;}
/////////////////////////////////////////////////////////////////////
// Insercions
/////////////////////////////////////////////////////////////////////
CMissatger & CMissatger::operator<<(ostream& (*pf)(ostream&))
// Aquest membre serveix per interceptar els 'endl'
{
	if (!m_activat) return *this;
	if (pf==endl) { 
		m_newMsg=true;
		print(m_hiHaCapcelera?m_capcelera:NULL);
	}
	else 
		m_stream << pf;
	return *this;
}

void CMissatger::operator() (char * str)
// Je je, aixo permet no modificar el codi, nomes cal anomenar
// el missatger com abans la funcio d'error.
{
	(*this) << str;
	m_newMsg=true;
	print(m_hiHaCapcelera?m_capcelera:NULL);
}
/////////////////////////////////////////////////////////////////////
// Implementacio
/////////////////////////////////////////////////////////////////////
void CMissatger::print(char * capcelera) {
	m_stream << ends;
	m_out.print(m_stream.str(), capcelera);
	m_stream.rdbuf()->freeze(false);
	m_stream.seekp(0);	
}

/////////////////////////////////////////////////////////////////////
// Proves
/////////////////////////////////////////////////////////////////////
void CMissatger::ProvaClasse() {
	// Provant els predefinits
	error << "Provant error" << endl;
	warning << "Provant avis" << endl;
	out << "Salida" << endl;
	log << "Log no especialitzat" << endl;
	log1 << "Log de nivell 1" << endl;
	log2 << "Log de nivell 2" << endl;
	log3 << "Log de nivell 3" << endl;
	log4 << "Log de nivell 4" << endl;
	// En principi, no cal definir cal outputer
	// Un missatger portable (No cal canviar cap parametre especific)
	// Segons com definim el typedef COutputer pot ser un o un altre
	COutputer portableOut;
	CMissatger portable("Portable", "Missatges portables", portableOut);
	portable << "Yo sempre surto :-|" << endl;
	// Un missatger per a terminals ANSI amb suport de color
	CColorOutputer colorOut(cout, 12);
	CMissatger color("Color", "Missatges colorejats", colorOut);
	color << "Mira! Surto pintat!" << endl;
	// Aquest es un MessageBox de Windows, compte!
	#ifdef WIN32
	CBoxOutputer boxOut(MB_ICONEXCLAMATION);
	CMissatger box("Error a un dialeg", NULL, boxOut);
	box << "A que mai havies fet servir una MB com a stream?" << endl;
	#endif		
}

/////////////////////////////////////////////////////////////////////
// Main per proves unitaries
/////////////////////////////////////////////////////////////////////
/*
void main (void)
{
	CMissatger::ProvaClasse();
}
*/
