// Distribucio.h: interface for the Distribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_DISTRIBUTION_H_INCLUDED)
#define __KKEP_DISTRIBUTION_H_INCLUDED

#include "BioIncludes.h"
#include "RandomStream.h"

namespace Bioscena {

/**
* A natural numbers probability distribution based on the role game dices metafore.
* The distribution is parametrized as this:
* <ul>
* <li>(m) The minimum value you can obtain
* <li>(n) The number of dices that will be added to this number
* <li>(M) The magnitude of the dices. They will score from 0 to M
* </ul>
* Some deductable caracteristics of the obtained distribution are that:
* <ul>
* <li>The maximum value is m + M*n
* <li>If you use only one dice, you will get an uniform distribution between the maximum and the minumum
* <li>When you increase the number of dices, the distributions aproximates to a normal distribution 
* with an increasing typical deviation and the median arround the center ( m + (n * M)/2 ).
* </ul>
* @see Roulette
* @see Probability
* @see sonCompatibles
*/

class Distribution
{
/// @name Construction/Destruction
/// @{
public:
	/**
	* The Distribution constructor.
	* @param minumum The minimum value of the distribution
	* @param dices The number of dices that will be added to the minimum
	* @param magnitude The maximum value of the distribution
	*/
	Distribution(uint32 minimum = 0, uint32 dices=0, uint32 magnitude=1) :
		m_minim(minimum),
		m_magnitudDau(magnitude),
		m_nombreDaus(dices)
	{
	};
	virtual ~Distribution() {};
/// @}

/// @name Extraction
/// @{
public:
	/**
	* This conversion operator is used to get a distribution value.
	* It allows to use the distribution as a number in a expression.
	*/
	uint32 operator() ()
	{
		// Inicialitzem amb el valor minim
		uint32 val=m_minim;
		// I li afegim l'extra dels daus
		for (uint32 i=m_nombreDaus; i--;)
			val+=rnd.get(0,m_magnitudDau);
		return val;
	}
/// @}

/// @name Configuration
/// @{
public:
	/// Sets the minimum value
	void minimum(uint32 minim) {
		m_minim=minim;
	}
	/// Gets the minimum value
	uint32 minimum() const {
		return m_minim;
	}
	/// Sets the dices number
	void dices(uint32 dices) {
		m_nombreDaus=dices;
	}
	/// Gets the dices number
	uint32 dices() const {
		return m_nombreDaus;
	}
	/// Sets the dices magnitude
	void dicesMagnitude(uint32 magnitude) {
		m_magnitudDau=magnitude;
	}
	/// Gets the dices magnitude
	uint32 dicesMagnitude() const {
		return m_magnitudDau;
	}
/// @}

// Attributes
private:
	uint32 m_minim;
	uint32 m_magnitudDau;
	uint32 m_nombreDaus;
};

}

#endif // !defined(__KKEP_DISTRIBUTION_H_INCLUDED)
