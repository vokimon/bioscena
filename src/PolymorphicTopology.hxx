// Topologia.h: interface for the Topology class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_POLYMORPHIC_TOPOLOGY_H_INCLUDED)
#define __KKEP_POLYMORPHIC_TOPOLOGY_H_INCLUDED

#include "BioIncludes.h"

namespace Bioscena {

/**
* I am a place-holder topology that allows to keep using run-time (hence, normal) 
* polymorfism in a place where the polimorphism is fixed on compile time 
* (hence templates).
*
* Use me as template parameter for a topology templated class/function/whatever
* when you dont want to fix on compile time the kind of topology to be used.
* If the kind of topology is known on compile time i am a bad option because
* my performance is worst than using the concrete topology directly.
* All my methods are inlined, and they delegate their tasks to the concrete
* topology using a virtual funtion call, so my costs are no less and no more
* than using normal polymorfism instead using static polymorfism... well
* maybe a derreference in each call...
*/
	class PolymorphicTopology  
	{
	//Attributes
	private:
		Topology * m_topology;
	// Construccio/Destruccio
	public:
		PolymorphicTopology(Topology & topology) {m_topology = &topology;};
		virtual ~PolymorphicTopology();

	/// @name Redefinibles
	//@{
	public:
		/// Gives the number of discrete positions that the topology has
		/// @return The number of discrete positions for the topology
		virtual uint32 size() const 
			{return m_topology->size();}

		/// Returns whether a given position is valid for the topology
		/// @param position A position to be checked
		/// @return true if the position is a valid one on the topology
		virtual bool isValidPosition(Position position) const
			{return m_topology->isValidPosition(position);}
		
		/// Returns a random valid position on the topology
		virtual Position randomPosition() const
			{return m_topology->randomPosition();}

		/// Calculates a single movement within the topology
		/// @param origin The origin of a movement
		/// @param relativeMovement A displacement
		/// @return The displaced position
		virtual Position displace(Position origin, Displacement relativeMovement) const
			{return m_topology->displace(origin, relativeMovement);}
		
		/// Calculates a randomly displaced position from an origin.
		/// By applying several random displacements you will obtain a normal
		/// distribution arround the origin.
		/// @param origin The origin of a movement
		/// @param radius The number of random displacements to perform
		/// @return The displaced position
		virtual Position displaceRandomly(Position origin, uint32 radius) const
			{return m_topology->displaceRandomly(origin, radius);}

		/// Calculates the displacement needed in order to go from one position to the other one.
		/// Not always an only displacement is enough to reach the goal but the displacement
		/// gives the direction to get closer.
		/// @param posOrigen The origin position
		/// @param posDesti The goal for the movement
		/// @param desp A reference to the variable where to put the calculated displacement
		/// @returns True if the given displacement is enough to reach the goal.
		virtual bool wayTo(Position posOrigen, Position posDesti, Displacement & desp) const
			{return m_topology->wayTo(posOrigen, posDesti, desp);}
		
		/// Calculates the oposite/mirrored displacement.
		/// @param desp A displacement to be mirror
		/// @returns The mirrored displacement
		virtual Displacement opositeDisplacement(Displacement desp) const
			{return m_topology->opositeDisplacement(desp);}
		
		/// Returns a displacement value that applied to one position it gives the same position.
		/// @returns A null displacement
		virtual Displacement nilDisplacement() const
			{return m_topology->nilDisplacement();}
		
		/// Returns the number of displacements to reach one position from another.
		/// @param origin The original position
		/// @param destination The final position
		/// @returns The number of displacements
		virtual uint32 distance(Position origin, Position destination) const;
			{return m_topology->distance(origin, destination);}
		
	//@}	
	// Funcions estatiques
	public:
	// Proves
	public:
		/// The class test
		static void ProvaClasse();
	};
}

#endif // !defined(__KKEP_POLYMORPHIC_TOPOLOGY_H_INCLUDED)
