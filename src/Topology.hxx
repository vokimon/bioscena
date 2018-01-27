// Topology.hxx: interface for the Topology class.
//
//////////////////////////////////////////////////////////////////////

#ifndef Topologia_hxx
#define Topologia_hxx

#include "BioIncludes.h"

namespace Bioscena {

/**
A Topology represents the geometrical features of a discrete biotop.

You can specialize this interface in order to describe your own
geometry for the biotop (the inert media in which life beings live).

A topology defines:

- A position domain (Possible values and its meaning)
- A displacement domain (Possible values and its meaning)
- The relation between them based on two functions:
	- Given a position and a displacement calculate the resulting position.
	- Given two positions calculate the displacement to reach one from the other.

This class also implements a dummy default implementation:
N cells so that they are ramdomly interconected.
@see Torus A reference implementation for a concrete Topology
*/
	class Topology
	{
	// Tipus propis
	public:
		/// The type for position identifiers
		typedef uint32 Position;
		/// The type for topologic displacements
		typedef uint32 Displacement;
	// Atributs
	protected:
		uint32 _size;
	// Construccio/Destruccio
	public:
		Topology(uint32 size=0);
		virtual ~Topology();

	/// @name Overridable
	//@{
	public:
		/// Gives the number of discrete positions that the topology has
		/// @return The number of discrete positions for the topology
		virtual uint32 size() const;

		/// Returns whether a given position is valid for the topology
		/// @param position A position to be checked
		/// @return true if the position is a valid one on the topology
		virtual bool isValidPosition(Position position) const;
		
		/// Returns a random valid position on the topology
		virtual Position randomPosition() const;

		/// Calculates a single movement within the topology
		/// @param origin The origin of a movement
		/// @param relativeMovement A displacement
		/// @return The displaced position
		virtual Position displace(Position origin, Displacement relativeMovement) const;
		
		/// Calculates a randomly displaced position from an origin.
		/// By applying several random displacements you will obtain a normal
		/// distribution arround the origin.
		/// @param origin The origin of a movement
		/// @param radius The number of random displacements to perform
		/// @return The displaced position
		virtual Position displaceRandomly(Position origin, uint32 radius) const;

		/// Calculates the displacement needed in order to go from one position to the other one.
		/// Not always an only displacement is enough to reach the goal but the displacement
		/// gives the direction to get closer.
		/// @param posOrigen The origin position
		/// @param posDesti The goal for the movement
		/// @param desp A reference to the variable where to put the calculated displacement
		/// @returns True if the given displacement is enough to reach the goal.
		virtual bool pathTowards(Position posOrigen, Position posDesti, Displacement & desp) const;
		
		/// Calculates the oposite/mirrored displacement.
		/// @param desp A displacement to be mirror
		/// @returns The mirrored displacement
		virtual Displacement opositeDisplacement(Displacement desp) const;
		
		/// Returns a displacement value that applied to one position it gives the same position.
		/// @returns A null displacement
		virtual Displacement nilDisplacement() const;
		
		/// Returns the number of displacements to reach one position from another.
		/// @param origin The original position
		/// @param destination The final position
		/// @returns The number of displacements
		virtual uint32 distance(Position origin, Position destination) const;
		
	//@}	
	// Testing
	public:
		/// Supervised class test.
		/// @see TopologyTest for automated ones.
		static void ProvaClasse();
	};
}

#endif//Topologia_hxx
