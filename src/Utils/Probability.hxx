// Probability.hxx: interface for the Probability class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(__KKEP_PROBABILITY_H_INCLUDED)
#define __KKEP_PROBABILITY_H_INCLUDED

#include "BioIncludes.h"
#include "RandomStream.hxx"

namespace Bioscena {

/**
* Represents a rational probability.
* 
* A rational probability is equivalent to the popular expression
* used in:
* '9 in 10 programmers agree that 1 in 10 programmers is a lammer'.
* This is a rational probability because in order to calculate the
* probability factor you must buid a fraction with these numbers in
* order to obtain a [0,1] probability factor.
* 
* Rational probabilities are smarter than [0,1] real domain ones
* because you are dealing with integers instead of floating point
* numbers.
* 
* In order to test the probability you must generate a random number
* between 1 and the sample size. If the number is less or equal the
* threshold the probability matches, if the number is greater the
* probability fails.
*/
class Probability 
{
// Construccio/Destruccio
public:
	/**
	* Constructs a Probability object that will be always true.
	* You can modify this behaviour using the fixa function.
	*/
	Probability() {
		m_sampleSize=1;
		m_success=1;
	};
	/**
	* Constructs a Probability object that will test true
	* <tt>success</tt> in <tt>sampleSize</tt>.
	* @param success The number of successes you expect in a given sample
	* @param sampleSize The number of tries
	* @pre The sample size must be greater than 0
	* @pre The success must be not greater than the sample size
	*/
	Probability(uint32 success, uint32 sampleSize) {
		set(success,sampleSize);
	};
	virtual ~Probability() {};
// Operacions
public:
	/**
	* Sets the probability as a fraction between the success
	* and the sample size.
	* The function fixes bad values as needed in order to
	* make the fraction fit in the interval [0,1], so:
	* - if the sample (denominator) is zero it is changed to one
	* - if the given (numerator) is more than the sample, then the
	* given is fixed to be equal.
	*
	* @param success The number of successes you expect in a given sample
	* @param sampleSize The number of tries
	* @pre The sample size must be greater than 0
	* @pre The success must be not greater than the sample size
	* @todo A secure version of this function for loading
	*/
	void set(uint32 success, uint32 sampleSize)
	{
		KKEP_ASSERT(sampleSize, 
			"Probability: Using a zero sample size");
		KKEP_ASSERT(success<=sampleSize, 
			"Probability: Success is greater than the sample size");
		m_sampleSize = sampleSize;
		m_success = success;
	}
	/// Gives the sample size
	uint32 sampleSize() const
	{
		return m_sampleSize;
	}
	/// Returns the median successful cases expected in a sample
	uint32 success() const
	{
		return m_success;
	}
	/// Shortcut for chance
	bool operator() () const
	{
		return chance();
	};
	/// Give a chance for the probability
	bool chance() const
	{
		return m_success >= rnd.get(1,m_sampleSize);
	};
/// @name Compare operators
///@{
public:
	/// @return True if the receiver has a different probability to success than the parameter
	bool operator != (const Probability & p) const {
		return m_success*p.sampleSize() != p.success()*m_sampleSize;
	};
	/// @return True if the receiver has the same probability to success than the parameter
	bool operator == (const Probability & p) const {
		return m_success*p.sampleSize() == p.success()*m_sampleSize;
	};
	/// @return True if the receiver has more probability to success than the parameter
	bool operator > (const Probability & p) const {
		return m_success*p.sampleSize() > p.success()*m_sampleSize;
	};
	/// @return True if the receiver has more probability to success than the parameter
	bool operator >= (const Probability & p) const {
		return m_success*p.sampleSize() >= p.success()*m_sampleSize;
	};
	/// @return True if the receiver has less probability to success than the parameter
	bool operator < (const Probability & p) const {
		return m_success*p.sampleSize() < p.success()*m_sampleSize;
	};
	/// @return True if the receiver has less or equal probability to success than the parameter
	bool operator <= (const Probability & p) const {
		return m_success*p.sampleSize() <= p.success()*m_sampleSize;
	};
///@}
// Atributs
private:
	/// The expected succesfull tests in a sample
	uint32 m_success;
	/// The sample size
	uint32 m_sampleSize;
// Funcions estatiques
public:
	/// A chance for a probability without having to create a Probability object
	static bool Chance(uint32 success, uint32 sampleSize) 
	{
		return success >= rnd.get(1,sampleSize);
	}
};

} // Namespace Bioscena
#endif // !defined(__KKEP_PROBABILITAT_H_INCLUDED)
