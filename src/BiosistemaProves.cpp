// Biosistema.cpp: implementation of the CBiosistema class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990831 VoK - Prototips instruccions organismes
// 19990831 VoK - Ja tenim taxonomista per natros sols
// 19991120 VoK - Implementades les operacions de fenotip
// 19991122 VoK - Proves: Control de teclat + estat visualitzacio
// 19991128 VoK - Els opcodes es llegeixen d'un fitxer
// 19991130 VoK - Implementades les operacions sensorials quimiques
// 19991130 VoK - Reorganitzada la classe
// 19991205 VoK - Implementades les operacions sensorials presencia
// 19991210 VoK - Traspasats des d'Organisme.cpp els costos d'engolir, 
//                excretar i atacar
// 19991211 VoK - Traspasats des d'Organisme.cpp els costos de mitosi
//                i generacio expontanea
// 19991214 VoK - Incorporada kbhit portable
// 19991214 VoK - Funcio 'nibble' per fer mes llegible el codi.
// 19991220 VoK - Afegides operacions de Shift
// 20000103 VoK - Sensibilitat interna
// 20000215 VoK - La feina de taxonomista es fa amb l'objecte extern
// 20000528 VoK - Adaptats els canvis dels grafix amb aquesta data
//////////////////////////////////////////////////////////////////////
// Notes de manteniment:
// - m_infoOrganisme apunta a un element d'un vector. Com a tal es pot
//   invalidar en inserir o eliminar un element. Aixi s'ha obviat la
//   actualitzacio en eliminar doncs sempre s'elimina l'actual i cal 
//   actualitzar despres la variable amb un altre valor. 
// - Es suposa arreu que canviaActual deixa les variables 'actuals'
//   a un valor valid. (Sempre existeix un element com a minim)
//////////////////////////////////////////////////////////////////////

#include <fstream>

#include "turbioconio.h"
#include "Biosistema.h"
#include "Configuracio.h"
#include "Grafic.h"
#include "TopologiaToroidal.h"
#include <sys/ioctl.h>

using namespace AnsiCodes;

class TerminalController {
private:
	uint32 _w;
	uint32 _h;

	void takeFromTerminal()
	{
		struct winsize size;
		ioctl(0, TIOCGWINSZ, &size);
		_w = size.ws_col;
		_h = size.ws_row;
	}

public:
	TerminalController()
	: _w(80)
	, _h(25)
	{
		takeFromTerminal();
	}
	uint32 height() const
	{
		return _h;
	}
	uint32 width() const
	{
		return _w;
	}

};

static void importaOrganisme(CBiosistema & biosistema)
{
	char filename[300];
	out << clrscr << endl;
	if (!cin.get(filename,300)) {
		cin.clear();
		return;
	}
	out << "Important l'organisme del fitxer '" << filename << '\'' << endl;
	COrganisme * org = new COrganisme("Control/ExpressioGenica");
	if (!org) {
		error << "Error de memoria" << endl;
		cin.get();
		return;
	}
	ifstream file(filename, ios::binary|ios::in);
	if (!file) {
		delete org;
		error << "Error obrint '" << filename << "'" << endl;
		cin.get();
		return;
	}
	org->load(file);
	file.close();
	if (!biosistema.introdueix(org)) {
		error << "Error introduint l'organisme" << endl;
		delete org;
		cin.get();
		return;
	}
	out << "Prem una tecla" << endl;
	cin.get();
}

static void exportaOrganisme(CBiosistema & biosistema)
{
	char filename[300];
	uint32 id;
	cin >> oct >> id >> dec;
	if (!cin) {
		cin.clear();
		return;
	}
	if (!cin.getline(filename,300)) {
		cin.clear();
		return;
	}
	out << clrscr << endl;
	out << "Exportant l'organisme " << oct << id << dec << " al fitxer '" << filename << '\'' << endl;
	ofstream file(filename, ios::binary|ios::out);
	if (!file) {
		error << "Error obrint '" << filename << "'" << endl;
		cin.get();
		return;
	}
	COrganisme * org = (*biosistema.comunitat())[id].cos();
	org->store(file);
	file.close();
	out << "Prem una tecla" << endl;
	cin.get();
}

