// ControlAleatori.cpp: implementation of the CControlAleatori class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19991202 VoK - Procediment de proves. Dumping
// 19991202 VoK - Fix: Inicialitzacions
// 19991202 VoK - Fix: Mecanisme d'expressio (seguentInstruccio...)
// 20000218 VoK - Afegit el membre 'clear'
// 20000708 VoK - Tipus de control (per la funcio CControlOrg::tipus)
//////////////////////////////////////////////////////////////////////

#include <iomanip>
#include "turbioconio.h" 
#include "ControlAleatori.h"
#include "RandomStream.h"
#include "Configuracio.h"
#include "Color.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CControlAleatori::CControlAleatori()
{
	m_tipus+="/ExpressioGenica";
}

CControlAleatori::~CControlAleatori()
{

}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

uint32 CControlAleatori::seguentInstruccio(const uint32 * fenotip)
{
	return rnd.get();
}

bool CControlAleatori::init(const CCariotip & car)
{
	return true;
}

void CControlAleatori::clear() {
}

void CControlAleatori::dump(CMissatger & msg)
{
	msg << "ControlAleatori" << endl;
}

void CControlAleatori::ProvaClasse()
{
	using namespace AnsiCodes;
	out << clrscr;
	out << blanc.brillant() << "Provant ControlAleatori" << blanc.fosc() << endl;

	CCariotip cariotip;
	cariotip.init(8); // Creem un cariotip de 8 cromosomes
	cariotip.dump(out); // El mostrem
	cin.get();

	CControlAleatori genotip; // Creem un genotip
	genotip.init(cariotip); // L'omplim de la traduccio del cariotip
	genotip.dump(out); // El mostrem

	uint32 fenotip[16];
	for (uint32 i=16; i--;) fenotip[i]=0xFFFFFFFF<<i;

	while (cin.get()!='q') {
		uint32 instr = genotip.seguentInstruccio(fenotip);
		out << " " << hex << setw(8) << instr << dec << endl;
	}

	out << "Provant creacio masiva de genotips fins que premis una tecla" << endl;
	while (!kbhit()) {
		CCariotip cariotip;
		cariotip.init(8); // Creem un cariotip de 8 cromosomes
		CControlAleatori genotip; // Creem un genotip
		genotip.init(cariotip); // L'omplim de la traduccio del cariotip
		// Al final de la volta es destrueixen.
	}

	cin.get();
}
