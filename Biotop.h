// Biotop.h: interface for the CBiotop class.
//
//////////////////////////////////////////////////////////////////////
// Change Log
// 20000218 VoK - Creat

#if !defined(__KKEP_BIOTOP_H_INCLUDED)
#define __KKEP_BIOTOP_H_INCLUDED

#include <iomanip>
#include "BioIncludes.h"
#include "RandomStream.h"
#include "Color.h"

using namespace AnsiCodes;

template<class Cella> 
class CBiotop: public CTopologia  
{
// Tipus propis
public:
//	typedef uint32 t_posicio;
//	typedef uint32 t_desplacament;
	typedef Cella t_cella;
// Atributs
protected:
	uint32 m_totalCasselles;
	Cella *m_casselles;
	CTopologia * m_topologia;
// Construccio/Destruccio
public:
	CBiotop(CTopologia * topologia);
	virtual ~CBiotop();

// Redefinibles
public:
	virtual t_posicio desplacament (t_posicio origen, t_desplacament movimentRelatiu)
	// Retorna la posicio resultant de fer el desplacament des de l'origen
	{
		return m_topologia->desplacament(origen, movimentRelatiu);
	}
	virtual t_posicio desplacamentAleatori (t_posicio origen, uint32 radi)
	// Retorna la posicio resultant de tants desplacaments aleatoris des de l'origen com indiqui el radi
	{
		return m_topologia->desplacamentAleatori(origen, radi);
	}
	virtual bool esValidaCassella(t_posicio cassella) 
	// Indica si la posicio es valida
	{
		return m_topologia->esValidaCassella(cassella);
	}
	virtual bool unio (t_posicio posOrigen, t_posicio posDesti, t_desplacament & desp)
	// Retorna cert si es posible unir-les amb un sol desplacament, a desp hi es
	// el desplacament per unir-les o apropar-les
	{
		return m_topologia->unio(posOrigen, posDesti, desp);
	}
	virtual t_posicio posicioAleatoria() 
	// Retorna una posicio aleatoria
	{
		return m_topologia->posicioAleatoria();
	}
	virtual uint32 tamany() 
	// Retorna el nombre de casselles de la topologia
	{
		return m_topologia->tamany();
	}
// Operacions
public:
	t_cella &operator [] (t_posicio index) 
	{
		if ((index>=tamany())||(index<0)) {
			error << "Accedint a una cella de la Topologia no existent" << endl;
			cin.get();
		}
		return m_casselles[index];
	}
// Implementacio
protected:
	void reservaCasselles(uint32 tamany);
// Proves
public:
	virtual void debugPresenta(CMissatger & stream) {
		stream << gotoxy(2,1);
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

template<class Cella> CBiotop<Cella>::CBiotop(CTopologia * topologia)
{
	m_casselles=NULL;
	m_totalCasselles=0;
	m_topologia=topologia;
	
	reservaCasselles(m_topologia->tamany());
}
template<class Cella> CTopologia<Cella>::~CTopologia()
{
	if (m_casselles) delete[] m_casselles;

}

//////////////////////////////////////////////////////////////////////
// Implementacio
//////////////////////////////////////////////////////////////////////

template<class Cella> void CBiotop<Cella>::reservaCasselles(uint32 tamany) {
	if (m_casselles) {
		warning << "Tornant a definir les celles de la topologia." <<endl;
		delete[] m_casselles;
		}
	if (!tamany) {
		error << "Creant un biotop sense casselles." <<endl;
		cin.get();
		}
	m_casselles = new Cella [m_totalCasselles=tamany];
	if (!m_casselles) {
		m_totalCasselles = 0;
		error << "No hi ha suficient memoria per les celles de la topologia." << endl;
		cin.get();
		}
}

#endif // !defined(__KKEP_BIOTOP_H_INCLUDED)
