// Roulette.h: interface for the CRoulette class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_ROULETTE_H_INCLUDED)
#define __KKEP_ROULETTE_H_INCLUDED

#include "BioIncludes.h"
#include "RandomStream.h"

/**
* A natural numbers probability distribution based on the roulette metafore.
* A roulette distribution is a distribution where the likehood of taking a
* value is proportional to the size of the roulette sector you assign to it.
* So the greater a sector is, the more likely to get it choosed.
* @todo Implement and use it
* @see CDistribucio
* @see CProbabilitat
* @see sonCompatibles
*/

class CRoulette
{
/// @name Construction/Destruction
/// @{
public:
	/**
	* The CRoulette constructor.
	*/
	CRoulette() {};
	/**
	* The CRoulette virtual destructor.
	*/
	virtual ~CRoulette() {};
/// @}

/// @name Operators
/// @{
public:
        /**
	* This conversion operator is used to get a distribution value.
	* It allows to use the distribution as a number in a expression.
	/// @todo Not yet implemented
	*/
	operator uint32()
	{
		// Throw the needle
		uint32 neddle=rnd.get(1,m_maximum);
		// Search the selected
		// TODO: Search the selected sector
		return 0;
	}
/// @}

/// @name Configuration
/// @{
public:
	/// Sets the sector size for a value
	/// @todo Not yet implemented
	void sector(uint32 value, uint32 size) {
		// TODO
	}
	/// Increase the sector size an amount
	/// @returns the resulting size for the sector
	/// @todo Not yet implemented
	uint32 increase(uint32 value, uint32 amount) {
		// TODO
	}
	/// Decrease the sector size an amount
	/// @returns the resulting size for the sector
	/// @todo Not yet implemented
	uint32 decrease(uint32 value, uint32 amount) {
		// TODO
	}
	/// Gets the sector size for a value
	/// @todo Not yet implemented
	uint32 sector(uint32 value) const {
		// TODO
	}
/// @}

/// @name Testing
/// @{
public:
	/// @todo Not yet implemented
	static void ClassTest();
/// @}

// Attributes
private:
	uint32 m_maximum;
};

#endif // !defined(__KKEP_ROULETTE_H_INCLUDED)
