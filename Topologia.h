// Topologia.h: interface for the CTopologia class.
//
//////////////////////////////////////////////////////////////////////
// Change Log
// 19990722 VoK - Fa servir templates per a les cel·les
// 19990723 VoK - Rebautizat: BiotopToroidal -> TopologiaToroidal

#if !defined(__KKEP_TOPOLOGIA_H_INCLUDED)
#define __KKEP_TOPOLOGIA_H_INCLUDED

#include "BioIncludes.h"
#include "RandomStream.h"

template<class Cella> 
class CTopologia  
{
// Atributs
protected:
	uint32 m_totalCasselles;
	Cella *m_casselles;
// Construccio/Destruccio
public:
	CTopologia();
	CTopologia(uint32 tamany);
	virtual ~CTopologia();
// Operacions
public:
	Cella &operator [] (uint32 index) 
	{
		if ((index>=m_totalCasselles)||(index<0)) {
			error<<"Accedint a una cella del Topologia no existent"<< endl;
			assert((index<m_totalCasselles)||(index>=0));
		}
		return m_casselles[index];
	}
	bool esValidaCassella(uint32 cassella) {return (cassella<m_totalCasselles)||(cassella>=0);};
	uint32 cassellaAlAtzar() {
		CRandomStream rnd;
		return rnd.get(0,m_totalCasselles-1);
	}
	virtual uint32 desplacament (uint32 origen, uint32 movimentRelatiu);
// Implementacio
protected:
	void reservaCasselles(uint32 tamany);
// Proves
public:
template<class output>
void debugPresenta(output & stream) {
	stream << "\033[1;1H";// Un gotoxy xapuser pero standard (ANSI)
	for (uint32 i=0;i<m_totalCasselles;i++)
		stream << m_casselles[i]<<"#";
	stream<<endl;
	}
public:
	static void ProvaClasse(void) {
		uint32 i;
		int escala[]={7,6,4,4,12,14,15,9};
		CTopologia<Cella> topologia(400);
		uint32 cuc[7]={130,130,130,130,130,130,130};
		while (cuc[0]!=8) {
			for (i=7;i--;) topologia[cuc[i]]=escala[6-i];
			topologia[topologia.cassellaAlAtzar()]=9;
			topologia.debugPresenta(out);
			CRandomStream rnd;
			uint32 direccio;
			rnd>>direccio;
			for (i=6;i--;) cuc[i+1]=cuc[i];
			cuc[0]=topologia.desplacament(cuc[0],direccio);
			if (cuc[0]==cuc[6]) cuc[0]=topologia.cassellaAlAtzar();
			out<<"Celles"<<setw(5)<<setfill(' ');
			for (i=7;i--;) out << cuc[i] << " - ";
			out<<setw(0)<<endl;
		}
	}
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

template<class Cella> CTopologia<Cella>::CTopologia()
{
	m_casselles=NULL;
	m_totalCasselles=0;
}
template<class Cella> CTopologia<Cella>::CTopologia(uint32 tamany)
{
	m_casselles=NULL;
	m_totalCasselles=0;
	reservaCasselles(tamany);
}
template<class Cella> CTopologia<Cella>::~CTopologia()
{

}
template<class Cella> void CTopologia<Cella>::reservaCasselles(uint32 tamany) {
	if (m_casselles) {
		warning << "Tornant a definir les celles de la topologia." <<endl;
		delete[] m_casselles;
		}
	try {
		m_casselles = new Cella [m_totalCasselles=tamany];
		}
	catch (...) 
	{
		m_casselles = NULL;
		m_totalCasselles = 0;
		error << "No hi ha suficient memoria per les celles de la topologia."<<endl;
	}
}

template<class Cella> uint32 
CTopologia<Cella>::desplacament (uint32 origen, uint32 movimentRelatiu)
// Aquesta funcio es la que determina la proximitat de les celles
// per omisio totes les celles estan aillades.
{
	return cassellaAlAtzar();
//	return origen;
}

#endif // !defined(__KKEP_TOPOLOGIA_H_INCLUDED)
