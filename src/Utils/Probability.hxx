// Probability.hxx: interface for the Probability class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(__KKEP_PROBABILITY_H_INCLUDED)
#define __KKEP_PROBABILITY_H_INCLUDED

#include "BioIncludes.h"
#include "RandomStream.h"

namespace Bioscena {

/**
* Represents a rational probability.
* <p>
* A rational probability is equivalent to the popular expression
* used in:
* '9 in 10 programmers agree that 1 in 10 programmers is a lammer'.
* This is a rational probability because in order to calculate the
* probability factor you must buid a fraction with these numbers in
* order to obtain a [0,1] probability factor.
* <p>
* Rational probabilities are smarter than [0,1] real domain ones
* because you are dealing with integers instead of floating point
* numbers.
* <p>
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
		m_mostra=1;
		m_encerts=1;
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
	* <ul>
	* <li>if the sample (denominator) is zero it is changed to one
	* <li>if the given (numerator) is more than the sample, then the
	* given is fixed to be equal.
	* </ul>
	* @param success The number of successes you expect in a given sample
	* @param sampleSize The number of tries
	* @pre The sample size must be greater than 0
	* @pre The success must be not greater than the sample size
	* @todo Use asserts and isValid instead corrections or a secure version of the function
	*/
	void set(uint32 success, uint32 sampleSize)
	{
		KKEP_ASSERT(sampleSize, "Probability: A zero sample size");
		KKEP_ASSERT(success<=sampleSize, "Probability: Success is greater than the sample size");
		// La mostra no pot ser 0 (probabilitat infinita??)
		m_mostra = sampleSize;
		// Els encerts han de ser inferiors o iguals a la mostra
		// (probabilitat fora de linterval [0,1])
		m_encerts = success;
	}
	/// Gives the sample size
	uint32 sampleSize() const
	{
		return m_mostra;
	}
	/// Returns the median successful cases expected in a sample
	uint32 success() const
	{
		return m_encerts;
	}
	/// A chance for the probability
	bool operator() () const
	{
		return chance();
	};
	/// A chance for the probability
	bool chance() const
	{
		return m_encerts >= rnd.get(1,m_mostra);
	};
/// @name Compare operators
///@{
public:
	/// @return True if the receiver has a different probability to success than the parameter
	bool operator != (const Probability & p) const {
		return m_encerts*p.sampleSize() != p.success()*m_mostra;
	};
	/// @return True if the receiver has the same probability to success than the parameter
	bool operator == (const Probability & p) const {
		return m_encerts*p.sampleSize() == p.success()*m_mostra;
	};
	/// @return True if the receiver has more probability to success than the parameter
	bool operator > (const Probability & p) const {
		return m_encerts*p.sampleSize() > p.success()*m_mostra;
	};
	/// @return True if the receiver has more probability to success than the parameter
	bool operator >= (const Probability & p) const {
		return m_encerts*p.sampleSize() >= p.success()*m_mostra;
	};
	/// @return True if the receiver has less probability to success than the parameter
	bool operator < (const Probability & p) const {
		return m_encerts*p.sampleSize() < p.success()*m_mostra;
	};
	/// @return True if the receiver has less or equal probability to success than the parameter
	bool operator <= (const Probability & p) const {
		return m_encerts*p.sampleSize() <= p.success()*m_mostra;
	};
///@}
// Atributs
private:
	/// The expected succesfull tests in a sample
	uint32 m_encerts;
	/// The sample size
	uint32 m_mostra;
// Funcions estatiques
public:
	/// A chance for a probability without having to create a Probability object
	static bool Chance(uint32 encerts, uint32 mostra) 
	{
		return encerts >= rnd.get(1,mostra);
	}
};

} // Namespace Bioscena
#endif // !defined(__KKEP_PROBABILITAT_H_INCLUDED)
