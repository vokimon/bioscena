// BiotopToroidal.h: interface for the Toroid class.
//
//////////////////////////////////////////////////////////////////////

#ifndef Toroid_hxx
#define Toroid_hxx

#include "BioIncludes.h"
#include "Topology.hxx"
#include "RandomStream.hxx"

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
	/** 
		The building block for a displacement vectors.
		Note that oposed directions are complementaries
		in order to speed up the oposite calculation.
		Every fouth bit is a disabler of the nibble,
		so nibles beyond 7 are NoDir.
	 */
	enum Direction
	{
		N     = 0b0000, // 0
		NE    = 0b0001, // 1
		E     = 0b0011, // 3
		SE    = 0b0010, // 2
		S     = 0b0111, // 7
		SW    = 0b0110, // 6
		W     = 0b0100, // 4
		NW    = 0b0101, // 5
		NoDir = 0b1000,  // 8
		// Deprecated ones
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
	/// Number of rows of the topology
	uint32 height() const {return m_yMax;}
	/// Number of columns for the topology
	uint32 width() const {return m_xMax;}
	/// The row corresponding to the given position
	uint32 row(Position pos) const {return pos / m_xMax;}
	/// The column corresponding to the given position
	uint32 col(Position pos) const {return pos % m_xMax;}
	/// Translates row and column into position
	Position posAt(uint32 row, uint32 col) const {return row*m_xMax+col;}
	/// Contstructs a displacement given a sequence of cardinal directions
	static Displacement displaceVector(
			Direction d0=NoDir,
			Direction d1=NoDir,
			Direction d2=NoDir,
			Direction d3=NoDir,
			Direction d4=NoDir,
			Direction d5=NoDir,
			Direction d6=NoDir,
			Direction d7=NoDir
			)
	{
		return (d0 | d1<<4 | d2<<8 | d3<< 12 | d4<< 16 | d5<<20 | d6<<24 | d7<<28)&0xFFFFFFFF;
	}
// Redefinibles
public: 
	inline Position displace (Position origin, Displacement relativeMovement) const override;
	inline bool wayTo (Position origin, Position destination, Displacement & desp) const override;

	inline Position displaceRandomly (Position origin, uint32 radius) const override;
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

Toroid::Position Toroid::displace(Position origin, Displacement relativeMovement) const
{
	// Descomentar la seguent linia perque el 4art bit del nibble
	// indiqui la seva inibicio i no la seva activacio 
//	relativeMovement^=0x88888888;

	// Les operacions estan fetes amb cura per que funcionin amb topologies
	// amb un nombre elevat de posicions sense que es produeixi overflow. 
	// Si no tenim aquest problema es podria optimitzar
	int moviment=0;
	for (; relativeMovement; relativeMovement>>=4)
		if (relativeMovement&010) 
			moviment += m_direccions [relativeMovement&0x0000007];

	if (moviment<0) {
		if (origin < uint32(-moviment))
			return (_size - uint32(-moviment)) + origin;
		else
			return origin + moviment;
		}
	else {
		if (_size-origin>uint32(moviment))
			return origin + moviment;
		else
			return (uint32)moviment - (_size - origin);
		}
}

bool Toroid::wayTo (Position origin, Position destination, Displacement & displacement) const
{
	uint32 x1 = origin % m_xMax;
	uint32 y1 = origin / m_xMax;
	uint32 x2 = destination % m_xMax;
	uint32 y2 = destination / m_xMax;
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

Toroid::Position Toroid::displaceRandomly (Position origin, uint32 radius) const
{
	// El radius esta expressat en displacements basics (4 bits) -> en un vector de 
	// displacement (32 bits) hi han 8 de basics.
	// pe. Si tenim radius=45 -> 5 vectors * 8 basics/vector + 5 basics
	// Recorda que el bit de mes pes de cada basic es un 'enabled'.

	// Primer calculem els basics que en sobren
	uint32 destination = displace(origin, (rnd.get()|0x88888888) & ~(0xFFFFFFFF>>((radius&7)<<2)));
	// Despres calculem vectors sencers amb 8 basics cadascun 
	for (radius>>=3; radius--;)
		destination = displace(destination,rnd.get()|0x88888888);
	return destination;
}

Toroid::Displacement Toroid::opositeDisplacement(Displacement desp) const
{
	return desp ^ 0x77777777u;
}

Toroid::Displacement Toroid::nilDisplacement() const
{
	return 0x88888888u;
}

}

#endif // !defined(Toroid_hxx)
