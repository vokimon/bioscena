// Probability.test.cxx: regresion test for the Probability class.
//
//////////////////////////////////////////////////////////////////////
// TODO:
// Chance with zero success 'never' successes
// Chance with sampleSize of success 'always' successes
// Chance with N% success successes N% of the tries
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "Probability.hxx"
#include <time.h>

using Bioscena::Probability;

namespace CppUnit {
	template <>
	struct assertion_traits<Bioscena::Probability> {
		static bool equal( const Bioscena::Probability& x, const Bioscena::Probability& y )
		{
			return x == y;
		}

		static std::string toString( const Bioscena::Probability& x )
		{
			OStringStream ost;
			ost << "(" << x.success() << " over " << x.sampleSize() << ")";
			return ost.str();
		}
	};
}

namespace BioscenaTest {

class ProbabilityBuildingTest;
CPPUNIT_TEST_SUITE_REGISTRATION(ProbabilityBuildingTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ProbabilityBuildingTest, "ProbabilityTests");

class ProbabilityComparationTest;
CPPUNIT_TEST_SUITE_REGISTRATION(ProbabilityComparationTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ProbabilityComparationTest, "ProbabilityTests");

//class ProbabilityTest;
//CPPUNIT_TEST_SUITE_REGISTRATION(ProbabilityTest);
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ProbabilityTest, "ProbabilityTests");

#define assertBehavesLikeStrictlyOrderedPair(lower, higher) \
	{\
		CPPUNIT_ASSERT(!(higher ==  lower));\
		CPPUNIT_ASSERT(  higher !=  lower );\
		CPPUNIT_ASSERT(!(higher <   lower));\
		CPPUNIT_ASSERT(  higher >   lower );\
		CPPUNIT_ASSERT(  higher >=  lower );\
		CPPUNIT_ASSERT(!(higher <=  lower));\
		CPPUNIT_ASSERT(!( lower == higher));\
		CPPUNIT_ASSERT(   lower != higher );\
		CPPUNIT_ASSERT(   lower <  higher );\
		CPPUNIT_ASSERT(!( lower >  higher));\
		CPPUNIT_ASSERT(!( lower >= higher));\
		CPPUNIT_ASSERT(   lower <= higher );\
	}\

#define assertBehavesLikeEquivalentPair(p1, p2) \
	{\
		CPPUNIT_ASSERT(  p1 == p2 );\
		CPPUNIT_ASSERT(!(p1 != p2));\
		CPPUNIT_ASSERT(!(p1 <  p2));\
		CPPUNIT_ASSERT(!(p1 >  p2));\
		CPPUNIT_ASSERT(  p1 >= p2 );\
		CPPUNIT_ASSERT(  p1 <= p2 );\
		CPPUNIT_ASSERT(  p2 == p1 );\
		CPPUNIT_ASSERT(!(p2 != p1));\
		CPPUNIT_ASSERT(!(p2 <  p1));\
		CPPUNIT_ASSERT(!(p2 >  p1));\
		CPPUNIT_ASSERT(  p2 >= p1 );\
		CPPUNIT_ASSERT(  p2 <= p1 );\
	}\

class ProbabilityComparationTest : public CppUnit::TestCase {
	typedef CppUnit::TestCase super;
	CPPUNIT_TEST_SUITE( ProbabilityComparationTest );
	CPPUNIT_TEST( testIdenticalBehaveLikeEquivalent );
	CPPUNIT_TEST( testProportionalBehaveLikeEquivalent );
	CPPUNIT_TEST( testDifferentAlwaysTrueBehaveLikeEquivalent );
	CPPUNIT_TEST( testDifferentAlwaysFalseBehaveLikeEquivalent );
	CPPUNIT_TEST( testSameDenominatorDifferentNumerator );
	CPPUNIT_TEST( testDifferentDenominatorOrderedNumerators );
	CPPUNIT_TEST( testDifferentDenominatorUnorderedNumerators );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
	}
	void tearDown() 
	{
	}
