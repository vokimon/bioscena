// Biotop.h: interface for the CBiotop class.
//
//////////////////////////////////////////////////////////////////////
// Change Log
// 20000218 VoK - Creat
// 20000708 VoK - esValidaCassella -> esPosicioValida
// 20000708 VoK - Modificacions als tipus propis

#ifndef __KKEP_BIOTOP_H_INCLUDED
#define __KKEP_BIOTOP_H_INCLUDED

#include <iomanip>
#include "BioIncludes.h"
#include "Topologia.h"
#include "RandomStream.h"
#include "Color.h"

using namespace AnsiCodes;

template<class Cella> 
class CBiotop: public Topology  
{
// Tipus propis
public:
	typedef Topology inherited;
	typedef inherited::t_posicio t_posicio;
	typedef inherited::t_desplacament t_desplacament;
	typedef Cella t_cella;
// Atributs
protected:
	uint32 m_nPosicions;
	t_cella *m_casselles;
	Topology * m_topologia;
// Construccio/Destruccio
public:
	CBiotop(Topology * topologia);
	virtual ~CBiotop();

// Redefinibles
public:
	// TODO: Carrega i descarrega de la topologia
	virtual ostream & store(ostream & str)
	{
		uint32 i;
		str.write((char*)&m_nPosicions,sizeof(uint32));
		for (i=0; i<m_nPosicions; i++) {
			m_casselles[i].store(str);
		}
		return str;
	}
	virtual istream & load(istream & str)
	{
		uint32 i, nPosicions;
		str.read((char*)&nPosicions,sizeof(uint32));
		if (nPosicions != m_topologia->tamany())
			error << "El nombre de posicions no coincideix amb el tamany de la topologia" << endl;
		for (i=0; i<nPosicions; i++) {
			m_casselles[i].load(str);
		}			
		return str;
	}
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
	virtual bool esPosicioValida(t_posicio cassella) 
	// Indica si la posicio es valida
	{
		return m_topologia->esPosicioValida(cassella);
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
	Topology * topologia() const {return m_topologia;}
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
		for (uint32 i=0; i<m_nPosicions; i++)
			stream << m_casselles[i] << "#";
		stream<<endl;
		}

public:
	static void ProvaClasse(void);
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

template<class Cella> CBiotop<Cella>::CBiotop(Topology * topologia)
{
	m_casselles=NULL;
	m_nPosicions=0;
	m_topologia=topologia;
	
	reservaCasselles(m_topologia->tamany());
}
template<class Cella> CBiotop<Cella>::~CBiotop()
{
	if (m_casselles) delete[] m_casselles;

}

//////////////////////////////////////////////////////////////////////
// Implementacio
//////////////////////////////////////////////////////////////////////

template<class Cella> void CBiotop<Cella>::reservaCasselles(uint32 tamany) {
	if (m_casselles) {
//		warning << "Tornant a definir les celles de la topologia." <<endl;
		delete[] m_casselles;
		}
	if (!tamany) {
		error << "Creant un biotop sense casselles." <<endl;
		cin.get();
		}
	m_casselles = new t_cella [m_nPosicions=tamany];
	if (!m_casselles) {
		m_nPosicions = 0;
		error << "No hi ha suficient memoria per les celles de la topologia." << endl;
		cin.get();
		}
}



#endif // !defined(__KKEP_BIOTOP_H_INCLUDED)
