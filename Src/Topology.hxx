// Topologia.h: interface for the CTopologia class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_TOPOLOGIA_H_INCLUDED)
#define __KKEP_TOPOLOGIA_H_INCLUDED

#include "BioIncludes.h"

/**
* A CTopologia encapsulates the geometrical caracteristics for a discrete biotop.
* 
* You can specialize this interface in order to describe your own
* geometry for the biotop (the inert media in which life beings live).
* <p>
* A topology defines:
* <ul>
* <li>A position domain (Possible values and its meaning)
* <li>A displacement domain (Possible values and its meaning)
* <li>The relation between them based on two functions:
* <ul>
* <li>Given a position and a displacement calculate the resulting position.
* <li>Given two positions calculate the displacement to reach one from the other.
* </ul>
* </ul>
* <p>
* This class also implements a dummy default implementation: 
* N cells so that they are ramdomly interconected.
* @see CTopologiaToroidal A reference implementation for a concrete CTopologia
*/
class CTopologia  
{
// Tipus propis
public:
	/// The position identifier type
	typedef uint32 t_posicio;
	/// The displacement type
	typedef uint32 t_desplacament;
// Atributs
protected:
	uint32 m_totalCasselles;
// Construccio/Destruccio
public:
	CTopologia(uint32 tamany=0);
	virtual ~CTopologia();

/// @name Redefinibles
//@{
public:
	/// Gives the number of discrete positions that the topology has
	/// @return The number of discrete positions for the topology
	virtual uint32 tamany() const;

	/// Returns whether a given position is valid for the topology
	/// @param A position
	/// @return true if the position is valid on the topology
	virtual bool esPosicioValida(t_posicio cassella) const;
	
	/// Returns a random valid position on the topology
	virtual t_posicio posicioAleatoria() const;

	/// Calculates a single movement within the topology
	/// @param origen The origin of a movement
	/// @param movimentRelatiu A displacement
	/// @return The displaced position
	virtual t_posicio desplacament(t_posicio origen, t_desplacament movimentRelatiu) const;
	
	/// Calculates a randomly displaced position from an origin.
	/// By applying several random displacements you will obtain a normal
	/// distribution arround the origin.
	/// @param origen The origin of a movement
	/// @param radi The number of random displacements to perform
	/// @return The displaced position
	virtual t_posicio desplacamentAleatori(t_posicio origen, uint32 radi) const;

	/// Calculates the displacement needed in order to go from one position to the other one.
	/// Not always an only displacement is enough to reach the goal but the displacement
	/// gives the direction to get closer.
	/// @param posOrigen The origin position
	/// @param posDesti The goal for the movement
	/// @param desp A reference to the variable where to put the calculated displacement
	/// @returns True if the given displacement is enough to reach the goal.
	virtual bool unio(t_posicio posOrigen, t_posicio posDesti, t_desplacament & desp) const;
	
	/// Calculates the inverse displacement.
	/// Not always an only displacement is enough to reach the goal but the displacement
	/// gives the direction to get closer.
	/// @param posOrigen The origin position
	/// @param posDesti The goal for the movement
	/// @param desp A reference to the variable where to put the calculated displacement
	/// @returns True if the given displacement is enough to reach the goal.
	virtual t_desplacament invers(t_desplacament desp) const;
	
	/// Returns a displacement value that applied to one position it gives the same position.
	/// @returns A null displacement
	virtual t_desplacament desplacamentNul() const;
	
	/// Returns the number of displacements to reach one position from another.
	/// @param posOrigen The original position
	/// @param posDesti The final position
	/// @returns The number of displacements
	virtual uint32 distancia(t_posicio posOrigen, t_posicio posDesti) const;
	
//@}	
// Funcions estatiques
public:
// Proves
public:
	/// The class test
	static void ProvaClasse();
};

#endif // !defined(__KKEP_TOPOLOGIA_H_INCLUDED)