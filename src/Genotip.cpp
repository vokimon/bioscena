// Genotip.cpp: implementation of the CGenotip class.
//
//////////////////////////////////////////////////////////////////////

#include <iomanip>
#ifdef _MSC_VER
// El namespace es per solucionar un conflicte amb el gotoxy de AnsiCodes.h
namespace ops {
	#include <conio.h>
	}
#define kbhit ops::_kbhit
#else
#include <pc.h>
#endif

#include "Genotip.h"
#include "RandomStream.h"
#include "Configuracio.h"
#include "Color.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGenotip::CGenotip()
{
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
			m_genActual=rnd.get(1,m_gens.size())-1;
		} while (!m_gens[m_genActual].traduible(fenotip) && --intents);
	}

	// Pot ser no hem trobat cap de bo
	if (m_gens[m_genActual].finalitzat())
		return 0; //rnd.get();
	
	// Finalment la podem agafar, pero, potser no funciona
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
