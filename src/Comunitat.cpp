// Comunitat.cpp: implementation of the CComunitat class.
//
//////////////////////////////////////////////////////////////////////
// Change Log: 
// 19990918 VoK - Recreat
// 20000221 VoK - Serialitzacio
//////////////////////////////////////////////////////////////////////
// TODO: Serialitzacio cooperativa amb InfoOrganisme

#include <algorithm>
#include <fstream>
//#include <functional>
#include "Comunitat.h"
#include "Configuracio.h"
#include "RandomStream.h"
#include "Color.h"
//#include "Grafic.h"

//////////////////////////////////////////////////////////////////////
// Variables estatiques
//////////////////////////////////////////////////////////////////////

static bool tracaAltes=false;
static bool tracaBaixes=false;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComunitat::CComunitat()
{
	m_nOrganismes=0;
}

CComunitat::~CComunitat()
{
	vector<CInfoOrganisme>::iterator it = m_organismes.begin();
	for (; it!=m_organismes.end(); ++it)
		it->cos(NULL);
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

ostream & CComunitat::store(ostream & str) 
{
	// TODO: Fer els store cooperativament amb InfoOrganisme
	uint32 valor;
	valor = m_organismes.size();
	str.write((char*)&valor,sizeof(uint32));
	valor -= m_disponibles.size();
	str.write((char*)&valor,sizeof(uint32));
	vector<CInfoOrganisme>::iterator it = m_organismes.begin();
	for (uint32 index=0; it!=m_organismes.end(); ++it, ++index) {
		if (it->cos()) {
			str.write((char*)&index,sizeof(uint32));
			valor = it->posicio();
			str.write((char*)&valor,sizeof(uint32));
			valor = it->taxo();
			str.write((char*)&valor,sizeof(uint32));
			it->cos()->store(str);
			}
		}
	return str;
}

istream & CComunitat::load(istream & str) 
{
	// TODO: Fer els loads cooperativament amb InfoOrganisme
	vector<CInfoOrganisme>::iterator it = m_organismes.begin();
	for (; it!=m_organismes.end(); ++it)
		it->cos(NULL);
	m_disponibles.clear();
	m_organismes.clear();
	m_nOrganismes=0;
	uint32 tamany, nOrganismes, index;
	str.read((char*)&tamany,sizeof(uint32));
	str.read((char*)&nOrganismes,sizeof(uint32));
	uint32 fillIndex=0;
	for (uint32 i=0; i<nOrganismes; i++, fillIndex++) {
		uint32 posicio, taxo;
		str.read((char*)&index,sizeof(uint32));
		// fica com a disponibles els indexos que es salta
		for ( ;fillIndex<index; fillIndex++) {
			m_disponibles.push_back(fillIndex);
			push_heap(m_disponibles.begin(), m_disponibles.end(), greater<uint32>());
			}
		// Crea les caselles al vector d'organismes (estiguin disponibles o no)
		while (index>=m_organismes.size()) 
			m_organismes.push_back(CInfoOrganisme());
		str.read((char*)&posicio,sizeof(uint32));
		str.read((char*)&taxo,sizeof(uint32));
		COrganisme * org = new COrganisme;
		if (org) {
			org->load(str);
			CInfoOrganisme & nouOrganisme = m_organismes[index];
			nouOrganisme.taxo(taxo);
			nouOrganisme.posicio(posicio);
			nouOrganisme.cos(org);
			ostrstream fluxe;
			fluxe
				<< setfill('0') 
				<< oct << (index>>6) << dec << CColor((index>>3)&07).brillant() << (index&07) << blanc.fosc() << "-" 
				<< setw(3) << nouOrganisme.subidentificador() << "-" 
				<< (taxo>>6) << (taxo&070?negre.fons((taxo&070)>>3):blanc) << (taxo&7) << blanc.fosc() << " "
				<< setfill(' ') 
				<< ends; // El fluxe no afegeix un /0 sino ho fem nosaltres
			nouOrganisme.descripcio(fluxe.str());
			fluxe.freeze(false);
			++m_nOrganismes;
			}
		}
	return str;
}


void CComunitat::dump(CMissatger & msg)
{
	msg << "Comunitat de " << (m_organismes.size() - m_disponibles.size()) << " organismes." << endl;
	vector<CInfoOrganisme>::iterator it = m_organismes.begin();
	for (uint32 index=0; it!=m_organismes.end(); ++it, ++index) {
//		if (it->cos()) {
			msg 
				<< groc.brillant() << "* Organisme #" << index << blanc.fosc()
				<< " Posicio: " << it->posicio() 
				<< " Taxo: " << it->taxo() 
				<< endl;
			msg << " Punter " << it->cos() << endl;
//			it->cos()->dump(msg);
//		}
	}
	msg << "Disponibles: ";
	dumpDisponibles(msg);
}

void CComunitat::dumpDisponibles(CMissatger & msg)
{
	vector<uint32>::iterator it = m_disponibles.begin();
	for (uint32 index=0; it!=m_disponibles.end(); ++it, ++index)
		msg << *it << "-";
	msg << endl;
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

CInfoOrganisme & CComunitat::operator[](uint32 index) {
	if (index>=m_organismes.size()) {
		cout << "Accedint a un organisme no existent!" << endl;
		cin.get();
	}
	return m_organismes[index];
}

uint32 CComunitat::introdueix(COrganisme* org, uint32 posicio, uint32 taxo)
	// PRE: org és un punter vàlid
{
	// Canvis en aquest codi poden afectar a 'load'!
	uint32 index;
	if (m_disponibles.size()) {
		index = m_disponibles.front();
		pop_heap(m_disponibles.begin(), m_disponibles.end(), greater<uint32>());
		m_disponibles.pop_back();
	} 
	else {
		index = m_organismes.size();
		m_organismes.push_back(CInfoOrganisme());
	}
	if (tracaAltes)
		out << "Introduint organisme " << index << endl;
	CInfoOrganisme & nouOrganisme = m_organismes[index];
	nouOrganisme.taxo(taxo);
	nouOrganisme.posicio(posicio);
	nouOrganisme.cos(org);
	ostrstream fluxe;
	fluxe
		<< setfill('0') 
		<< oct << (index>>6) << dec << CColor((index>>3)&07).brillant() << (index&07) << blanc.fosc() << "-" 
		<< setw(3) << nouOrganisme.subidentificador() << "-" 
		<< (taxo>>6) << (taxo&070?negre.fons((taxo&070)>>3):blanc) << (taxo&7) << blanc.fosc() << " "
		<< setfill(' ') 
		<< ends; // El fluxe no afegeix un /0 sino ho fem nosaltres
	nouOrganisme.descripcio(fluxe.str());
	fluxe.freeze(false);
	++m_nOrganismes;
	return index;
}

void CComunitat::extreu (uint32 index)
	// Pre: index es un organisme anteriorment introduit
{
	if (tracaBaixes)
		out << "Extreient organisme " << index << endl;
//	CInfoOrganisme & vellOrganisme = m_organismes[index];
	// Eliminem l'organisme, si encara esta
	m_organismes[index].cos(NULL);
	// Posem el node 'index' disponible
	m_disponibles.push_back(index);
	push_heap(m_disponibles.begin(), m_disponibles.end(), greater<uint32>());
	// TODO: Alliberar espai quan hi hagi molt de lliure
	--m_nOrganismes;
}

uint32 CComunitat::organismeAleatori()
{
	KKEPAssert ((m_organismes.size())>(m_disponibles.size()), 
		"Comunitat: No hi ha organismes per escollir");
	while (true) {
		uint32 org=rnd.get(0,m_organismes.size()-1);
		if (org>=m_organismes.size()) {
			error << "Error escogiendo organismo" << endl;
			cin.get();
		}
		if (m_organismes[org].cos()) return org;
	}
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CComunitat::ProvaClasse (void)
{
	out << "Provant inicialitzacio" << endl;
	CComunitat com;
	com.dump(out); cin.get();
	out << "Provant Introduccio successiva de 9 organismes" << endl;
	COrganisme * org;
	org = new COrganisme();
	com.introdueix(org,1,11);
	com.dump(out); cin.get();
	org = new COrganisme();
	com.introdueix(org,2,12);
	com.dump(out); cin.get();
	org = new COrganisme();
	com.introdueix(org,3,13);
	com.dump(out); cin.get();
	org = new COrganisme();
	com.introdueix(org,4,14);
	com.dump(out); cin.get();
	org = new COrganisme();
	com.introdueix(org,5,15);
	org = new COrganisme();
	com.introdueix(org,6,16);
	org = new COrganisme();
	com.introdueix(org,7,17);
	org = new COrganisme();
	com.introdueix(org,8,18);
	org = new COrganisme();
	com.introdueix(org,9,19);
	com.dump(out); cin.get();
	out << "Provant Extraccio d'organismes aleatoris" << endl;
	com.extreu(com.organismeAleatori());
	com.dump(out); cin.get();
	com.extreu(com.organismeAleatori());
	com.dump(out); cin.get();
	com.extreu(com.organismeAleatori());
	com.extreu(com.organismeAleatori());
	com.extreu(com.organismeAleatori());
	com.extreu(com.organismeAleatori());
	com.extreu(com.organismeAleatori());
	com.extreu(com.organismeAleatori());
	com.extreu(com.organismeAleatori());
	com.dump(out); cin.get();
	out << "Comunitat buida, prem 's' i return si vols veure que passa si extreus un altre" << endl;
	if (cin.get()=='y') {
		com.extreu(com.organismeAleatori());
		com.dump(out); cin.get();
		}
	out << "Provant serialitzacio" << endl;
	out << " Torno a omplir la comunitat..." << endl;
	org = new COrganisme();
	com.introdueix(org,5,15);
	com.dump(out); cin.get();
	org = new COrganisme();
	com.introdueix(org,6,16);
	com.dump(out); cin.get();
	org = new COrganisme();
	out << " Extrec un per deixar un espai en blanc..." << endl;
	com.introdueix(org,7,17);
	com.dump(out); cin.get();
	com.extreu(com.organismeAleatori());
	out << " Passivitzant..." << endl;
	com.dump(out); cin.get();
	ofstream ofile("Borrame", ios::out|ios::binary);
	com.store(ofile);
	ofile.close();
	out << " Activant..." << endl;
	CComunitat com2;
	ifstream ifile("Borrame", ios::in|ios::binary);
	com2.load(ifile);
	ifile.close();
	com2.dump(out); cin.get();
}


uint32 CComunitat::tamany()
{
	return m_organismes.size() - m_disponibles.size();
}

bool CComunitat::esValid(uint32 index)
{
	return index<m_organismes.size() && m_organismes[index].cos()!=0;
}
