// Organisme.cpp: implementation of the COrganisme class.
//
//////////////////////////////////////////////////////////////////////
#include <iomanip>
#include "Organisme.h"
#include "Compatibilitat.h"
#include "Missatger.h"
#include "Color.h"
#include "RandomStream.h"

static CMissatger tracaOrganisme ("Organisme");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COrganisme::COrganisme() : m_energia(OrganismeCaducitatEnergia)
{
	// Desactivem o on el traceig
	tracaOrganisme.activa();
	// En principi cap registre del fenotip es diferit
	m_lecturaDiferida=0L;
	// Inicialitzem el fenotip
	m_fenotip = new uint32[OrganismeLongitudFenotip];
	if (!m_fenotip) tracaOrganisme("Error demanant fenotip");
	for (int i=OrganismeLongitudFenotip; i--;)
		rnd >> m_fenotip[i];
	// Tot individu comenca amb 
	m_energia.afegeix(OrganismeEnergiaMinima);
}

COrganisme::~COrganisme()
{

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

bool COrganisme::anabolitza(uint32 & energia, uint32 A, uint32 toleranciaA, uint32 B, uint32 toleranciaB)
	// Realitza la reaccio A + B + energia -> C
{
	tracaOrganisme <<"Anabolitzant: "<< endl;
	tracaOrganisme << hex << setfill('0');
	tracaOrganisme <<"\tPatrons:     "<<setw(8)<<A<<" "<<setw(8)<<B<<endl;
	tracaOrganisme <<"\tTolerancies: "<<setw(8)<<toleranciaA<<" "<<setw(8)<<toleranciaB<<endl;
	tracaOrganisme << dec << setfill(' ');
	list<t_mollecula>::iterator itA;
	for (itA=m_nutrients.begin(); itA!=m_nutrients.end(); itA++)
	{
		if (sonCompatibles(*itA, A, toleranciaA))
			break;
	}
	if (itA==m_nutrients.end()) {
		tracaOrganisme <<"\tNo s'ha trobat la mol·lecula A: "<<endl;
		return false;
	}
	list<t_mollecula>::iterator itB;
	for (itB=m_nutrients.begin(); itB!=m_nutrients.end(); itB++)
	{
		if (sonCompatibles(*itB, B, toleranciaB)&&(itA!=itB))
			break;
	}
	if (itB==m_nutrients.end()) {
		tracaOrganisme <<"\tNo s'ha trobat la mol·lecula B: "<<endl;
		return false;
	}
	uint32 C=*itA|*itB;
	energia=comptaUns(C)- min(comptaUns(*itA),comptaUns(*itB));

	tracaOrganisme << hex << setfill('0');
	tracaOrganisme <<"\tReactius: "<<setw(8)<<(*itA)<<" "<<setw(8)<<(*itB)<<endl;
	tracaOrganisme <<"\tProducte: "<<setw(8)<<C<<endl;
	tracaOrganisme << dec << setfill(' ');
	tracaOrganisme << "\tEnergia emprada: " << energia << endl;

	m_nutrients.erase(itA);
	m_nutrients.erase(itB);
	m_nutrients.push_back(C);

	return true;
}

bool COrganisme::catabolitza(uint32 & energia, uint32 A, uint32 toleranciaA, uint32 clauCatabolitzadora)
{
	tracaOrganisme <<"Catabolitzant: "<< endl;
	tracaOrganisme << hex << setfill('0');
	tracaOrganisme <<"\tPatro     : "<<setw(8)<<A<<endl;
	tracaOrganisme <<"\tTolerancia: "<<setw(8)<<toleranciaA<<endl;
	tracaOrganisme <<"\tCatabolic : "<<setw(8)<<clauCatabolitzadora<<endl;
	tracaOrganisme << dec << setfill(' ');
	list<t_mollecula>::iterator itA;
	for (itA=m_nutrients.begin(); itA!=m_nutrients.end(); itA++)
	{
		if (sonCompatibles(*itA, A, toleranciaA))
			break;
	}
	if (itA==m_nutrients.end()) {
		tracaOrganisme <<"\tNo s'ha trobat la mol·lecula"<< endl;
		return false;
	}
	uint32 B=(*itA^clauCatabolitzadora)&*itA;
	uint32 C=(*itA^~clauCatabolitzadora)&*itA;
	energia=comptaUns(*itA)- max(comptaUns(B),comptaUns(C));

	tracaOrganisme << hex << setfill('0');
	tracaOrganisme <<"\tReactius: "<<setw(8)<<(*itA)<<endl;
	tracaOrganisme <<"\tProducte: "<<setw(8)<<B<<" "<<setw(8)<<C<<endl;
	tracaOrganisme << dec << setfill(' ');
	tracaOrganisme << "\tEnergia obtinguda: " << energia << endl;

	m_nutrients.erase(itA);
	m_nutrients.push_back(B);
	m_nutrients.push_back(C);

	return true;
}

void COrganisme::engoleix(t_mollecula element)
{
	m_nutrients.push_back(element);
}

bool COrganisme::excreta(t_mollecula & excreccio, uint32 patro, uint32 tolerancia)
{
	tracaOrganisme <<"Excretant: "<< endl;
	tracaOrganisme << hex << setfill('0');
	tracaOrganisme <<"\tPat: "<<setw(8)<<patro<<endl;
	tracaOrganisme <<"\tTol: "<<setw(8)<<tolerancia<<endl;
	list<t_mollecula>::iterator it;
	for (it=m_nutrients.begin(); it!=m_nutrients.end(); it++)
	{
		if (sonCompatibles(*it, patro, tolerancia))
			break;
	}
	if (it==m_nutrients.end()) {
		tracaOrganisme << dec << setfill(' ');
		return false;
	}
	excreccio=*it;
	m_nutrients.erase(it);
	tracaOrganisme <<"\tExc: "<<setw(8)<<excreccio<<endl;
	tracaOrganisme << dec << setfill(' ');
	return true;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void COrganisme::ProvaClasse()
{
	log1 << "Provant organismes: "<< endl;
	tracaOrganisme << hex << setfill('0');
	COrganisme pepe;
	pepe.debugPresentaFenotip(tracaOrganisme);
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
	pepe.debugPresentaNutrients(tracaOrganisme);
	t_mollecula excrecio;
	uint32 energia;
	uint32 A, tolA, B, tolB, key;
	pepe.catabolitza(energia, A=0x0006FF00, tolA=0x000000FF, key=0x0000F000);
	pepe.anabolitza (energia, A=0x000000F0, tolA=0x0000FFFF, B=0x0000F600, tolB=0x0000F0F0);
	pepe.excreta   (excrecio, A=0x000000F0, tolA=0x0000FFFF);
	pepe.debugPresentaNutrients(tracaOrganisme);
	tracaOrganisme << dec << setfill(' ');
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
	for (i=0; i<OrganismeLongitudFenotip; i++)
	{
		msgr << "\t" << setw(2) << i << " " << setw(8)<< m_fenotip[i] << " ";
		if (!((i+1)%4)) msgr<< endl;
	}
	if (i%4) msgr<< endl;
	msgr << dec << setfill(' ');
}


COrganisme::t_instruccio COrganisme::seguentInstruccio()
{
	return rnd.get();
/*
	m_genotip();
	return m_genotip.seguentGen();
*/
}