protected:
	void testIdenticalBehaveLikeEquivalent() {
		Probability identical1(3,10);
		Probability identical2(3,10);
		assertBehavesLikeEquivalentPair(identical1, identical2);
	}
	void testProportionalBehaveLikeEquivalent() {
		Probability threeOnTen(3,10);
		Probability sixOnTwenty(6,20);
		assertBehavesLikeEquivalentPair(threeOnTen, sixOnTwenty);
	}
	void testDifferentAlwaysTrueBehaveLikeEquivalent() {
		Probability sevenOnSeven(7,7);
		Probability thirtyOnThirty(30,30);
		assertBehavesLikeEquivalentPair(sevenOnSeven, thirtyOnThirty);
	}
	void testDifferentAlwaysFalseBehaveLikeEquivalent() {
		Probability zeroOnSeven(0,7);
		Probability zeroOnThirty(0,30);
		assertBehavesLikeEquivalentPair(zeroOnSeven, zeroOnThirty);
	}
	void testSameDenominatorDifferentNumerator() {
		Probability threeOnTen(3,10);
		Probability fourOnTen(4,10);
		assertBehavesLikeStrictlyOrderedPair(threeOnTen,fourOnTen);
	}
	void testDifferentDenominatorOrderedNumerators() {
		Probability twoOnFive(2,5);
		Probability threeOnTen(3,10);
		assertBehavesLikeStrictlyOrderedPair(threeOnTen,twoOnFive);
	}
	void testDifferentDenominatorUnorderedNumerators() {
		Probability threeOnTen(3,10);
		Probability oneOnSix(1,6);
		assertBehavesLikeStrictlyOrderedPair(oneOnSix,threeOnTen);
	}

};

class ProbabilityBuildingTest : public CppUnit::TestCase {
	typedef CppUnit::TestCase super;
	CPPUNIT_TEST_SUITE( ProbabilityBuildingTest );
	CPPUNIT_TEST( testValidConstructorsValuesCanBeRetrievedByAccessors );
	CPPUNIT_TEST( testDefaultProbabilityIsAlwaysTrue );
	CPPUNIT_TEST( testConstruct_WithZeroSampleSizeFails );
	CPPUNIT_TEST( testConstruct_WithGreaterSuccessFails );
	CPPUNIT_TEST( testSet_WithZeroSampleSizeFails );
	CPPUNIT_TEST( testSet_WithGreaterSuccessFails );
	CPPUNIT_TEST( testCopyConstructor_ConstructsEquivalent );
	CPPUNIT_TEST( testAssignOperator_MakesAssignedEquivalent );
	CPPUNIT_TEST_SUITE_END();

protected:

	void testValidConstructorsValuesCanBeRetrievedByAccessors() {
		Probability fiveOnSix(5,6);
		CPPUNIT_ASSERT_EQUAL(uint32(5),fiveOnSix.success());
		CPPUNIT_ASSERT_EQUAL(uint32(6),fiveOnSix.sampleSize());
	}

	void testConstructors_WithProportionalValuesAreNotSimplified() {
		Probability fiveOnSix(3,6);
		CPPUNIT_ASSERT_EQUAL(uint32(3),fiveOnSix.success());
		CPPUNIT_ASSERT_EQUAL(uint32(6),fiveOnSix.sampleSize());
	}

	void testDefaultProbabilityIsAlwaysTrue() {
		Probability defaultConstructed;
		Probability alwaysTrue(1,1);
		CPPUNIT_ASSERT_EQUAL(alwaysTrue,defaultConstructed);
	}

	void testCopyConstructor_ConstructsEquivalent() {
		Probability eightOnTen(8,10);
		Probability aCopy(eightOnTen);
		CPPUNIT_ASSERT_EQUAL(eightOnTen,aCopy);
	}

