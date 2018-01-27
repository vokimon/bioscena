// BiotopToroidal.h: interface for the Toroid class.
//
//////////////////////////////////////////////////////////////////////

#ifndef Toroid_hxx
#define Toroid_hxx

#include "BioIncludes.h"
#include "Topology.hxx"
#include "RandomStream.hxx"
#include "Missatger.h"

namespace Bioscena {

/**
	A Toroid is a Topology that arranges cells as a
	rectangular array having cells at opposite borders
	connected as well.
	
	_Positions_ are enumerated starting at zero
	in latin reading order: left to right and top down.

	__Notice:__ This implementation connects the last cell
	of each row, not to the first cell of the same row,
	but to the first cell of the next row.
	This simplifies a lot the math and logic of movements
	since a given direction corresponds to a fixed offset
	whichever the position.

	_Displacements_ encode in its 32 bits a sequence of
	8 micromovements. Each micromovement is a nibble (4 bits),
	where the upper bit disables it, and the lower 3 bits
	indicates a cardinal direction:
	(N,S,E,W,NE,NW,SE,SW).

	_Directions_ are binary encoded so that binary inversion
	gives the opposite direction and that most contiguous
	directions are just one bit inversion away (or two).


	direction|  bits
	---------|------
	N        | 0 000
	NE       | 0 001
	E        | 0 011
	SE       | 0 010
	S        | 0 111
	SW       | 0 110
	W        | 0 100
	NW       | 0 101
	None     | 1 xxx


	@see Topology
*/
class Toroid : public Topology
{
// Tipus interns
public:
	typedef Topology inherited;
	typedef inherited::Position Position;
	typedef inherited::Displacement Displacement;
	/** 
	 * The building block for a displacement vectors.
	 * Note that oposed directions are complementaries 
	 * in order to speed up the oposite calculation.
	 */
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
	Toroid (uint32 XMax, uint32 YMax);
// Operacions
public: 
	void height(uint32 rows) {m_yMax=rows;}
	void width(uint32 cols) {m_xMax=cols;}
	uint32 height() const {return m_yMax;}
	uint32 width() const {return m_xMax;}
// Redefinibles
public: 
	inline Position displace (Position origen, Displacement movimentRelatiu) const override;
	inline bool wayTo (Position posOrigen, Position posDesti, Displacement & desp) const override;
	inline Position displaceRandomly (Position posOrigen, uint32 radi) const override;
	inline Displacement opositeDisplacement(Displacement desp) const override;
	inline Displacement nilDisplacement() const override;
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

Toroid::Position Toroid::displace(Position origen, Displacement movimentRelatiu) const
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
			return (_size - uint32(-moviment)) + origen;
		else
			return origen + moviment;
		}
	else {
		if (_size-origen>uint32(moviment))
			return origen + moviment;
		else
			return (uint32)moviment - (_size - origen);
		}
}

bool Toroid::wayTo (Position posOrigen, Position posDesti, Displacement & displacement) const
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
	displacement=0x00000000;
	basic = esquerra?(adalt?UP_LEFT:DOWN_LEFT):(adalt?UP_RIGHT:DOWN_RIGHT);
	basic|=0x8;
	while (dx&&dy&&nBasics) {
		dx--;
		dy--;
		nBasics--;
		displacement|=basic<<(nBasics<<2);
	}
	basic = dx?(esquerra?LEFT:RIGHT):(adalt?UP:DOWN);
	basic|=0x8;
	while (nBasics&&(dx||dy)) {
		if (dx) dx--;
		if (dy) dy--;
		nBasics--;
		displacement|=basic<<(nBasics<<2);
	}

	return !(dx || dy);
}

Toroid::Position Toroid::displaceRandomly (Position posOrigen, uint32 radi) const
{
	// El radi esta expressat en displacements basics (4 bits) -> en un vector de 
	// displacement (32 bits) hi han 8 de basics.
	// pe. Si tenim radi=45 -> 5 vectors * 8 basics/vector + 5 basics
	// Recorda que el bit de mes pes de cada basic es un 'enabled'.

	// Primer calculem els basics que en sobren
	uint32 posDesti = displace(posOrigen, (rnd.get()|0x88888888) & ~(0xFFFFFFFF>>((radi&7)<<2)));
	// Despres calculem vectors sencers amb 8 basics cadascun 
	for (radi>>=3; radi--;)
		posDesti = displace(posDesti,rnd.get()|0x88888888);
	return posDesti;
}

Toroid::Displacement Toroid::opositeDisplacement(Displacement desp) const {
	return desp ^ 0x77777777;
}

Toroid::Displacement Toroid::nilDisplacement() const {
	return 0;
}

}

#endif // !defined(Toroid_hxx)
