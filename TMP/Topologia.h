// Topologia.h: interface for the CTopologia class.
//
//////////////////////////////////////////////////////////////////////
// Change Log
// 19990722 VoK - Fa servir templates per a les cel·les
// 19990723 VoK - Rebautizat: BiotopToroidal -> TopologiaToroidal

#if !defined(__KKEP_TOPOLOGIA_H_INCLUDED)
#define __KKEP_TOPOLOGIA_H_INCLUDED

#include <iomanip>
#include "BioIncludes.h"
#include "RandomStream.h"

template<class Cella> 
class CTopologia  
{
// Tipus propis
public:
	typedef uint32 t_posicio;
	typedef uint32 t_desplacament;
	typedef Cella t_cella;
// Atributs
protected:
	uint32 m_totalCasselles;
	Cella *m_casselles;
// Construccio/Destruccio
public:
	CTopologia();
	CTopologia(uint32 tamany);
	virtual ~CTopologia();

// Redefinibles
public:
	virtual t_posicio desplacament (t_posicio origen, t_desplacament movimentRelatiu)
		// Retorna la posicio resultant de fer el desplacament des de l'origen
	{
		// TODO: Per defecte aillades o indeterministic
		return posicioAleatoria();
//		return origen;
	}
	virtual bool esValidaCassella(t_posicio cassella) 
	{
		return (cassella<m_totalCasselles)&&(cassella>=0);
	}
	virtual bool unio (t_posicio posOrigen, t_posicio posDesti, t_desplacament & desp)
		// Retorna cert si es posible unir-les amb un sol desplacament, a desp hi es
		// el desplacament per unir-les o apropar-les
	{
		rnd >> desp;
		return false;
	}
	virtual t_posicio posicioAleatoria() 
	{
		CRandomStream rnd;
		return rnd.get(0,m_totalCasselles-1);
	}
// Operacions
public:
	t_cella &operator [] (t_posicio index) 
	{
		if ((index>=m_totalCasselles)||(index<0)) {
			error<<"Accedint a una cella del Topologia no existent"<< endl;
			assert((index<m_totalCasselles)||(index>=0));
		}
		return m_casselles[index];
	}
	template <class t_predicat> t_desplacament explora 
		(t_posicio origen, t_desplacament direccio, uint32 radi, t_predicat pred)
	{
		uint32 base=desplacament(origen, desplacament);
		for (uint32 i=10; i--;) {
			uint32 desp=rnd.get();
			if (pred(m_casselles[desplacament(base, desp)]))
				return desp;
		}
	}
// Implementacio
protected:
	void reservaCasselles(uint32 tamany);
// Proves
public:
	virtual void debugPresenta(CMissatger & stream) {
		stream << gotoxy(1,2);
		for (uint32 i=0;i<m_totalCasselles;i++)
			stream << m_casselles[i]<<"#";
		stream<<endl;
		}
public:
	static void ProvaClasse(void) {
		uint32 i;
		int escala[]={7,6,4,4,12,14,15,9};
		CTopologia<t_cella> topologia(400);
		t_posicio cuc[7]={130,130,130,130,130,130,130};
		while (cuc[0]!=8) {
			for (i=7;i--;) topologia[cuc[i]]=escala[6-i];
			topologia[topologia.posicioAleatoria()]=9;
			topologia.debugPresenta(out);
			CRandomStream rnd;
			t_desplacament direccio;
			rnd>>direccio;
			for (i=6;i--;) cuc[i+1]=cuc[i];
			cuc[0]=topologia.desplacament(cuc[0],direccio);
			if (cuc[0]==cuc[6]) cuc[0]=topologia.posicioAleatoria();
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

//////////////////////////////////////////////////////////////////////
// Implementacio
//////////////////////////////////////////////////////////////////////

template<class Cella> void CTopologia<Cella>::reservaCasselles(uint32 tamany) {
	if (m_casselles) {
		warning << "Tornant a definir les celles de la topologia." <<endl;
		delete[] m_casselles;
		}
	m_casselles = new Cella [m_totalCasselles=tamany];
	if (!m_casselles)
	{
		m_casselles = NULL; // TODO: Treure aquesta perogrullada
		m_totalCasselles = 0;
		error << "No hi ha suficient memoria per les celles de la topologia."<<endl;
		cin.get();
	}
}


#endif // !defined(__KKEP_TOPOLOGIA_H_INCLUDED)
