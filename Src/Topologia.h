// Topologia.h: interface for the Topology class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_TOPOLOGIA_H_INCLUDED)
#define __KKEP_TOPOLOGIA_H_INCLUDED

#include "BioIncludes.h"

namespace Bioscena {

/**
* A Topology encapsulates the geometrical caracteristics for a discrete biotop.
* 
* You can specialize this interface in order to describe your own
* geometry for the biotop (the inert media in which life beings live).
* <p>
* A topology defines:
* <ul>
* <li>A position domain (Possible values and its meaning)</li>
* <li>A displacement domain (Possible values and its meaning)</li>
* <li>The relation between them based on two functions:
* <ul>
* <li>Given a position and a displacement calculate the resulting position.</li>
* <li>Given two positions calculate the displacement to reach one from the other.</li>
* </ul>
* </ul>
* <p>
* This class also implements a dummy default implementation: 
* N cells so that they are ramdomly interconected.
* @see Torus A reference implementation for a concrete Topology
*/
	class Topology  
	{
	// Tipus propis
	public:
		/// The type for position identifiers
		typedef uint32 t_position;
		/// The type for topologic displacements
		typedef uint32 t_displacement;
	// Atributs
	protected:
		uint32 m_totalCasselles;
	// Construccio/Destruccio
	public:
		Topology(uint32 tamany=0);
		virtual ~Topology();

	/// @name Redefinibles
	//@{
	public:
		/// Gives the number of discrete positions that the topology has
		/// @return The number of discrete positions for the topology
		virtual uint32 tamany() const;

		/// Returns whether a given position is valid for the topology
		/// @param A position
		/// @return true if the position is valid on the topology
		virtual bool esPosicioValida(t_position cassella) const;
		
		/// Returns a random valid position on the topology
		virtual t_position posicioAleatoria() const;

		/// Calculates a single movement within the topology
		/// @param origen The origin of a movement
		/// @param movimentRelatiu A displacement
		/// @return The displaced position
		virtual t_position desplacament(t_position origen, t_displacement movimentRelatiu) const;
		
		/// Calculates a randomly displaced position from an origin.
		/// By applying several random displacements you will obtain a normal
		/// distribution arround the origin.
		/// @param origen The origin of a movement
		/// @param radi The number of random displacements to perform
		/// @return The displaced position
		virtual t_position desplacamentAleatori(t_position origen, uint32 radi) const;

		/// Calculates the displacement needed in order to go from one position to the other one.
		/// Not always an only displacement is enough to reach the goal but the displacement
		/// gives the direction to get closer.
		/// @param posOrigen The origin position
		/// @param posDesti The goal for the movement
		/// @param desp A reference to the variable where to put the calculated displacement
		/// @returns True if the given displacement is enough to reach the goal.
		virtual bool unio(t_position posOrigen, t_position posDesti, t_displacement & desp) const;
		
		/// Calculates the inverse displacement.
		/// Not always an only displacement is enough to reach the goal but the displacement
		/// gives the direction to get closer.
		/// @param posOrigen The origin position
		/// @param posDesti The goal for the movement
		/// @param desp A reference to the variable where to put the calculated displacement
		/// @returns True if the given displacement is enough to reach the goal.
		virtual t_displacement invers(t_displacement desp) const;
		
		/// Returns a displacement value that applied to one position it gives the same position.
		/// @returns A null displacement
		virtual t_displacement desplacamentNul() const;
		
		/// Returns the number of displacements to reach one position from another.
		/// @param posOrigen The original position
		/// @param posDesti The final position
		/// @returns The number of displacements
		virtual uint32 distancia(t_position posOrigen, t_position posDesti) const;
		
	//@}	
	// Funcions estatiques
	public:
	// Proves
	public:
		/// The class test
		static void ProvaClasse();
	};
}

#endif // !defined(__KKEP_TOPOLOGIA_H_INCLUDED)
