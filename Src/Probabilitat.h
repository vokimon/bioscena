// Probabilitat.h: interface for the CProbabilitat class.
//
//////////////////////////////////////////////////////////////////////
// 19991018 VoK - Creat
// 19991116 VoK - Afegits els accessors de lectura encerts() i mostra()
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_PROBABILITAT_H_INCLUDED)
#define __KKEP_PROBABILITAT_H_INCLUDED

#include "BioIncludes.h"
#include "RandomStream.h"

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
* <p>
* @attention Because of the implicit conversion to bool, you must
* be careful, to mix probability in other kind of expressions like
* comparing two CProbabilitat objects.
*/
class CProbabilitat 
{
// Construccio/Destruccio
public:
	/**
	* Constructs a CProbabilitat object that will be always true.
	* You can modify this behaviour using the fixa function.
	*/
	CProbabilitat() {
		m_mostra=1;
		m_encerts=1;
	};
	/**
	* Constructs a CProbabilitat object that will test true
	* <tt>success</tt> in <tt>sampleSize</tt>.
	* @param success The number of successes you expect in a given sample
	* @param sampleSize The number of tries
	* @pre The sample size must be greater than 0
	* @pre The success must be not greater than the sample size
	*/
	CProbabilitat(uint32 success, uint32 sampleSize) {
		KKEPAssert(sampleSize, "CProbabilitat: A zero sample size");
		KKEPAssert(success<=sampleSize, "CProbabilitat: Success is greater than the sample size");
		m_mostra=sampleSize;
		m_encerts=success;
	};
	virtual ~CProbabilitat();
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
	*/
	void fixa(uint32 success, uint32 sampleSize)
	{
		// La mostra no pot ser 0 (probabilitat infinita??)
		m_mostra = sampleSize?sampleSize:1;
		// Els encerts han de ser inferiors o iguals a la mostra
		// (probabilitat fora de linterval [0,1])
		m_encerts = sampleSize<success?sampleSize:success;
	}
	/// Gives the sample size
	uint32 mostra() const
	{
		return m_mostra;
	}
	/// Returns the median successful cases expected in a sample
	uint32 encerts() const
	{
		return m_encerts;
	}
	/// A chance for the probability
	operator bool() const
	{
		return esDona();
	};
	/// A chance for the probability
	bool esDona() const
	{
		return m_encerts >= rnd.get(1,m_mostra);
	};
/// @name Compare operators
///@{
public:
	/// @return True if the receiver has a different probability to success than the parameter
	bool operator != (const CProbabilitat & p) const {
//		cout << "Intermediate !=: " << m_encerts*p.mostra() << " " << p.encerts()*m_mostra << std::endl;
		return m_encerts*p.mostra() != p.encerts()*m_mostra;
	};
	/// @return True if the receiver has the same probability to success than the parameter
	bool operator == (const CProbabilitat & p) const {
		return m_encerts*p.mostra() == p.encerts()*m_mostra;
	};
	/// @return True if the receiver has more probability to success than the parameter
	bool operator > (const CProbabilitat & p) const {
		return m_encerts*p.mostra() > p.encerts()*m_mostra;
	};
	/// @return True if the receiver has more probability to success than the parameter
	bool operator >= (const CProbabilitat & p) const {
		return m_encerts*p.mostra() >= p.encerts()*m_mostra;
	};
	/// @return True if the receiver has less probability to success than the parameter
	bool operator < (const CProbabilitat & p) const {
		return m_encerts*p.mostra() < p.encerts()*m_mostra;
	};
	/// @return True if the receiver has less or equal probability to success than the parameter
	bool operator <= (const CProbabilitat & p) const {
		return m_encerts*p.mostra() <= p.encerts()*m_mostra;
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
	/// A chance for a probability without having to create a CProbabilitat object
	static bool EsDona(uint32 encerts, uint32 mostra) 
	{
		return encerts >= rnd.get(1,mostra);
	}
	/**
	* The test for the class.
	* Test includes:
	* <ul>
	* <li>Testing comparation operands for some significative couples
	* <li>Testing the success ratio tendency along a lot of executions (Tester assisted)
	* <li>Performance test between several usage patterns
	* </ul>
	*/
	static void TestClass();
};

#endif // !defined(__KKEP_PROBABILITAT_H_INCLUDED)
