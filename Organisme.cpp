// Organisme.cpp: implementation of the COrganisme class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990902 VoK - 
// 19991124 VoK - Comencada a fer la estructura jerarquica per 
//                permetre barreja d'organismes de diferents tipus.
// 19991126 VoK - Fix: No feiem delete del fenotip al destructor
// 19991214 VoK - Capacitat del pap limitat (Ocupava massa memoria)
// 19991215 VoK - L'energia es disipa
// 20000103 VoK - Funcio membre que detecta nutrients al pap sense 
//                extreure'ls. Y2K compatible!!!
// 20000217 VoK - Serialitzacio
//////////////////////////////////////////////////////////////////////

#include <iomanip>
#include "Organisme.h"
#include "Compatibilitat.h"
#include "Missatger.h"
#include "Color.h"
#include "RandomStream.h"
#include "Configuracio.h"

static CMissatger tracaOrganisme ("Organisme");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COrganisme::COrganisme() : 
	m_energia(Config.get("Organisme/Energia/Compartiments"))
{
	// Desactivem o on el traceig
	tracaOrganisme.desactiva();
	// Inicialitzem el material genetic
	m_cariotip.init(rnd.get(
		Config.get("Organisme/Cariotip/LongitudMinima"),
		Config.get("Organisme/Cariotip/LongitudMaxima")));
	m_mutat = false; 
	m_genotip.init(m_cariotip);
	// Inicialitzem el fenotip
	m_fenotip = new uint32[Config.get("Organisme/Fenotip/Longitud")];
	if (!m_fenotip) {
		error << "Error demanant fenotip" << endl;
		cin.get();
	}
	for (int i=Config.get("Organisme/Fenotip/Longitud"); i--;)
		rnd >> m_fenotip[i];
	// Tot individu comenca amb 
	m_nutrients.clear();
	m_edat=0;
	m_foo=0;
	m_primigeni=true;
	m_tempsDisipacio=Config.get("Organisme/Energia/CaducitatCompartiments");
	m_tempsDisipacioRestant=m_tempsDisipacio;
}

COrganisme::COrganisme(CCariotip &c) : 
	m_energia(Config.get("Organisme/Energia/Compartiments"))
{
	// Desactivem o on el traceig
	tracaOrganisme.desactiva();
	// Inicialitzem el material genetic
	m_cariotip.init(c);
	m_mutat = m_cariotip.muta(); 
	m_genotip.init(m_cariotip);
	// Inicialitzem el fenotip
	m_fenotip = new uint32[Config.get("Organisme/Fenotip/Longitud")];
	if (!m_fenotip) 
	{
		error << "Error demanant fenotip" << endl;
		cin.get();
	}
	for (uint32 i=Config.get("Organisme/Fenotip/Longitud"), bit=1; i--;)
		m_fenotip[i] = rnd.get() & ~0x88888888; // (bit <<=1);
//		rnd >> m_fenotip[i];
	// Tot individu comenca amb 
	m_nutrients.clear();
	m_edat=0;
	m_foo=0;
	m_primigeni=false;
	m_tempsDisipacio=Config.get("Organisme/Energia/CaducitatCompartiments");
	m_tempsDisipacioRestant=m_tempsDisipacio;
}

