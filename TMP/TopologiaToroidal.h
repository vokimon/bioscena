// BiotopToroidal.h: interface for the CBiotopToroidal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_TOPOLOGIATOROIDAL_H_INCLUDED)
#define __KKEP_TOPOLOGIATOROIDAL_H_INCLUDED

#include "BioIncludes.h"
#include "Topologia.h"
#include "RandomStream.h"
#include "Missatger.h"
#include "Color.h"

enum DireccionsBasiques 
{
	UP		  = 0x00,
	UP_RIGHT  = 0x01,
	RIGHT     = 0x02,
	DOWN_RIGHT= 0x03,
	DOWN	  = 0x07,
	DOWN_LEFT = 0x06,
	LEFT      = 0x05,
	UP_LEFT   = 0x04
};


template <class Cella>
class CTopologiaToroidal : public CTopologia<Cella>
{
// Contruccio/Destruccio
public: 
	CTopologiaToroidal (uint32 XMax, uint32 YMax);
// Redefinibles
	virtual t_posicio desplacament (t_posicio origen, t_desplacament movimentRelatiu);
	virtual bool unio (t_posicio posOrigen, t_posicio posDesti, t_desplacament & desp);
// Atributs
protected:
	uint32 m_xMax;
	uint32 m_yMax;
	int32 m_direccions[10]; // Cal que sigui signed!
// Proves
public:
	void debugPresenta(CMissatger & stream) {
		stream << gotoxy(1,2);
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
			topo[topo.posicioAleatoria()].deposita(4);
			topo.debugPresenta(out);
			CRandomStream rnd;
			uint32 direccio;
			rnd>>direccio;
			for (i=6;i--;) cuc[i+1]=cuc[i];
			cuc[0]=topo.desplacament(cuc[0],direccio);
			if (cuc[0]==cuc[6]) cuc[0]=topo.posicioAleatoria();
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
	//   nD<xMax<0x1FFFFFFF on nD es el nombre maxim de desplacaments basics a un desplacament complert
	//   2<yMax<(0xFFFFFFFF>>digitsSignificatius(xMax))
{
//	cout<<"Inicializando un Substrato Toroidal de "<<xMax <<" X "<<yMax<<endl;
	reservaCasselles(xMax*yMax);
	m_direccions[UP        ]=  0 -xMax;
	m_direccions[UP_RIGHT  ]= +1 -xMax;
	m_direccions[RIGHT     ]= +1 +0   ;
	m_direccions[DOWN_RIGHT]= +1 +xMax;
	m_direccions[DOWN      ]=  0 +xMax;
	m_direccions[DOWN_LEFT ]= -1 +xMax;
	m_direccions[LEFT      ]= -1 +0   ;
	m_direccions[UP_LEFT   ]= -1 -xMax;
	m_xMax=xMax;
	m_yMax=yMax;
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

template <class Cella>
CTopologiaToroidal<Cella>::t_posicio CTopologiaToroidal<Cella>::desplacament(t_posicio origen, t_desplacament movimentRelatiu)
{
	// Segur que es podria fer mes optim pero no funcionaria pels
	// biotops amb un nombre de celles elevat.
	int moviment=0;
/*	for (uint32 selector=movimentRelatiu>>24; selector; selector>>=1) {
		if (selector&1) 
			moviment += m_direccions [movimentRelatiu&0x0000007];
		movimentRelatiu >>= 3;
	}
*/

	// Descomentar la seguent linia per 4art bit inibicio en comptes 
	// d'activacio del nibble
//	movimentRelatiu^=0x88888888; 
	for (; movimentRelatiu; movimentRelatiu>>=4) {
		if (movimentRelatiu&010) 
			moviment += m_direccions [movimentRelatiu&0x0000007];
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

template <class Cella>
bool CTopologiaToroidal<Cella>::unio (t_posicio posOrigen, t_posicio posDesti, t_desplacament & desplacament)
{
	uint32 x1 = posOrigen % m_xMax;
	uint32 y1 = posOrigen / m_xMax;
	uint32 x2 = posDesti % m_xMax;
	uint32 y2 = posDesti / m_xMax;
	out << "Origen: " << x1 << "@" << y1 << " Desti: " << x2 << "@" << y2 << endl;
	bool adalt, esquerra;
	uint32 dx, dy;
	uint32 inner, outer;
	
	// TODO: Correccions per a les espirals horitzontals

	if (x1<x2) {
		inner = x2-x1; // Camino interior, se hace hacia derecha
		outer = (m_xMax-x2)+x1; // Camino exterior, se hace hacia izquerda
		esquerra=outer<inner;
		dx=esquerra?outer:inner;
		if (esquerra && dx>x1) // Correccio espiralitat horitzontal
			y1--;
	}
	else
	{
		inner = x1-x2;// Camino interior, se hace hacia izquierda
		outer = (m_xMax-x1)+x2; // Camino exterior, se hace hacia derecha
		esquerra=inner<outer; 
		dx=esquerra?inner:outer;
		if (!esquerra && x1>m_xMax-dx) // Correccio espiralitat horitzontal
			y1++;
	}

	if (y1<y2) {
		inner = y2-y1; // Camino interior, se hace hacia abajo
		outer = (m_yMax-y2)+y1; // Camino exterior, se hace hacia arriba
		adalt=outer<inner;
		dy=adalt?outer:inner;
	}
	else
	{
		inner = y1-y2; // Camino interior, se hace hacia arriba
		outer = (m_yMax-y1)+y2; // Camino exterior, se hace hacia abajo
		adalt=inner<outer;
		dy=(adalt)?inner:outer;
	}

	uint32 basic;
	out << "dx: " << dx << (esquerra?"L":"R")<< " dy: " << dy << (adalt?"U":"D") << endl;

	uint32 nBasics=8;
	desplacament=0x00000000;
	basic = esquerra?(adalt?UP_LEFT:DOWN_LEFT):(adalt?UP_RIGHT:DOWN_RIGHT);
	basic|=0x8;
	while (dx&&dy&&nBasics) {
		dx--;
		dy--;
		nBasics--;
		desplacament|=basic<<(nBasics<<2);
	}
	basic = dx?(esquerra?LEFT:RIGHT):(adalt?UP:DOWN);
	basic|=0x8;
	while (nBasics&&(dx||dy)) {
		if (dx) dx--;
		if (dy) dy--;
		nBasics--;
		desplacament|=basic<<(nBasics<<2);
	}

	return !(dx || dy);
}

#endif // !defined(__KKEP_TOPOLOGIATOROIDAL_H_INCLUDED)
