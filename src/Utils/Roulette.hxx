// Roulette.h: interface for the Roulette class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(__KKEP_ROULETTE_H_INCLUDED)
#define __KKEP_ROULETTE_H_INCLUDED

#include "BioIncludes.h"
#include "RandomStream.hxx"

namespace Bioscena {

/**
* A probability distribution among a set of natural numbers based on the roulette metafore.
* A roulette distribution is a distribution where the likehood of taking a
* value is proportional to the size of the roulette sector you assign to it.
* So the greater a sector is, the more likely to get it choosed.
* @todo Implement and use it
* @see Distribution
* @see Probability
* @see sonCompatibles
*/

class Roulette
{
public:
	typedef uint32 t_size;
	typedef uint32 t_value;
	typedef std::map<t_value, t_size> t_sectors;
	typedef std::vector<t_value> t_values;
/// @name Construction/Destruction
/// @{
public:
	/**
	* The Roulette constructor.
	*/
	Roulette() {m_maximum=0;};
	/**
	* The Roulette virtual destructor.
	*/
	virtual ~Roulette() {};
/// @}

/// @name Operators
/// @{
public:
        /**
	* This conversion operator is used to get a distribution value.
	* It allows to use the distribution as a number in a expression.
	* @todo Optimize roulette, check greater sectors first
	*/
	operator t_value()
	{
		// Throw the needle
		t_size neddle=Random::Get(0,m_maximum-1);
		return spin(neddle);
	}
	/**
	 * Returns the sector chosen if we spin the roulette with that force
	 * @pre force &lt; summatory of sector sizes
	 */
	t_value spin(t_size force) {
		// Search the selected
		t_size accomulated=0;
		for (
			t_sectors::const_iterator it = sectors.begin();
			it!=sectors.end();
			it++
			) 
		{
			accomulated += it->second;
			if (accomulated>force) return it->first;
		}
		KKEP_ASSERT(false,"Out of bounds of the roulette");
		return 0;
	}
	
/// @}

/// @name Configuration
/// @{
public:
	/// Gets the sector size for a value
	/// @todo Not yet implemented
	t_size sector(t_value value) const {
		t_sectors::const_iterator it = sectors.find(value);
		if (it== sectors.end()) return 0;
		return it->second;
	}
	/// Sets the sector size for a value
	/// @todo Not yet implemented
	void sector(t_value value, t_size size) {
		t_sectors::iterator it = sectors.find(value);
		if (it== sectors.end()) {
			sectors.insert(std::make_pair(value,size));
			m_maximum+=size;
			return;
		}
		m_maximum-=it->second;
		m_maximum+=size;
		it->second=size;
		if (!size) {
			sectors.erase(it);
			return;
		}
	}
	/// Increase the sector size an amount
	/// @returns the resulting size for the sector
	/// @todo Not yet implemented
	t_size increase(t_value value, t_size amount) {
		// TODO
		return 0;
	}
	/// Decrease the sector size an amount
	/// @returns the resulting size for the sector
	/// @todo Not yet implemented
	t_size decrease(t_value value, t_size amount) {
		// TODO
		return 0;
	}
/// @}

/// @name Testing
/// @{
public:
	bool checkInvariant() {
		t_size acomulator=0;
		for (
			t_sectors::const_iterator it = sectors.begin();
			it!=sectors.end();
			it++
			) 
		{
			acomulator += it->second;
		}
		if (acomulator!=m_maximum) return false;
		return true;
	}
	
	/// @todo Not yet implemented
	static void ClassTest();
/// @}

// Attributes
private:
	t_size m_maximum;
	t_sectors sectors;
};

} // Namespace Bioscena
#endif // !defined(__KKEP_ROULETTE_H_INCLUDED)