static void exportaBiosistema(CBiosistema & biosistema)
{
	char filename[300];
	if (!cin.getline(filename,300)) {
		cin.clear();
		return;
	}
	out << clrscr << endl;
	out << "Exportant el biosistema al fitxer '" << filename << '\'' << endl;
	ofstream file(filename, ios::binary|ios::out);
	if (!file) {
		error << "Error obrint '" << filename << "'" << endl;
		cin.get();
		return;
	}
	biosistema.store(file);
	file.close();
	out << "Prem una tecla" << endl;
	cin.get();
}

static void importaBiosistema(CBiosistema & biosistema)
{
	char filename[300];
	if (!cin.getline(filename,300)) {
		cin.clear();
		return;
	}
	out << clrscr << endl;
	out << "Important el biosistema del fitxer '" << filename << '\'' << endl;
	ifstream file(filename, ios::binary|ios::in);
	if (!file) {
		error << "Error obrint '" << filename << "'" << endl;
		cin.get();
		return;
	}
	biosistema.load(file);
	biosistema.canviaOrganismeActiu();

	file.close();
	out << "Prem una tecla" << endl;
	cin.get();
}

//////////////////////////////////////////////////////////////////////
// Variables estatiques
//////////////////////////////////////////////////////////////////////

static CMemoryOutputer unLog(clog, 40);
static CMissatger logAccio("Accions realitzades", NULL, unLog);


//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

static char TextAjuda[] =
	"Ajuda de consola de Bioscena\n" 
	"****************************\n\n" 
	"Aquestes tecles son disposables durant l'execucio del prototip.\n"
	"Si prems qualsevol tecla/comanda el programa es para fins que prems el retorn.\n"
	"\nGeneral:\n\n"
	"  H      Per tornar a veure aquesta pantalla d'ajuda\n"
	"  Q      Surt del programa\n"
	"\nVisualitzacio:\n\n" 
	"  M      Visualitza el mapa (amb Shift ho treu)\n" 
	"  O      Visualitza comparativa organismes (amb Shift la treu)\n"
	"  T      Visualitza comparativa taxons (amb Shift la treu)\n"
	"  L      Visualitza les accions dels organismes una per una (Amb Shift ho treu)\n"
	"  V+Num  Visualitza l'interior de l'organisme especificat\n"
	"  J+Num  Es salta 'Num' pasos entre visualitzacio i visualitzacio\n"
	"\nNavegacio Mapa:\n\n"
	"  W      1 posicio adalt (amb Shift 1 pantalla sencera)\n"
	"  S      1 posicio abaix (amb Shift 1 pantalla sencera)\n"
	"  D      1 posicio a la dreta (amb Shift 1 pantalla sencera)\n"
	"  A      1 posicio a l'esquerra (amb Shift 1 pantalla sencera)\n"
	"\nActuacio sobre el biosistema:\n\n"
	"  r          Carrega el fitxer standard de configuracio d'agents externs\n"
	"  RNomFitxer Carrega el fitxer de configuracio d'agents externs\n"
	"  CNomFitxer Exporta un organisme al fitxer especificat\n"
	"  cNomFitxer Importa un organisme des del fitxer especificat\n"
	"  XNomFitxer Exporta el biosistema al fitxer especificat\n"
	"  xNomFitxer Importa el biosistema des del fitxer especificat\n"
	"  e          Discrimina el taxo dels organismes mutants\n"
	"  E          No discrimina el taxo dels organismes mutants\n"
	"  P          Congela el biosistema (amb Shift ho descongela)\n"
	"\n  Premi return per a continuar amb el programa\n"
	"\n  Copirait KKEPerians UNLTD\n";

inline static uint32 agafaEdatOrganisme (CInfoOrganisme & info) {
	return info.cos()->edat();
	}
inline static uint32 agafaEnergiaOrganisme (CInfoOrganisme & info) {
	return info.cos()->energia();
	}
