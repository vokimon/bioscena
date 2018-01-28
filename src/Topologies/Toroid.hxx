// Toroid.hxx: interface for the Toroid class.
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
		NoDir = 0b1000  // 8
	};
// Contruccio/Destruccio
public: 
	Toroid (uint32 xMax, uint32 yMax);
	virtual ~Toroid();
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
	inline uint32 distance (Position origin, Position destination) const override;
	Position displaceRandomly (Position origin, uint32 radius) const override;
	inline Displacement opositeDisplacement(Displacement desp) const override;
	inline Displacement nilDisplacement() const override;
// Atributs
protected:
	uint32 m_xMax;
	uint32 m_yMax;
	int32 m_direccions[10]; // Must be signed!
// Proves
public:
	/// Supervised test. @see ToroidTest
	static void ClassTest(void);
};

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

Toroid::Position Toroid::displace(Position origin, Displacement relativeMovement) const
{
	// Descomentar la seguent linia perque el 4art bit del nibble
	// indiqui la seva inibicio i no la seva activacio 
	relativeMovement^=0x88888888u;

	// Les operacions estan fetes amb cura per que funcionin amb topologies
	// amb un nombre elevat de posicions sense que es produeixi overflow. 
	// Si no tenim aquest problema es podria optimitzar

	int moviment=0;
	for (; relativeMovement; relativeMovement>>=4)
		if (relativeMovement&010u)
			moviment += m_direccions [relativeMovement&0x0000007u];

	if (moviment<0) {
		if (origin < uint32(-moviment))
			return _size + moviment + origin;
		else
			return origin + moviment;
		}
	else {
		if (origin+moviment<_size)
			return origin + moviment;
		else
			return origin + moviment - _size;
		}
}

static void applyDisplacement(uint32 & displacement, uint32 N, uint32 direction) {
	for (uint32 i=N;i--;)
		displacement = (displacement<<4) | direction;
}

bool Toroid::wayTo (Position origin, Position destination, Displacement & displacement) const
{
	displacement = 0x88888888u;

	// Reduce it to the problem of being at 0,0
	uint32 normalizedTarget = (destination + size() - origin)%size();

	uint32 xoff = col(normalizedTarget);
	uint32 yoff = row(normalizedTarget);

	Direction horizontal = E;
	if (xoff>m_xMax/2) {
		xoff = m_xMax-xoff;
		yoff +=1;
		horizontal = W;
	}
	Direction vertical = S;
	if (yoff>m_yMax/2) {
		yoff = m_yMax-yoff;
		vertical = N;
	}

	bool reached = true;
	if (yoff>8) {
		reached = false;
		yoff=8;
	}
	if (xoff>8) {
		reached = false;
		xoff=8;
	}

	Direction combined = horizontal==E? (vertical==S?SE:NE) : (vertical==S?SW:NW);
	uint32 ncombined = std::min(xoff, yoff);

	applyDisplacement(displacement, ncombined, combined);
	applyDisplacement(displacement, xoff-ncombined, horizontal);
	applyDisplacement(displacement, yoff-ncombined, vertical);

	displacement &= 0xFFFFFFFF;

	return reached;
}

uint32 Toroid::distance(Position origin, Position destination) const
{
	// TODO: N/S distances
	// TODO: cross distances
	uint32 result = origin<=destination?destination-origin:origin-destination;
	uint32 rows = row(result);
	uint32 cols = col(result);
	if (rows>m_yMax/2) rows = m_yMax-rows;
	if (cols>m_xMax/2) cols = m_xMax-cols;
	return std::max(rows,cols);
}

Toroid::Displacement Toroid::opositeDisplacement(Displacement desp) const
{
	// invert just the 3 lower bits of each nibble
	return desp ^ 0x77777777u;
}

Toroid::Displacement Toroid::nilDisplacement() const
{
	return 0x88888888u;
}

}

#endif //Toroid_hxx
