// Distribucio.h: interface for the Distribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_DISTRIBUTION_H_INCLUDED)
#define __KKEP_DISTRIBUTION_H_INCLUDED

#include "BioIncludes.h"
#include "RandomStream.hxx"

namespace Bioscena {

/**
* A natural numbers probability distribution based on the role game dices metafore.
* The distribution is parametrized as this:
* - (m) The minimum value you can obtain
* - (n) The number of dices that will be added to this number
* - (M) The magnitude of the dices. They will score from 0 to M
*
* Some deductable caracteristics of the obtained distribution are that:
* - The maximum value is m + M*n
* - If you use only one dice, you will get an uniform distribution between the maximum and the minimum
* - When you increase the number of dices, the distributions aproximates to a normal distribution 
* with an increasing standarstandardtion and the median arround the center ( m + (n * M)/2 ).
*
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
	* @param minimum The minimum value of the distribution
	* @param dices The number of dices that will be added to the minimum
	* @param magnitude The maximum value of the distribution
	*/
	Distribution(uint32 minimum = 0, uint32 dices=0, uint32 magnitude=1) :
		m_minimum(minimum),
		m_dicesMagnitude(magnitude),
		m_numberOfDices(dices)
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
		uint32 val=m_minimum;
		for (uint32 i=m_numberOfDices; i--;)
			val+=rnd.get(0,m_dicesMagnitude);
		return val;
	}
/// @}

/// @name Configuration
/// @{
public:
	/// Sets the minimum value
	void minimum(uint32 minimum) {
		m_minimum=minimum;
	}
	/// Gets the minimum value
	uint32 minimum() const {
		return m_minimum;
	}
	/// Sets the dices number
	void dices(uint32 dices) {
		m_numberOfDices=dices;
	}
	/// Gets the dices number
	uint32 dices() const {
		return m_numberOfDices;
	}
	/// Sets the dices magnitude
	void dicesMagnitude(uint32 magnitude) {
		m_dicesMagnitude=magnitude;
	}
	/// Gets the dices magnitude
	uint32 dicesMagnitude() const {
		return m_dicesMagnitude;
	}
/// @}

// Attributes
private:
	uint32 m_minimum;
	uint32 m_dicesMagnitude;
	uint32 m_numberOfDices;
};

}

#endif // !defined(__KKEP_DISTRIBUTION_H_INCLUDED)