COrganisme::~COrganisme()
{
	if (m_fenotip) delete[] m_fenotip;
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

ostream & COrganisme::store(ostream & str) 
{
//	str << binary;
	for (uint32 i=0; i<Config.get("Organisme/Fenotip/Longitud"); i++) {
		str.write((char*)&(m_fenotip[i]),sizeof(uint32));
		if (!str) error << "Error escrivint la cassella " << i << " del fenotip" << endl;
		}
	uint32 nNutrients=m_nutrients.size();
	str.write((char*)&nNutrients,sizeof(uint32));
	list<t_mollecula>::iterator it;
	for (it=m_nutrients.begin(); it!=m_nutrients.end(); it++) {
		str.write((char*)&(*it),sizeof(uint32));
		if (!str) error << "Error escrivint el nutrient " << i << " del pap" << endl;
		}
	m_cariotip.store(str);
	m_energia.store(str);
	// TODO: Store: Pap
	return str;
}

istream & COrganisme::load(istream & str) {
//	str >> binary;
	if (m_fenotip) delete[] m_fenotip;
	m_fenotip = new uint32[Config.get("Organisme/Fenotip/Longitud")];
	for (uint32 i=0; i<Config.get("Organisme/Fenotip/Longitud"); i++) {
		uint32 valor;
		str.read((char*)&valor,sizeof(uint32));
		if (m_fenotip&&str) m_fenotip[i]=valor;
	}
	uint32 nNutrients;
	str.read((char*)&nNutrients,sizeof(uint32));
	for (uint32 i=0; i<nNutrients; i++) {		
		uint32 valor;
		str.read((char*)&valor,sizeof(uint32));
		if (str) m_nutrients.push_back(valor);
		}
	m_cariotip.load(str);
	m_genotip.clear();
	m_genotip.init(m_cariotip);
	m_energia.load(str);
	// TODO: Load: Pap
	return str;
}

void COrganisme::dump(CMissatger & msgr)
{
//	debugPresentaNutrients(msgr);
	debugPresentaFenotip(msgr);
	m_cariotip.dump(msgr);
	m_genotip.dump(msgr);
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

// Fenotip

uint32 COrganisme::fenotip(uint32 index)
{
	return m_fenotip[index];
}

void COrganisme::fenotip(uint32 index, uint32 valor)
{
	m_fenotip[index]=valor;
}

uint32 & COrganisme::operator[](uint32 index)
{
	return m_fenotip[index];
}

// Nutricio

void COrganisme::engoleix(t_mollecula element)
{
	m_nutrients.push_back(element);
	uint32 maximNutrients = Config.get("Organisme/Pap/Capacitat"); 
	if (maximNutrients && m_nutrients.size()>maximNutrients)
		m_nutrients.pop_front();
}
	
bool COrganisme::excreta(t_mollecula & excretada, uint32 patro, uint32 tolerancia)
{
	list<t_mollecula>::iterator it;
	for (it=m_nutrients.begin(); it!=m_nutrients.end(); it++)
		if (sonCompatibles(*it, patro, tolerancia))
			break;
	if (it==m_nutrients.end())
		return false;
	excretada=*it;
	m_nutrients.erase(it);
	return true;
}

bool COrganisme::detecta(uint32 & detectada, uint32 patro, uint32 tolerancia)
{
	list<t_mollecula>::iterator it;
	for (it=m_nutrients.begin(); it!=m_nutrients.end(); it++)
		if (sonCompatibles(*it, patro, tolerancia)) {
			detectada=*it;
			return true;
			}
	return false;
}

bool COrganisme::defensa(list<t_mollecula> &fluxeQuimic, uint32 clauAtac, uint32 patroNutrient, uint32 toleranciaNutrient)
{
	// TODO: Treure el random i posar quelcom amb sentit per la defensa (una clau de defensa)
	uint32 forcaAtac = comptaUns(clauAtac^rnd.get());
	// Aixo es per si un dia volem fer comportaments d'inoculacio
	while (!fluxeQuimic.empty()&&forcaAtac)
	{
		engoleix(fluxeQuimic.back());
		forcaAtac--;
	}
	fluxeQuimic.clear();
	// I ara ve lo normal d'atacar.
	// El biosistema sabra que fer-ne d'aquests nutrients a fluxeQuimic:
	// - Passar-los al atacant directament
	// - Deixar-los lliures al medi
	while (forcaAtac)
	{
		t_mollecula mol;
		if (excreta(mol, patroNutrient, toleranciaNutrient))
			fluxeQuimic.push_back(mol);
		forcaAtac--;
	}
	return fluxeQuimic.size()!=0;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void COrganisme::ProvaClasse()
{
	log1 << "Provant organismes: "<< endl;
	tracaOrganisme << hex << setfill('0');
	COrganisme pepe;
//	pepe.debugPresentaFenotip(log1);
	pepe.engoleix(0x0000FF00);
	pepe.engoleix(0x0000FFF0);
	pepe.engoleix(0x00000F00);
	pepe.engoleix(0x0000F0FF);
	pepe.engoleix(0x000000F0);
	pepe.engoleix(0x00000000);
	pepe.engoleix(0x0000000F);
	pepe.engoleix(0x0000FF0F);
	pepe.engoleix(0x0000F000);
	pepe.engoleix(0x0000FF0F);
	pepe.engoleix(0x00000F0F);
//	pepe.debugPresentaNutrients(log1);
	t_mollecula excrecio;
	uint32 energia;
	uint32 A, tolA, B, tolB, key;
	pepe.excreta   (excrecio, A=0x000000F0, tolA=0x0000FFFF);
	pepe.excreta   (excrecio, B=0x000000F0, tolB=0x0000FFFF);

	pepe.debugPresentaNutrients(log1);

	out << "Provant Serialitzacio" << endl;

	ofstream ofile("borrame.hex", ios::binary|ios::out);
	if (!ofile) error << "Abriendo fichero" << endl;
	pepe.store(ofile);
	ofile.close();

	COrganisme pepet;
	ifstream ifile("borrame.hex", ios::binary|ios::in);
	if (!ifile) error << "Abriendo fichero" << endl;
	pepet.load(ifile);
	ifile.close();
	pepe.debugPresentaFenotip(out);	cin.get();
	pepet.debugPresentaFenotip(out); cin.get();
	pepe.debugPresentaNutrients(out); cin.get();
	pepet.debugPresentaNutrients(out); cin.get();
	pepe.m_cariotip.dump(out); cin.get();
	pepet.m_cariotip.dump(out); cin.get();
	pepe.m_genotip.dump(out); cin.get();
	pepet.m_genotip.dump(out); cin.get();
	pepe.m_energia.dump(out); cin.get();
	pepet.m_energia.dump(out); cin.get();
}

void COrganisme::debugPresentaNutrients(CMissatger & msgr)
{
	list<t_mollecula>::iterator it;
	msgr << "Contingut de l'estomac:"<< endl << hex << setfill('0');
	for (it=m_nutrients.begin(); it!=m_nutrients.end(); it++)
	{
		msgr << "\t" << setw(8)<< *it << "." << endl;
	}
	msgr << dec << setfill(' ');
}

void COrganisme::debugPresentaFenotip(CMissatger & msgr)
{
	uint32 i;
	msgr << "Contingut dels registres:"<< endl << hex << setfill('0');
	for (i=0; i<Config.get("Organisme/Fenotip/Longitud"); i++)
	{
		msgr << "\t" << setw(2) << i << " " << setw(8)<< m_fenotip[i] << " ";
		if (!((i+1)%4)) msgr<< endl;
	}
	if (i%4) msgr<< endl;
	msgr << dec << setfill(' ');
}


COrganisme::t_instruccio COrganisme::seguentInstruccio()
{
	m_edat++;
	return m_genotip.seguentInstruccio(m_fenotip);
	if (m_tempsDisipacio) {
		m_tempsDisipacioRestant--;
		if (!m_tempsDisipacioRestant) {
			m_energia();
			m_tempsDisipacioRestant = m_tempsDisipacio;
		}
	}
}

bool COrganisme::consumeixEnergia(uint32 energia)
{
	return m_energia.consumeix(energia)==0;
}

void COrganisme::guanyaEnergia(uint32 energia)
{
	m_energia.afegeix(energia);
}

uint32 COrganisme::energia()
{
	return m_energia.total();
}

uint32 COrganisme::edat()
{
	return m_edat;
}
