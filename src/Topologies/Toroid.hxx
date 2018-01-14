// Toroid.hxx: interface for the Toroid class.
//
//////////////////////////////////////////////////////////////////////

#ifndef Toroid_hxx
#define Toroid_hxx

#include "BioIncludes.h"
#include "Topology.hxx"
#include "RandomStream.hxx"
//#include "Missatger.h"

namespace Bioscena {

/**
	A Toroid is a Topology that arranges cells as a
	rectangular array where cells at opposite borders
	are connected as well.
	
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


	direction| bits
	---------|------
	N        | 000
	NE       | 001
	E        | 010
	SE       | 011
	S        | 111
	SW       | 110
	W        | 101
	NW       | 010


	@see Topology
*/
class Toroid : public Bioscena::Topology
{
// Tipus interns
public:
	typedef Topology inherited;
	typedef inherited::Position Position;
	typedef inherited::Displacement Displacement;
	/** 
		The building block for a displacement vectors.
		Note that oposed directions are complementaries
		in order to speed up the oposite calculation.
		Every fouth bit is a disabler of the nibble,
		so nibles beyond 7 are NoDir.
	 */
	enum Direction
	{
		N  = 0,
		NE = 1,
		E  = 2,
		SE = 3,
		S  = 7,
		SW = 6,
		W  = 5,
		NW = 4,
		NoDir = 8
	};
// Contruccio/Destruccio
public: 
	Toroid (uint32 xMax=5, uint32 yMax=5);
// Operacions
public: 
	void height(uint32 rows) {m_yMax=rows;}
	void width(uint32 cols) {m_xMax=cols;}
	/// @return the number of rows for the topology
	uint32 height() const {return m_yMax;}
	/// @return the number of columns for the topology
	uint32 width() const {return m_xMax;}
	/// @return the row corresponding to the given position
	uint32 row(Position pos) const {return pos / m_xMax;}
	/// @return the column corresponding to the given position
	uint32 col(Position pos) const {return pos % m_xMax;}
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
	virtual inline Position displace (Position origin, Displacement relativeMovement) const;
	virtual inline bool wayTo (Position posOrigen, Position posDesti, Displacement & desp) const;
	virtual inline Position displaceRandomly (Position origin, uint32 radius) const;
	virtual inline Displacement opositeDisplacement(Displacement desp) const;
	virtual inline Displacement nilDisplacement() const;
// Atributs
protected:
	uint32 m_xMax;
	uint32 m_yMax;
	int32 m_direccions[10];
// Proves
public:
	static void ProvaClasse(void);
};

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

Toroid::Position Toroid::displace(Position origin, Displacement movimentRelatiu) const
{
	// Descomentar la seguent linia perque el 4art bit del nibble
	// indiqui la seva inibicio i no la seva activacio 
	movimentRelatiu^=0x88888888u;

	// Les operacions estan fetes amb cura per que funcionin amb topologies
	// amb un nombre elevat de posicions sense que es produeixi overflow. 
	// Si no tenim aquest problema es podria optimitzar

	int moviment=0;
	for (; movimentRelatiu; movimentRelatiu>>=4)
		if (movimentRelatiu&010) 
			moviment += m_direccions [movimentRelatiu&0x0000007u];
	// -8*(xMax+1) < moviment < 8*(xMax + 1)

	if (moviment<0) {
		if (origin < uint32(-moviment))
			return m_totalCasselles + moviment + origin;
		else
			return origin + moviment;
		}
	else {
		if (origin+moviment<m_totalCasselles)
			return origin + moviment;
		else
			return origin + moviment - m_totalCasselles;
		}
}

static void applyDisplacement(uint32 & displacement, uint32 N, uint32 direction) {
	for (uint32 i=N;i--;)
		displacement = (displacement<<4) | direction;
}

bool Toroid::wayTo (Position posOrigen, Position posDesti, Displacement & displacement) const
{
	displacement = 0x88888888u;

	// Reduce it to the problem of being at 0,0
	uint32 normalizedTarget = (posDesti + size() - posOrigen)%size();

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

Toroid::Position Toroid::displaceRandomly (Position posOrigen, uint32 radi) const
{
	// El radi esta expressat en displacements basics (4 bits) -> en un vector de 
	// displacement (32 bits) hi han 8 de basics.
	// pe. Si tenim radi=45 -> 5 vectors * 8 basics/vector + 5 basics
	// Recorda que el bit de mes pes de cada basic es un 'enabled'.

	// Primer desplacem els basics que en sobren d'un vector sencer
	unsigned partSteps = radi & 0x7;
	Displacement partDisplacement = (rnd.get()|0x88888888u) ^ (0x88888888u>>(32-(partSteps*4)));
	uint32 posDesti = displace(posOrigen, partDisplacement);
	// Despres calculem vectors sencers amb 8 basics cadascun 
	for (unsigned fullSteps=radi>>=3; fullSteps--;)
		posDesti = displace(posDesti,rnd.get()|0x88888888u);
	return posDesti;
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
