// Cariotip.cpp: implementation of the CCariotip class.
//
//////////////////////////////////////////////////////////////////////
// Vots per punter
// - Ens fem responsables de la memoria
// - 
// - 
// - 
// Vots per copia
// - La copia dels cromosomes es superficial pels punters amb els metodes de STL
// - 
// - 
// - 

#ifdef _MSC_VER
// El namespace es per solucionar un conflicte amb el gotoxy de AnsiCodes.h
namespace ops {
	#include <conio.h>
	}
#define kbhit ops::_kbhit
#else
#include <pc.h>
#endif
#include "Cariotip.h"
#include "Configuracio.h"
#include "Color.h"
#include "Probabilitat.h"
#include "MutacioCariotip.h"
#include "MutacioCromosomica.h"
#include "MutacioGenica.h"

using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Variables estatiques
//////////////////////////////////////////////////////////////////////

static bool tracePrivate=false;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCariotip::CCariotip()
	:m_cromosomes()
{
}

CCariotip::~CCariotip()
{
	for (t_cromosomes::iterator it = m_cromosomes.begin(); it!=m_cromosomes.end(); it++)
		if (*it) {
			delete *it;
			*it=NULL;
		}
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

void CCariotip::dump(CMissatger & msg)
{
	msg << "Cariotip amb " << m_cromosomes.size() << " cromosomes " << endl;
	for (uint32 i=m_cromosomes.size(); i--;) {
		msg << i << "-\t";
		if (m_cromosomes[i]) m_cromosomes[i]->dump(msg);
		else msg << "(NULL)" << endl;
	}
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

bool CCariotip::init(uint32 nCromosomes)
{
	if (!ocupaCromosomes(nCromosomes))
		return false;
	ompleCromosomesAleatoriament(
		Config.get("Organisme/Cromosoma/LongitudMinima"),
		Config.get("Organisme/Cromosoma/LongitudMaxima")
		);
	return true;
}

bool CCariotip::init(CCariotip & c)
{
	if (!ocupaCromosomes(c.tamany()))
		return false;
	ompleCromosomesCopiant(c.m_cromosomes);
	return true;
}

bool CCariotip::init(uint32 first, uint32 last)
{
	if (!ocupaCromosomes(last-first+1))
		return false;
	ompleCromosomesSequencialment(first);
	return true;
}

uint32 CCariotip::cromosomaAleatori(void) const
{
	return rnd.get(0,m_cromosomes.size()-1);
}

uint32 CCariotip::tamany() const
{
	return m_cromosomes.size();
}

CCariotip::t_cromosoma & CCariotip::operator[] (uint32 n)
// Pre: No ha d'estar buida i n<this->tamany()
{
	return m_cromosomes[n];
}

void CCariotip::afegeix(t_cromosoma & c, uint32 index)
{
	if (index<m_cromosomes.size())
		m_cromosomes.insert(m_cromosomes.begin()+index, c);
	else
		m_cromosomes.push_back(c);
}

void CCariotip::afegeix(t_cromosoma & c)
{
		m_cromosomes.push_back(c);
}

CCariotip::t_cromosoma CCariotip::extreu(uint32 index)
// Pre: No ha d'estar buida
// Post: Compte! Extreu pero no elimina el cromosoma, el retorna!
{
	t_cromosoma crm;
	if (index<m_cromosomes.size())
	{
		crm=m_cromosomes[index];
		m_cromosomes.erase(m_cromosomes.begin()+index);
	}
	else
	{
		crm = m_cromosomes.back();
		m_cromosomes.pop_back();
	}
	return crm;
}

CCariotip::t_cromosoma CCariotip::extreu()
// Pre: No ha d'estar buida
// Post: Compte! Extreu pero no elimina el cromosoma, el retorna!
{
	t_cromosoma crm;
	crm = m_cromosomes.back();
	m_cromosomes.pop_back();
	return crm;
}

//////////////////////////////////////////////////////////////////////
// Implementacio
//////////////////////////////////////////////////////////////////////

bool CCariotip::initCromosomes()
	{
	if (tracePrivate)
		out << "CCariotip: Initant les variables relatives als cromosomes" << endl;
	return true;
}

bool CCariotip::ocupaCromosomes(uint32 nCromosomes)
// PRE: m_codons=NULL, no hi hauria error pero es perdria la memoria
{
	if (tracePrivate)
		out << "CCariotip: Ocupant espai per " << nCromosomes << " cromosomes" << endl;
	if (!nCromosomes) 
		return false;
	m_cromosomes.assign(nCromosomes, NULL);
/*
	// Si la biblioteca no esta actualitzada aixo serveix com a alternativa a l'assign
	m_cromosomes.clear();
	m_cromosomes.insert(m_cromosomes.begin(), size_t(nCromosomes), (CCromosoma*)NULL);
*/
	return true;
}

void CCariotip::alliberaCromosomes()
{
	if (tracePrivate)
		out << "CCariotip: Alliberant les variables relatives als cromosomes" << endl;
	for (t_cromosomes::iterator it = m_cromosomes.begin(); it!=m_cromosomes.end(); it++)
		if (*it) delete (*it);
	m_cromosomes.clear();
}

void CCariotip::ompleCromosomesCopiant(t_cromosomes cromosomes)
// PRE: sizeof(cromosomes)>=m_nCromosomes
{
	uint32 nCromosomes=m_cromosomes.size();
	while (nCromosomes--)
		if (!m_cromosomes[nCromosomes]) {
			m_cromosomes[nCromosomes] = new CCromosoma;
			if (!m_cromosomes[nCromosomes]) {
				error << "Error omplint el cariotip per copia" << endl;
				cin.get();
			}
			m_cromosomes[nCromosomes]->init(*(cromosomes[nCromosomes]));
		}
}

void CCariotip::ompleCromosomesAleatoriament(uint32 minLong, uint32 maxLong)
{
	uint32 nCromosomes=m_cromosomes.size();
	while (nCromosomes--)
		if (!m_cromosomes[nCromosomes]) {
			m_cromosomes[nCromosomes] = new CCromosoma;
			if (!m_cromosomes[nCromosomes]) {
				error << "Error omplint el cariotip aleatoriament" << endl;
				cin.get();
			}
			m_cromosomes[nCromosomes]->init(rnd.get(minLong,maxLong));
		}
}

void CCariotip::ompleCromosomesSequencialment(uint32 primer)
// PRE: MAXuint32-primer<m_nCromosomes
{
	uint32 nCromosomes=m_cromosomes.size();
	while (nCromosomes--) {
		t_cromosoma & crm = m_cromosomes[nCromosomes];
		if (!crm) {
			crm = new CCromosoma;
			if (!crm) {
				error << "Error omplint el cariotip per debug" << endl;
				cin.get();
			}
			crm->init(nCromosomes+primer,nCromosomes+primer+3);
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CCariotip::ProvaClasse(void)
{
	out << clrscr;
	out << blanc.brillant() << "Provant Cariotip" << blanc.fosc() << endl;
	CCariotip car;
	car.dump(out);
	uint32 i=rnd.get(1,5);
	while (i--) {
		uint32 tamany=rnd.get(1,10), pos=rnd.get(0,5);
		out << "Afegint cromosoma de longitud " << tamany << " a la posicio " << pos << endl;
		CCromosoma * crm = new CCromosoma;
		crm->init(tamany);
		car.afegeix(crm,pos);
		car.dump(out);
	}
	CCariotip car1, car2, car3;
	car1.init(2,4);
	out << "\nCariotip #1" << endl;
	car1.dump(out);
	car2.init(car1);
	out << "\nCariotip #2" << endl;
	car2.dump(out);
	car3.init(8);
	out << "\nCariotip #3" << endl;
	car3.dump(out);
	cin.get();

	out << "Provant mutacio masiva de cariotips fins que premis una tecla" << endl;
	while (!kbhit()) {
		car3.muta();
//		car3.dump(out);
	}
}




bool CCariotip::muta()
{
	if (!CProbabilitat::EsDona(Config.get("Organisme/ProbabilitatMutacio/Encerts"),Config.get("Organisme/ProbabilitatMutacio/Mostra")))
		return false;	
	CMutacioCariotip * mutacio;
	switch (rnd.get(1,1))
	{
	case 0:
		mutacio = CMutacioGenica::Nova(rnd.get(1,CMutacioGenica::Nombre())-1);
		break;
	case 1:
		mutacio = CMutacioCromosomica::Nova(rnd.get(1,CMutacioCromosomica::Nombre())-1);
		break;
	case 2:
		mutacio = CMutacioCariotip::Nova(rnd.get(1,CMutacioCariotip::Nombre())-1);
		break;
	default:
		error << "ops!!! Cas no previst a " << __FILE__ << ":" << __LINE__ << endl;
		cin.get();
		return false;
	}

	if (!mutacio) return false;
//	out << mutacio->tipus() << endl;
	mutacio->muta(*this);
	if (!mutacio) out << "copon!" << endl;
	delete mutacio;
	
	return true;
}
