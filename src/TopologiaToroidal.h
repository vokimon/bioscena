// BiotopToroidal.h: interface for the CBiotopToroidal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_TOPOLOGIATOROIDAL_H_INCLUDED)
#define __KKEP_TOPOLOGIATOROIDAL_H_INCLUDED

#include "BioIncludes.h"
#include "Topologia.h"
#include "RandomStream.h"
#include "Color.h"

template <class Cella>
class CTopologiaToroidal : public CTopologia<Cella>
{
// Contruccio/Destruccio
public: 
	CTopologiaToroidal (uint32 XMax, uint32 YMax);
// Redefinicio
	virtual t_idCella desplacament (t_idCella origen, t_desplacament movimentRelatiu);
// Atributs
protected:
	uint32 m_xMax;
	uint32 m_yMax;
	int32 m_direccions[10]; // Cal que sigui signed!
// Proves
public:
	template<class output>
	void debugPresenta(output & stream) {
		stream << "\033[2;1H";// Un gotoxy xapuser pero estandard (ANSI)
		uint32 nCella=0;
		for (uint32 j=0;j<m_yMax;j++) {
			for (uint32 i=0;i<m_xMax;i++)
//				stream << m_casselles[nCella++] <<"#";
				stream << m_casselles[nCella++];
			stream<<endl;
			}
		}
public:
	static void ProvaClasse(void) {
		uint32 i;
		int escala[]={7,6,4,4,12,14,15,9};
		CTopologiaToroidal<Cella> topo(25,21);
		uint32 cuc[7]={130,130,130,130,130,130,130};
		while (cuc[0]!=8) {
			for (i=7;i--;) topo[cuc[i]].ocupa(escala[6-i]);
			topo[cuc[6]].desocupa();
			topo[topo.cassellaAlAtzar()].deposita(4);
			topo.debugPresenta(out);
			CRandomStream rnd;
			uint32 direccio;
			rnd>>direccio;
			for (i=6;i--;) cuc[i+1]=cuc[i];
			cuc[0]=topo.desplacament(cuc[0],direccio);
			if (cuc[0]==cuc[6]) cuc[0]=topo.cassellaAlAtzar();
			out<<"Celles"<<setw(5)<<setfill(' ');
			for (i=7;i--;) out << cuc[i] << " - ";
			out<<setw(0)<<endl;
		}
	}
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

template <class Cella>
CTopologiaToroidal<Cella>::CTopologiaToroidal(uint32 xMax=5, uint32 yMax=5)
	// Precondicions:
	//   xMax*yMax < Maxim valor positiu representable amb un uint32.
	//   xMax>2 yMax>2
{
//	cout<<"Inicializando un Substrato Toroidal de "<<xMax <<" X "<<yMax<<endl;
	reservaCasselles(xMax*yMax);
	m_direccions[00]=  0 -xMax;
	m_direccions[01]= +1 -xMax;
	m_direccions[02]= +1 +0   ;
	m_direccions[03]= +1 +xMax;
	m_direccions[07]=  0 +xMax;
	m_direccions[06]= -1 +xMax;
	m_direccions[05]= -1 +0   ;
	m_direccions[04]= -1 -xMax;
	m_xMax=xMax;
	m_yMax=yMax;
}

//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

template <class Cella>
CTopologiaToroidal<Cella>::t_idCella CTopologiaToroidal<Cella>::desplacament(t_idCella origen, t_desplacament movimentRelatiu)
{
	// Segur que es podria fer mes optim pero no funcionaria pels
	// biotops amb un nombre de celles elevat.
	int moviment=0;
	for (uint32 selector=movimentRelatiu>>24; selector; selector>>=1) {
		if (selector&1) 
			moviment += m_direccions [movimentRelatiu&0x0000007];
		movimentRelatiu >>= 3;
	}
	if (moviment<0) {
		if (origen < uint32(-moviment))
			return (m_totalCasselles - uint32(-moviment)) + origen;
		else
			return origen + moviment;
		}
	else {
		if (m_totalCasselles-origen>uint32(moviment))
			return origen + moviment;
		else
			return (uint32)moviment - (m_totalCasselles - origen);
		}
}

#endif // !defined(__KKEP_TOPOLOGIATOROIDAL_H_INCLUDED)
