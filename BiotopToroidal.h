// BiotopToroidal.h: interface for the CBiotopToroidal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_BIOTOPTOROIDAL_H_INCLUDED)
#define __KKEP_BIOTOPTOROIDAL_H_INCLUDED

#include "BioIncludes.h"
#include "Biotop.h"
#include "RandomStream.h"
#include "Color.h"

template <class Cella>
class CBiotopToroidal : public CBiotop<Cella>
{
// Contruccio/Destruccio
public:
	CBiotopToroidal (uint32 XMax, uint32 YMax);
// Redefinicio
	virtual uint32 desplacament (uint32 origen, uint32 movimentRelatiu);
// Atributs
protected:
	uint32 m_xMax;
	uint32 m_yMax;
	int32 m_direccions[10]; // Cal que sigui signed!
// Proves
protected:
	template<class output>
	void debugPresenta(output & stream) {
		stream << "\033[1;1H";// Un gotoxy xapuser pero standard (ANSI)
		uint32 nCella=0;
		for (uint32 j=0;j<m_yMax;j++) {
			for (uint32 i=0;i<m_xMax;i++)
				stream << m_casselles[nCella++] <<"#";
			stream<<endl;
			}
		}
public:
	static void ProvaClasse(void) {
		uint32 i;
		int escala[]={7,6,4,4,12,14,15,9};
		CBiotopToroidal<Cella> biotop(25,21);
		uint32 cuc[7]={130,130,130,130,130,130,130};
		while (cuc[0]!=8) {
			for (i=7;i--;) biotop[cuc[i]]=escala[6-i];
			biotop[biotop.cassellaAlAtzar()]=vermell.brillant().fons(groc);
			biotop.debugPresenta(out);
			CRandomStream rnd;
			uint32 direccio;
			rnd>>direccio;
			for (i=6;i--;) cuc[i+1]=cuc[i];
			cuc[0]=biotop.desplacament(cuc[0],direccio);
			if (cuc[0]==cuc[6]) cuc[0]=biotop.cassellaAlAtzar();
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
CBiotopToroidal<Cella>::CBiotopToroidal(uint32 xMax=5, uint32 yMax=5)
{
	// Precondicions:
	//   xMax*yMax < Maxim valor positiu representable amb un uint32.
	//   xMax>2 yMax>2
	cout<<"Inicializando un Substrato Toroidal de "<<xMax <<" X "<<yMax<<endl;
	reservaCasselles(xMax*yMax);
	m_direccions[0]=  0 -xMax;
	m_direccions[1]= +1 -xMax;
	m_direccions[2]= +1 +0   ;
	m_direccions[3]= +1 +xMax;
	m_direccions[4]=  0 +xMax;
	m_direccions[5]= -1 +xMax;
	m_direccions[6]= -1 +0   ;
	m_direccions[7]= -1 -xMax;
	m_xMax=xMax;
	m_yMax=yMax;
}

//////////////////////////////////////////////////////////////////////
// Debugging (a comentar en la versio definitiva)
//////////////////////////////////////////////////////////////////////

template <class Cella>
uint32 CBiotopToroidal<Cella>::desplacament(uint32 origen, uint32 movimentRelatiu)
{
	// Es podria fer mes optim pero no funcionaria pels
	// biotops amb un nombre de celles elevat.
	int moviment=0;
	moviment += m_direccions [(movimentRelatiu&0x0007)];
	moviment += m_direccions [(movimentRelatiu&0x0038)>>3];
	moviment += m_direccions [(movimentRelatiu&0x01C0)>>6];
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

#endif // !defined(__KKEP_BIOTOPTOROIDAL_H_INCLUDED)
