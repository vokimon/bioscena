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

using namespace AnsiCodes;

static void importaOrganisme(CBiosistema & biosistema)
{
	char filename[300];
	out << clrscr << endl;
	if (!cin.get(filename,300)) {
		cin.clear();
		return;
	}
	out << "Important l'organisme del fitxer '" << filename << '\'' << endl;
	COrganisme * org = new COrganisme;
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
	"  L      Visualitza les accions dels organismes una per una (Amb Shift ho treu)\n"
	"  V+Num  Visualitza l'interior de l'organisme especificat\n"
	"  J+Num  Es salta 'Num' pasos entre visualitzacio i visualitzacio\n"
	"\nNavegacio Mapa:\n\n"
	"  W      1 posicio adalt (amb Shift 1 pantalla sencera)\n"
	"  S      1 posicio abaix (amb Shift 1 pantalla sencera)\n"
	"  D      1 posicio a la dreta (amb Shift 1 pantalla sencera)\n"
	"  A      1 posicio a l'esquerra (amb Shift 1 pantalla sencera)\n"
	"\nActuacio sobre el biosistema:\n\n"
	"  R      Torna a carregar el fitxer de configuracio d'agents externs\n"
	"  E      Diferencia els colors dels organismes mutants (amb Shift es desactiva)\n"
	"  P      Congela el biosistema (amb Shift ho descongela)\n"
	"\n  Premi return per a continuar amb el programa\n"
	"\n  Copirait KKEPerians UNLTD\n";

void CBiosistema::ProvaClasse()
{
//	out << clrscr;
	out << blanc.brillant() << "Provant Biosistema" << blanc.fosc() << endl;

	Config.parsejaArxiu("Bioscena.ini", error);
	CBiosistema biosistema;
	biosistema.carregaOpCodes("Opcodes.ini", error);
	out << "Inicialitzant Biotop..." << endl;
	CTopologia * topologia = new CTopologiaToroidal(Config.get("Biotop/CasellesAmplitud"),Config.get("Biotop/CasellesAltitud"));
	biosistema.biotop(new CBiotop<CBiosistema::t_substrat>(topologia));
	biosistema.comunitat(new CComunitat);
	biosistema.taxonomista(new CTaxonomista);
	biosistema.agents(CAgent::ParsejaArxiu("Agents.ini", *(biosistema.biotop()), error));

	uint32 pasVisualitzacio=0;
	uint32 pas=pasVisualitzacio;

	enum {Blanc, Mapa, MapaOrganismes, Organismes} mode = MapaOrganismes;
	bool modeCanviat=true;
	CComparativaOrganismes graf1(biosistema.comunitat());
	CComparativaOrganismes graf2(biosistema.comunitat());
	CComparativaOrganismes graf3(biosistema.comunitat());
	CComparativaOrganismes graf4(biosistema.comunitat());
	CComparativaOrganismes graf5(biosistema.comunitat());
	CMapa mapa1(&biosistema);

	mapa1.primeraPosicio(0);
	bool helpWanted=true;
	bool editWanted=false;
	bool redisplayWanted=false;
	bool congelat=false;
	uint32 id=0;
	out << endl << "Biosistema carregat. Prem return per continuar..." << endl;
	cin.get();

	while (true) {
		// Actualitzem el mode de visualitzacio
		if (modeCanviat) {
			out << clrscr;
			switch (mode) {
			case Mapa:
			default:
				mapa1.tamany(1,2,80,45); 
				break;
			case MapaOrganismes:
				mapa1.tamany(1,2,40,40);
				graf1.tamany(41,1,40,8);
				graf1.primerOrganisme(0);
				graf2.tamany(41,11,40,8);
				graf2.primerOrganisme(40);
				graf3.tamany(41,21,40,8);
				graf3.primerOrganisme(80);
				graf4.tamany(41,31,40,8);
				graf4.primerOrganisme(120);
				graf5.tamany(41,41,40,8);
				graf5.primerOrganisme(160);
				break;
			case Organismes:
				graf1.tamany(1,2,80,8);
				graf1.primerOrganisme(0);
				graf2.tamany(1,12,80,8);
				graf2.primerOrganisme(80);
				graf3.tamany(1,22,80,8);
				graf3.primerOrganisme(160);
				graf4.tamany(1,32,80,8);
				graf4.primerOrganisme(240);
				graf5.tamany(1,42,80,7);
				graf5.primerOrganisme(320);
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
			if (mode==Mapa||mode==MapaOrganismes)
				mapa1.visualitza(out);
			if (mode==MapaOrganismes||mode==Organismes) {
				graf1.visualitza(out);
				graf2.visualitza(out);
				graf3.visualitza(out);
				graf4.visualitza(out);
				graf5.visualitza(out);
			}
			uint32 pos = mapa1.primeraPosicio();
			out << setfill('0');
			switch (mode)
			{
			case Blanc:
			case Mapa:
				out << gotoxy(42,1) 
					<< "Coords: " << setw(3) << pos%Config.get("Biotop/CasellesAmplitud") 
					<< '@' << setw(3) << pos/Config.get("Biotop/CasellesAmplitud") 
					;
				break;
			case MapaOrganismes:
				out << gotoxy(1,42) 
					<< "Coords: " << setw(3) << pos%Config.get("Biotop/CasellesAmplitud") 
					<< '@' << setw(3) << pos/Config.get("Biotop/CasellesAmplitud") 
					;
				break;
			}
			out << gotoxy(1, 1) << "Poblacio: " << setw(6) << biosistema.comunitat()->tamany();
			out << gotoxy(26,1) << "Temps: " << setw(8) << biosistema.m_temps;
			out << setfill(' ') << endl;
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
					else if (mode==Mapa) mode=Blanc;
					else modeCanviat = false;
					break;
				case 'm': 
					modeCanviat = true;
					if (mode==Organismes) mode=MapaOrganismes;
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
					out << clrscr << endl;
					biosistema.deleteAgents();
					biosistema.agents(CAgent::ParsejaArxiu(filename, *(biosistema.biotop()), error));
					// TODO: Recarregar configuracio 
					out << "Prem una tecla" << endl;
					cin.get();
					modeCanviat = true;
					break;
				case 'r': 
					out << clrscr << endl;
					biosistema.deleteAgents();
					biosistema.agents(CAgent::ParsejaArxiu("Agents.ini", *(biosistema.biotop()), error));
					// TODO: Recarregar configuracio 
					out << "Prem una tecla" << endl;
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
				// out << gotoxy(20,48) << "Tecla: " << '\'' << (a?a:'#') << '\''<< endl;
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
			cout << clrscr << endl;
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
			else cout << "L'organisme ha mort o mai ha existit" << endl;
			cin.get();
			modeCanviat = true;
		}
	}
}


