// BiotopToroidal.h: interface for the CBiotopToroidal class.
//
//////////////////////////////////////////////////////////////////////
// Change Log
// 19990722 VoK - Fa servir templates per a les cel·les
// 19990722 VoK - La superclasse reserva les casselles
// 19990723 VoK - Rebautizat: BiotopToroidal -> TopologiaToroidal
// 19991130 VoK - Adaptada desplacament aleatori per radis en desp. basics
// 20000112 VoK - Fix: desp.aleat: el pic el calculava al reves
// 20000120 VoK - Ja no es un template
// 20000120 VoK - Fix lexic: Altura -> alcada
// 20000708 VoK - esValidaCassella -> esPosicioValida
//////////////////////////////////////////////////////////////////////

#ifndef __KKEP_TOPOLOGIATOROIDAL_H_INCLUDED
#define __KKEP_TOPOLOGIATOROIDAL_H_INCLUDED

#include "BioIncludes.h"
#include "Topologia.h"
#include "RandomStream.h"
#include "Missatger.h"


/**
* The CTopogogiaToroidal class is an optimized implementation 
* of Topology interface that represents a toroidal topology. 
* That is a 2D mesh of cells so that when you reach the border
* you get in the oposite border.
* <p>
* It has the particularity that when you reach the end of a
* row of cells you get the next row like it were a twisted
* string of cells. So if you move step by step in one cardinal
* direction, you will visit every position on the topology.
*
* TODO: Document how the displacement works here
*
* @see Topology
*/
class Torus : public Topology
{
// Tipus interns
public:
	typedef Topology inherited;
	typedef inherited::t_posicio t_posicio;
	typedef inherited::t_desplacament t_desplacament;
	enum t_DireccionsBasiques 
	{
		UP        = 0x00,
		UP_RIGHT  = 0x01,
		RIGHT     = 0x02,
		DOWN_RIGHT= 0x03,
		DOWN	  = 0x07,
		DOWN_LEFT = 0x06,
		LEFT      = 0x05,
		UP_LEFT   = 0x04
	};
// Contruccio/Destruccio
public: 
	Torus (uint32 XMax, uint32 YMax);
// Operacions
public: 
	uint32 amplada() const {return m_xMax;}
	uint32 alcada() const {return m_yMax;}
// Redefinibles
public: 
	virtual inline t_posicio desplacament (t_posicio origen, t_desplacament movimentRelatiu) const;
	virtual inline bool unio (t_posicio posOrigen, t_posicio posDesti, t_desplacament & desp) const;
	virtual inline t_posicio desplacamentAleatori (t_posicio posOrigen, uint32 radi) const;
	virtual inline t_desplacament invers(t_desplacament desp) const;
	virtual inline t_desplacament desplacamentNul() const;
// Atributs
protected:
	uint32 m_xMax;
	uint32 m_yMax;
	int32 m_direccions[10]; // Cal que sigui signed!
// Proves
public:
	static void ProvaClasse(void);
};

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

Torus::t_posicio Torus::desplacament(t_posicio origen, t_desplacament movimentRelatiu) const
{
	// Descomentar la seguent linia perque el 4art bit del nibble
	// indiqui la seva inibicio i no la seva activacio 
//	movimentRelatiu^=0x88888888;

	// Les operacions estan fetes amb cura per que funcionin amb topologies
	// amb un nombre elevat de posicions sense que es produeixi overflow. 
	// Si no tenim aquest problema es podria optimitzar
	int moviment=0;
	for (; movimentRelatiu; movimentRelatiu>>=4)
		if (movimentRelatiu&010) 
			moviment += m_direccions [movimentRelatiu&0x0000007];

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

bool Torus::unio (t_posicio posOrigen, t_posicio posDesti, t_desplacament & desplacament) const
{
	uint32 x1 = posOrigen % m_xMax;
	uint32 y1 = posOrigen / m_xMax;
	uint32 x2 = posDesti % m_xMax;
	uint32 y2 = posDesti / m_xMax;
//	out << "Origen: " << x1 << "@" << y1 << " Desti: " << x2 << "@" << y2 << endl;
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
//	out << "dx: " << dx << (esquerra?"L":"R")<< " dy: " << dy << (adalt?"U":"D") << endl;

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

Torus::t_posicio Torus::desplacamentAleatori (t_posicio posOrigen, uint32 radi) const
{
	// El radi esta expressat en desplacaments basics (4 bits) -> en un vector de 
	// desplacament (32 bits) hi han 8 de basics.
	// pe. Si tenim radi=45 -> 5 vectors * 8 basics/vector + 5 basics
	// Recorda que el bit de mes pes de cada basic es un 'enabled'.

	// Primer calculem els basics que en sobren
	uint32 posDesti = desplacament(posOrigen, (rnd.get()|0x88888888) & ~(0xFFFFFFFF>>((radi&7)<<2)));
	// Despres calculem vectors sencers amb 8 basics cadascun 
	for (radi>>=3; radi--;)
		posDesti = desplacament(posDesti,rnd.get()|0x88888888);
	return posDesti;
}

Torus::t_desplacament Torus::invers(t_desplacament desp) const {
	return desp ^ 0x77777777;
}

Torus::t_desplacament Torus::desplacamentNul() const {
	return 0;
}

#endif // !defined(__KKEP_TOPOLOGIATOROIDAL_H_INCLUDED)
