// Organisme.cpp: implementation of the COrganisme class.
//
//////////////////////////////////////////////////////////////////////
#include <iomanip>
#include "Organisme.h"
#include "Compatibilitat.h"
#include "Missatger.h"
#include "Color.h"
#include "RandomStream.h"

static CMissatger tracaOrganisme ("Traca Organisme");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COrganisme::COrganisme() : m_energia(OrganismeCaducitatEnergia)
{
	tracaOrganisme.activa();
	for (int i=OrganismeNumRegistres; i--;)
		rnd >> m_registres[i];
}

COrganisme::~COrganisme()
{

}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

COrganisme::t_mollecula COrganisme::metabolitza(uint32 patro, uint32 tolerancia)
	// PRE: m_nutrients te nutrients (TODO: considerar treure-la)
{
	tracaOrganisme<<"Metabolitzant: "<< endl;
	tracaOrganisme << hex << setfill('0');
	tracaOrganisme <<"\tPat: "<<setw(8)<<patro<<endl;
	tracaOrganisme <<"\tTol: "<<setw(8)<<tolerancia<<endl;
	t_mollecula producte;
	list<t_mollecula>::iterator it;
	for (it=m_nutrients.begin(); it!=m_nutrients.end(); it++)
	{
		if (compat2(*it, patro, tolerancia))
			break;
	}
	if (it==m_nutrients.end())
		it=m_nutrients.begin();
	tracaOrganisme 
		<<"\tMolecula escollida: "<<setw(8)<<(*it)<<endl;
	tracaOrganisme 
		<<"\tRegistre metabolitzador: "<<setw(8)<<registre(R_METABOLISME)<<endl;
	producte=*it^registre(R_METABOLISME);
	m_nutrients.erase(it);
	m_energia.afegeix(comptaUns(producte));
	tracaOrganisme << dec << setfill(' ') << "\tEnergia obtinguda " << comptaUns(producte) << endl;
	return producte;
}

void COrganisme::engulleix(uint32 element)
{
	m_nutrients.push_back(element);
}

uint32 COrganisme::registre(uint32 index)
{
	return m_registres[index];
}

void COrganisme::registre(uint32 index, uint32 valor)
{
	m_registres[index]=valor;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void COrganisme::ProvaClasse()
{
	log1 << "Provant organismes: "<< endl;
	tracaOrganisme << hex << setfill('0');
	COrganisme pepe;
	pepe.engulleix(0x00000000);
	pepe.engulleix(0x000FF000);
	pepe.engulleix(0xF00FFF00);
	pepe.engulleix(0xF0000F00);
	pepe.engulleix(0xF0F0FF00);
	pepe.engulleix(0x00000000);
	pepe.engulleix(0x000F0F00);
	pepe.engulleix(0x0F00FF00);
	pepe.engulleix(0xF0F00F00);
	pepe.engulleix(0xF00FF000);
	pepe.engulleix(0x000F0F00);
	pepe.debugPresentaNutrients(tracaOrganisme);
	t_mollecula excrecio;
	uint32 pat, tol;
	excrecio=pepe.metabolitza(pat=0x0006FF00, tol=0xFF0000FF);
	tracaOrganisme << hex << setfill('0');
	tracaOrganisme << "\tExcrecio: "<<setw(8)<<excrecio<< endl;
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