inline static uint32 agafaCarregaOrganisme (CInfoOrganisme & info) {
	return info.cos()->carrega();
	}
inline static uint32 agafaCensTaxo (CInfoTaxo & info) {
	return info.cens;
	}
inline static uint32 agafaTotalTaxo (CInfoTaxo & info) {
	return info.morts+info.cens;
	}

void CBiosistema::ProvaClasse()
{
	TerminalController terminal;
//	out << clrscr;
	out << blanc.brillant() << "Provant Biosistema" << blanc.fosc() << endl;

	Config.parsejaArxiu("Bioscena.ini", error);
	CBiosistema biosistema;
	biosistema.carregaOpCodes("Opcodes.ini", error);
	out << "Inicialitzant Biotop..." << endl;
	Bioscena::Topology * topologia = new Bioscena::Torus(Config.get("Biotop/CasellesAmplitud"),Config.get("Biotop/CasellesAltitud"));
	biosistema.biotop(new Bioscena::CBiotop<CBiosistema::t_substrat>(topologia));
	biosistema.comunitat(new CComunitat);
	biosistema.taxonomista(new CTaxonomista);
	biosistema.agents(CAgent::ParsejaArxiu("Agents.ini", biosistema, error));

	uint32 pasVisualitzacio=0;
	uint32 pas=pasVisualitzacio;

	enum {Blanc, Mapa, MapaOrganismes, Organismes, Taxons, MapaTaxons} mode 
		= MapaOrganismes;
	bool modeCanviat=true;
	CComparativaOrganismes grafO[5];
	CDominiGraficaComparativa<CInfoOrganisme> 
		dominiEnergia, dominiEdat, dominiCarrega;
	dominiEdat.funcio(agafaEdatOrganisme);
	dominiEdat.representacio('-','?');
	dominiEnergia.funcio(agafaEdatOrganisme);
	dominiEnergia.representacio('*','!');
	dominiEnergia.logaritmic(true);
	dominiCarrega.funcio(agafaCarregaOrganisme);
	dominiCarrega.representacio('+','^');
	dominiCarrega.logaritmic(false);
	for (int i=5; i--;) {
		grafO[i].comunitat(biosistema.comunitat());
		grafO[i].afegeixDomini(dominiEdat);
		grafO[i].afegeixDomini(dominiEnergia);
//		grafO[i].afegeixDomini(dominiCarrega);
		}

	CComparativaTaxons grafT[5];
	CDominiGraficaComparativa<CInfoTaxo> dominiCens, dominiTotal;
	dominiCens.funcio(agafaCensTaxo);
	dominiCens.representacio('-','?');
	dominiTotal.funcio(agafaTotalTaxo);
	dominiTotal.representacio('+','?');
	for (int i=5; i--;) {
		grafT[i].taxonomista(biosistema.taxonomista());
		grafT[i].afegeixDomini(dominiCens);
		grafT[i].afegeixDomini(dominiTotal);
		}
	
	CMapa mapa1(&biosistema);

	mapa1.primeraPosicio(0);
	bool helpWanted=true;
	bool editWanted=false;
	bool redisplayWanted=false;
	bool congelat=false;
	uint32 id=0;
	std::cout << endl << "Biosistema carregat. Prem return per continuar..." << endl;
	cin.get();

	uint32 width = terminal.width();
	uint32 height = terminal.height();

	while (true) {
		// Actualitzem el mode de visualitzacio
		if (modeCanviat) {
			std::cout << clrscr;
			switch (mode) {
			case Mapa:
			default:
				mapa1.tamany(1,2,width,45); 
				break;
			case MapaOrganismes:
				mapa1.tamany(1,2,width-42,40);
				for (uint32 i=0; i<5; i++) {
					grafO[i].tamany(width-40,1+i*10,40,8);
					grafO[i].inici(i*40);
				}
			case MapaTaxons:
				mapa1.tamany(1,2,width-42,40);
				for (uint32 i=0; i<5; i++) {
					grafT[i].tamany(width-40,1+i*10,40,8);
					grafT[i].inici(i*40);
				}
				break;
			case Organismes:
				for (uint32 i=0; i<4; i++) {
					grafO[i].tamany(1,2+i*10,width,8);
					grafO[i].inici(i*width);
				}
			case Taxons:
				for (uint32 i=0; i<4; i++) {
					grafT[i].tamany(1,2+i*10,width,8);
					grafT[i].inici(i*width);
				}
			}
			modeCanviat=false;
			redisplayWanted=true;
		}

		// Fem un pas del biosistema
		if (!congelat) {
			biosistema();
			if (!biosistema.temps() && !pas--) {
				pas=pasVisualitzacio;
				redisplayWanted=true;
			}
		}

		// Visualitzem
		if (redisplayWanted) {
			redisplayWanted=false;
			if (mode==Mapa||mode==MapaOrganismes||mode==MapaTaxons)
				mapa1.visualitza(std::cout);
			if (mode==MapaOrganismes||mode==Organismes) {
				grafO[0].visualitza(std::cout);
				grafO[1].visualitza(std::cout);
				grafO[2].visualitza(std::cout);
				grafO[3].visualitza(std::cout);
				grafO[4].visualitza(std::cout);
			}
			if (mode==MapaTaxons||mode==Taxons) {
				grafT[0].visualitza(std::cout);
				grafT[1].visualitza(std::cout);
				grafT[2].visualitza(std::cout);
				grafT[3].visualitza(std::cout);
				grafT[4].visualitza(std::cout);
			}
			uint32 pos = mapa1.primeraPosicio();
			std::cout << setfill('0');
			if (mode==MapaOrganismes||mode==MapaTaxons) {
				std::cout << gotoxy(1,42) 
					<< "Coords: " << setw(3) << pos%Config.get("Biotop/CasellesAmplitud") 
					<< '@' << setw(3) << pos/Config.get("Biotop/CasellesAmplitud") 
					;
				}
			else {
				std::cout << gotoxy(42,1) 
					<< "Coords: " << setw(3) << pos%Config.get("Biotop/CasellesAmplitud") 
					<< '@' << setw(3) << pos/Config.get("Biotop/CasellesAmplitud") 
					;
				}
			std::cout << gotoxy(1, 1) << "Poblacio: " << setw(6) << biosistema.comunitat()->tamany();
			std::cout << gotoxy(26,1) << "Temps: " << setw(8) << biosistema.m_temps;
			std::cout << setfill(' ') << endl;
		}
		char filename[300];
		// Obtenim l'entrada del teclat
		if (kbhit())
		{
			cout << gotoxy(1,48) << clrlin << "Comanda: " << flush;
			bool sortir=false;
			while (!sortir)
			{
				char a = cin.get();
				if (!cin) break;
				switch (a) 
				{
				case 'D': mapa1.scrollPageRight(1); redisplayWanted=true; break;
				case 'd': mapa1.scrollRight(1); redisplayWanted=true; break;
				case 'A': mapa1.scrollPageLeft(1); redisplayWanted=true; break;
				case 'a': mapa1.scrollLeft(1); redisplayWanted=true; break;
				case 'W': mapa1.scrollPageUp(1); redisplayWanted=true; break;
				case 'w': mapa1.scrollUp(1); redisplayWanted=true; break;
				case 'S': mapa1.scrollPageDown(1); redisplayWanted=true; break;
				case 's': mapa1.scrollDown(1); redisplayWanted=true; break;
				case 'M': 
					modeCanviat = true;
					if (mode==MapaOrganismes) mode=Organismes;
					else if (mode==MapaTaxons) mode=Taxons;
					else if (mode==Mapa) mode=Blanc;
					else modeCanviat = false;
					break;
				case 'm': 
					modeCanviat = true;
					if (mode==Organismes) mode=MapaOrganismes;
					else if (mode==Taxons) mode=MapaTaxons;
					else if (mode==Blanc) mode=Mapa;
					else modeCanviat = false;
					break;
				case 'O': 
					modeCanviat = true;
					if (mode==MapaOrganismes) mode=Mapa;
					else if (mode==Organismes) mode=Blanc;
					else modeCanviat = false;
					break;
				case 'o': 
					modeCanviat = true;
					if (mode==Mapa) mode=MapaOrganismes;
					else if (mode==Blanc) mode=Organismes;
					else if (mode==Taxons) mode=Organismes;
					else if (mode==MapaTaxons) mode=MapaOrganismes;
					else modeCanviat = false;
					break;
				case 'T': 
					modeCanviat = true;
					if (mode==MapaTaxons) mode=Mapa;
					else if (mode==Taxons) mode=Blanc;
					else modeCanviat = false;
					break;
				case 't': 
					modeCanviat = true;
					if (mode==Mapa) mode=MapaTaxons;
					else if (mode==Blanc) mode=Taxons;
					else if (mode==Organismes) mode=Taxons;
					else if (mode==MapaOrganismes) mode=MapaTaxons;
					else modeCanviat = false;
					break;
				case 'Q': 
				case 'q': cin.clear(); return;
				case 'H': 
				case 'h': helpWanted=true; break;
				case 'L': modeCanviat = true; biosistema.activaLog(false); break;
				case 'l': modeCanviat = true; biosistema.activaLog(true); break;
				case 'R': 
					if (!cin.get(filename,300)) {
						cin.clear();
						break;
					}
					std::cout << clrscr << endl;
					biosistema.deleteAgents();
					biosistema.agents(CAgent::ParsejaArxiu(filename, biosistema, error));
					// TODO: Recarregar configuracio 
					std::cout << "Prem una tecla" << endl;
					cin.get();
					modeCanviat = true;
					break;
				case 'r': 
					std::cout << clrscr << endl;
					biosistema.deleteAgents();
					biosistema.agents(CAgent::ParsejaArxiu("Agents.ini", biosistema, error));
					// TODO: Recarregar configuracio 
					std::cout << "Prem una tecla" << endl;
					cin.get();
					modeCanviat = true;
					break;
				case 'x':
					importaBiosistema(biosistema);
					modeCanviat = true;
					break;
				case 'X':
					exportaBiosistema(biosistema);
					modeCanviat = true;
					break;
				case 'c': 
					importaOrganisme(biosistema);
					modeCanviat = true;
					break;
				case 'C': 
					exportaOrganisme(biosistema);
					modeCanviat = true;
					break;
				case 'e': biosistema.taxonomista()->activaEspeciacio(true); break;
				case 'E': biosistema.taxonomista()->activaEspeciacio(false); break;
				case 'V':
				case 'v':
					cin >> oct >> id >> dec;
					if (cin) editWanted=true;
					cin.clear();
					break;
				case 'p': congelat=true; break;
				case 'P': congelat=false; break;
				case 'J': 
				case 'j':
					cin >> pasVisualitzacio;
					if (cin) pas = pasVisualitzacio;
					cin.clear();
					break;
				case '\0':
					warning << "Valla, el \\0!!!" << endl;
					cin.get();
				case '\n': sortir=true;
					break;
				}
				// std::cout << gotoxy(20,48) << "Tecla: " << '\'' << (a?a:'#') << '\''<< endl;
			}
		}
		if (helpWanted) {
			helpWanted=false;
			cout << clrscr << TextAjuda << endl;
			cin.get();
			modeCanviat = true;
		}
		if (editWanted) {
			editWanted=false;
			out << clrscr << endl;
			if (biosistema.comunitat()->esValid(id)) {
				COrganisme * org = (*biosistema.comunitat())[id].cos();
				uint32 pos = (*biosistema.comunitat())[id].posicio();
				out << (*biosistema.comunitat())[id].descripcio() << '\t' 
					<<  pos%Config.get("Biotop/CasellesAmplitud")<< '@' <<  pos/Config.get("Biotop/CasellesAmplitud") 
					<< " Edat:" << (*biosistema.comunitat())[id].cos()->edat()
					<< " Energia: " << (*biosistema.comunitat())[id].cos()->energia()
					<< endl;
				org->dump(out);
			}
			else out << "L'organisme ha mort o mai ha existit" << endl;
			cin.get();
			modeCanviat = true;
		}
	}
}