	void testAssignOperator_MakesAssignedEquivalent() {
		Probability eightOnTen(8,10);
		Probability toBeAssigned(4,5);
		toBeAssigned=eightOnTen;
		CPPUNIT_ASSERT_EQUAL(eightOnTen,toBeAssigned);
	}

	void testConstruct_WithZeroSampleSizeFails() {
		try {
			Probability zeroSampleSizeProbability(0,0);
			CPPUNIT_FAIL("Setting zero sample size should throw an assertion");
		} catch (Bioscena::ErrAssertionFailed) 
		{
			// TODO: Check message
		}
	}

	void testConstruct_WithGreaterSuccessFails() {
		try {
			Probability greaterSuccessProbability(3,2);
			CPPUNIT_FAIL("Setting a success greater than the sample size should throw an assertion");
		} catch (Bioscena::ErrAssertionFailed) 
		{
			// TODO: Check message
		}
	}

	void testSet_WithZeroSampleSizeFails() {
		Probability zeroSampleSizeProbability;
		try {
			zeroSampleSizeProbability.set(0,0);
			CPPUNIT_FAIL("Setting zero sample size should fail");
		} catch (Bioscena::ErrAssertionFailed) {
			// TODO: Check message
		}
	}

	void testSet_WithGreaterSuccessFails() {
		Probability greaterSuccessProbability;
		try {
			greaterSuccessProbability.set(3,2);
			CPPUNIT_FAIL(
				"Setting a success greater than the sample size should throw an assertion");
		} catch (Bioscena::ErrAssertionFailed) {
			// TODO: Check message
		}
	}

};

class ProbabilityTest : public CppUnit::TestCase {
	typedef CppUnit::TestCase super;
	CPPUNIT_TEST_SUITE( ProbabilityTest );
//	CPPUNIT_TEST( testPerformance );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
	}
	void tearDown() 
	{
	}
protected:
	void testPerformance() {
		const uint32 N = 0xfff;
		const uint32 success=3;
		const uint32 sample=10;
		ostream & out = std::cout;
		out 
			<< "Performance and success ratio tendency for a " 
			<< success << "/" <<sample << " probability..." << std::endl;
		{
			uint32 encerts=0;
			time_t t = time(NULL);
			for (uint32 i = N; i--;) {
				if (Probability(success,sample)()) encerts++;
			}
			out << "Temporary object -"
				<< " Time:" << time(NULL)-t 
				<< " Encerts: " << encerts 
				<< " Mostra: " << N 
				<< " Ratio: " << ((double)encerts*100/N) 
				<< std::endl;
		}
		{
			uint32 encerts=0;
			time_t t = time(NULL);
			for (uint32 i = N; i--;) {
				if (Probability::Chance(success,sample)) encerts++;
			}
			out << "Static function -" 
				<< " Time:" << time(NULL)-t 
				<< " Encerts: " << encerts 
				<< " Mostra: " << N 
				<< " Ratio: " << ((double)encerts*100/N) 
				<< std::endl;
		}
		{
			uint32 encerts=0;
			time_t t = time(NULL);
			for (uint32 i = N; i--;) {
				if (Probability::Chance(3,10)) encerts++;
			}
			out << "Static function inmediate -" 
				<< " Time:" << time(NULL)-t 
				<< " Encerts: " << encerts 
				<< " Mostra: " << N 
				<< " Ratio: " << ((double)encerts*100/N) 
				<< std::endl;
		}
		{
			uint32 encerts=0;
			Probability p(success,sample);
			time_t t = time(NULL);
			for (uint32 i = N; i--;) {
				if (p.chance()) encerts++;
			}
			out << "Stack object -"
				<< " Time:" << time(NULL)-t 
				<< " Encerts: " << encerts 
				<< " Mostra: " << N 
				<< " Ratio: " << ((double)encerts*100/N) 
				<< std::endl;
		}
	}
};


}
