// Genotip.cpp: implementation of the CGenotip class.
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
#include "Genotip.h"
#include "RandomStream.h"
#include "Configuracio.h"
#include "Color.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGenotip::CGenotip()
{
	m_tipus+="/ExpressioGenica";
	m_genActual=0;
}

CGenotip::~CGenotip()
{

}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

uint32 CGenotip::seguentInstruccio(uint32 * fenotip)
{
	// Si no hi ha gens, ke hi farem
	if (!m_gens.size())
		return 0; //rnd.get();

	// Si s'ens ha acabat l'ultim, triem un de nou
	if (m_gens[m_genActual].finalitzat()) {
		int intents=Config.get("Organisme/Genotip/Traduibilitat/Intents");
		do {
			uint32 unGenAleatori=rnd.get(1,m_gens.size())-1;
			if (m_gens[unGenAleatori].traduible(fenotip)) {
				m_genActual=unGenAleatori;
				break;
			}
			m_gens[unGenAleatori].reset();
		} while (--intents);
		if (!intents) {
			return 0;
		}
	}

	// Tenim un gen amb zona operadora valida, pero, potser no funciona
	uint32 instr;
	if (!m_gens[m_genActual].seguentInstruccio(instr))
		return 0; //rnd.get();
	return instr;
}

bool CGenotip::init(CCariotip & car)
{
	for (uint32 crm=car.tamany(); crm--;) {
		if (!car[crm]) continue;
		for (uint32 pos=0; pos<car[crm]->tamany();) {
			CGen gen;
			if (gen.init(*(car[crm]),pos))
				m_gens.push_back(gen);
		}
	}
	return true;
}

void CGenotip::clear() {
	m_gens.clear();
}

void CGenotip::dump(CMissatger & msg)
{
	msg << "Genotip amb " << m_gens.size() << " gens" << endl;
	for (uint32 i=m_gens.size(); i--;) {
		msg << i << "-\t";
		m_gens[i].dump(msg);
	}

}

void CGenotip::ProvaClasse()
{
	using namespace AnsiCodes;
	out << clrscr;
	out << blanc.brillant() << "Provant Genotip" << blanc.fosc() << endl;

	CCariotip cariotip;
	cariotip.init(8); // Creem un cariotip de 8 cromosomes
	cariotip.dump(out); // El mostrem
	cin.get();

	CGenotip genotip; // Creem un genotip
	genotip.init(cariotip); // L'omplim de la traduccio del cariotip
	genotip.dump(out); // El mostrem

	uint32 fenotip[16];
	for (uint32 i=16; i--;) fenotip[i]=0xFFFFFFFF<<i;

	while (cin.get()!='q') {
		uint32 instr = genotip.seguentInstruccio(fenotip);
		genotip.m_gens[genotip.m_genActual].dump(out);
		out << " " << hex << setw(8) << instr << dec << endl;
	}

	out << "Provant creacio masiva de genotips fins que premis una tecla" << endl;
	while (!kbhit()) {
		CCariotip cariotip;
		cariotip.init(8); // Creem un cariotip de 8 cromosomes
		CGenotip genotip; // Creem un genotip
		genotip.init(cariotip); // L'omplim de la traduccio del cariotip
		// Al final de la volta es destrueixen.
	}

	cin.get();
}
