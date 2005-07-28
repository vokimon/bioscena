// Karyotype.test.cxx: regresion test for the Karyotype class.
//
//////////////////////////////////////////////////////////////////////
// TODO:
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "Karyotype.hxx"

namespace BioscenaTest {

class KaryotypeMutationsTest;
CPPUNIT_TEST_SUITE_REGISTRATION(KaryotypeMutationsTest);

class KaryotypeMutationsTest : public CppUnit::TestFixture {
	typedef CppUnit::TestCase super;
	CPPUNIT_TEST_SUITE( KaryotypeMutationsTest );
	CPPUNIT_TEST( testPositiveAneploidy_duplicating );
	CPPUNIT_TEST( testPositiveAneploidy_insertingAtTheLastPosition );
	CPPUNIT_TEST( testPositiveAneploidy_failsWhenDuplicatedBeyondLimit );
	CPPUNIT_TEST( testPositiveAneploidy_failsWhenTargetPositionBeyondLimits );
	CPPUNIT_TEST( testNegativeAneploidy_insideRange );
	CPPUNIT_TEST( testNegativeAneploidy_outOfRange );
	CPPUNIT_TEST( testNegativeAneploidy_whenTooLittle );
	CPPUNIT_TEST( testEuploidy );
	CPPUNIT_TEST( testMutationByFusion );
	CPPUNIT_TEST( testMutationByFusion_whenSameChromosome );
	CPPUNIT_TEST( testMutationByFusion_whenSelectedOutOfRange );
	CPPUNIT_TEST( testMutationByFusion_whenFusedOutOfRange );
	CPPUNIT_TEST( testMutationBySplit );
	CPPUNIT_TEST( testMutationBySplit_whenSelectedBeyondRange );
	CPPUNIT_TEST( testMutationBySplit_whenSplitPointBeyondRange );
	CPPUNIT_TEST( testMutationBySplit_withZeroSplitPoint );
	CPPUNIT_TEST( testMutationBySplit_withTargetBeyondRange );
	CPPUNIT_TEST( testMutationByTranslocation );
	CPPUNIT_TEST( testMutationByTranslocation_withTargetBeyondRange );
	CPPUNIT_TEST( testMutationByTranslocation_withSourceBeyondRange );
	CPPUNIT_TEST( testMutationByTranslocation_withStartPointBeyondRange );
	CPPUNIT_TEST( testMutationByTranslocation_withInsertionPointBeyondRange );
	CPPUNIT_TEST( testMutationByTranslocation_withZeroLength );
	CPPUNIT_TEST( testMutationByTranslocation_withLengthThatMakesSourceWithNoCodons );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp()
	{
	}
	void tearDown()
	{
	}
protected:
	void testPositiveAneploidy_duplicating()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,2);

		karyotype.mutationByPositiveAneploidy(/*To be duplicated*/ 0, /* TargetPosition*/ 2);

		std::ostringstream os;
		karyotype.dumpOn(os);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"["
				"[00000000:00000001]:"
				"[00000010:00000011]:"
				"[00000000:00000001]:"
				"[00000020:00000021]:"
				"[00000030:00000031]"
				"]"),
			os.str());
	}
	void testPositiveAneploidy_insertingAtTheLastPosition()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,2);

		karyotype.mutationByPositiveAneploidy(/*To be duplicated*/ 0, /* TargetPosition*/ 4);

		std::ostringstream os;
		karyotype.dumpOn(os);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"["
				"[00000000:00000001]:"
				"[00000010:00000011]:"
				"[00000020:00000021]:"
				"[00000030:00000031]:"
				"[00000000:00000001]"
				"]"),
			os.str());
	}
	void testPositiveAneploidy_failsWhenDuplicatedBeyondLimit()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,2);
		try 
		{
			karyotype.mutationByPositiveAneploidy(/*To be duplicated*/ 4, /* TargetPosition*/ 2);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (Bioscena::ErrAssertionFailed & e)
		{
			const char * expectedMessage = "Positive Anaeploidy: Selecting a chromosome beyond range";
			CPPUNIT_ASSERT_EQUAL(expectedMessage, e.what());
		}
	}
	void testPositiveAneploidy_failsWhenTargetPositionBeyondLimits()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,2);
		try 
		{
			karyotype.mutationByPositiveAneploidy(/*To be duplicated*/ 0, /* TargetPosition*/ 5);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (Bioscena::ErrAssertionFailed & e)
		{
			const char * expectedMessage = "Positive Anaeploidy: Placing duplicated chromosome beyond range";
			CPPUNIT_ASSERT_EQUAL(expectedMessage, e.what());
		}
	}
	void testNegativeAneploidy_insideRange()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,2);

		karyotype.mutationByNegativeAneploidy(2);

		std::ostringstream os;
		karyotype.dumpOn(os);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"["
				"[00000000:00000001]:"
				"[00000010:00000011]:"
				"[00000030:00000031]"
				"]"),
			os.str());
	}
	void testNegativeAneploidy_outOfRange()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,2);
		try 
		{
			karyotype.mutationByNegativeAneploidy(4);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (Bioscena::ErrAssertionFailed & e)
		{
			const char * expectedMessage = "Negative Anaeploidy: Selecting a chromosome beyond range";
			CPPUNIT_ASSERT_EQUAL(expectedMessage, e.what());
		}
	}
	void testNegativeAneploidy_whenTooLittle()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(1,2);

		karyotype.mutationByNegativeAneploidy(0);

		std::ostringstream os;
		karyotype.dumpOn(os);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"["
				"[00000000:00000001]"
				"]"),
			os.str());
	}
	void testEuploidy()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,2);

		karyotype.mutationByEuploidy();

		std::ostringstream os;
		karyotype.dumpOn(os);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"["
				"[00000000:00000001]:"
				"[00000010:00000011]:"
				"[00000020:00000021]:"
				"[00000030:00000031]:"
				"[00000000:00000001]:"
				"[00000010:00000011]:"
				"[00000020:00000021]:"
				"[00000030:00000031]"
				"]"),
			os.str());
	}
	void testMutationByFusion()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,2);

		karyotype.mutationByFusion(0,3);

		std::ostringstream os;
		karyotype.dumpOn(os);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"["
				"[00000000:00000001:00000030:00000031]:"
				"[00000010:00000011]:"
				"[00000020:00000021]"
				"]"),
			os.str());
	}
	void testMutationByFusion_whenSameChromosome()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,2);

		karyotype.mutationByFusion(2,2);

		std::ostringstream os;
		karyotype.dumpOn(os);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"["
				"[00000000:00000001]:"
				"[00000010:00000011]:"
				"[00000020:00000021:00000020:00000021]:"
				"[00000030:00000031]"
				"]"),
			os.str());
	}
	void testMutationByFusion_whenSelectedOutOfRange()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,2);
		try 
		{
			karyotype.mutationByFusion(4,2);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (Bioscena::ErrAssertionFailed & e)
		{
			const char * expectedMessage = "MutationByFusion: Selecting a chromosome beyond range";
			CPPUNIT_ASSERT_EQUAL(expectedMessage, e.what());
		}
	}
	void testMutationByFusion_whenFusedOutOfRange()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,2);
		try 
		{
			karyotype.mutationByFusion(2,4);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (Bioscena::ErrAssertionFailed & e)
		{
			const char * expectedMessage = "MutationByFusion: Fusing a chromosome beyond range";
			CPPUNIT_ASSERT_EQUAL(expectedMessage, e.what());
		}
	}
	void testMutationBySplit()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,3);

		karyotype.mutationBySplit(1,/*split point*/ 1, /*target*/ 4);

		std::ostringstream os;
		karyotype.dumpOn(os);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"["
				"[00000000:00000001:00000002]:"
				"[00000010]:"
				"[00000020:00000021:00000022]:"
				"[00000030:00000031:00000032]:"
				"[00000011:00000012]"
				"]"),
			os.str());
	}
	void testMutationBySplit_whenSelectedBeyondRange()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,3);
		try 
		{
			karyotype.mutationBySplit(4,/*split point*/ 1, /*target*/ 4);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (Bioscena::ErrAssertionFailed & e)
		{
			const char * expectedMessage = "MutationBySplit: Splitting a chromosome beyond range";
			CPPUNIT_ASSERT_EQUAL(expectedMessage, e.what());
		}
	}
	void testMutationBySplit_whenSplitPointBeyondRange()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,3);
		try 
		{
			karyotype.mutationBySplit(2,/*split point*/ 2, /*target*/ 4);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (Bioscena::ErrAssertionFailed & e)
		{
			const char * expectedMessage = "MutationBySplit: Split point beyond chromosome lenght";
			CPPUNIT_ASSERT_EQUAL(expectedMessage, e.what());
		}
	}
	void testMutationBySplit_withZeroSplitPoint()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,3);
		try 
		{
			karyotype.mutationBySplit(2,/*split point*/ 0, /*target*/ 4);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (Bioscena::ErrAssertionFailed & e)
		{
			const char * expectedMessage = "MutationBySplit: Split point can not be 0";
			CPPUNIT_ASSERT_EQUAL(expectedMessage, e.what());
		}
	}
	void testMutationBySplit_withTargetBeyondRange()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,3);
		try 
		{
			karyotype.mutationBySplit(2,/*split point*/ 1, /*target*/ 5);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (Bioscena::ErrAssertionFailed & e)
		{
			const char * expectedMessage = "MutationBySplit: Target chromosome position beyond range";
			CPPUNIT_ASSERT_EQUAL(expectedMessage, e.what());
		}
	}
	void testMutationByTranslocation()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,3);

		karyotype.mutationByTranslocation(/*source*/0,/*start*/2,/*lenght*/2,/*target*/3,/*start*/2);

		std::ostringstream os;
		karyotype.dumpOn(os);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"["
				"[00000001]:"
				"[00000010:00000011:00000012]:"
				"[00000020:00000021:00000022]:"
				"[00000030:00000031:00000002:00000000:00000032]"
				"]"),
			os.str());
	}
	void testMutationByTranslocation_withTargetBeyondRange()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,3);
		try 
		{
			karyotype.mutationByTranslocation(/*source*/0,/*start*/2,/*lenght*/2,/*target*/4,/*start*/2);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (Bioscena::ErrAssertionFailed & e)
		{
			const char * expectedMessage = "MutationByTranslocation: Target chromosome position beyond range";
			CPPUNIT_ASSERT_EQUAL(expectedMessage, e.what());
		}
	}
	void testMutationByTranslocation_withSourceBeyondRange()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,3);
		try 
		{
			karyotype.mutationByTranslocation(/*source*/4,/*start*/2,/*lenght*/2,/*target*/3,/*start*/2);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (Bioscena::ErrAssertionFailed & e)
		{
			const char * expectedMessage = "MutationByTranslocation: Source chromosome position beyond range";
			CPPUNIT_ASSERT_EQUAL(expectedMessage, e.what());
		}
	}
	void testMutationByTranslocation_withStartPointBeyondRange()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,3);
		try 
		{
			karyotype.mutationByTranslocation(/*source*/3,/*start*/3,/*lenght*/2,/*target*/3,/*start*/2);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (Bioscena::ErrAssertionFailed & e)
		{
			const char * expectedMessage = "MutationByTranslocation: Segment start point beyond range";
			CPPUNIT_ASSERT_EQUAL(expectedMessage, e.what());
		}
	}
	void testMutationByTranslocation_withInsertionPointBeyondRange()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,3);
		try 
		{
			karyotype.mutationByTranslocation(/*source*/3,/*start*/2,/*lenght*/2,/*target*/3,/*start*/3);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (Bioscena::ErrAssertionFailed & e)
		{
			const char * expectedMessage = "MutationByTranslocation: Insertion point beyond target chromosome size";
			CPPUNIT_ASSERT_EQUAL(expectedMessage, e.what());
		}
	}
	void testMutationByTranslocation_withZeroLength()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,3);
		try 
		{
			karyotype.mutationByTranslocation(/*source*/3,/*start*/2,/*lenght*/0,/*target*/3,/*start*/2);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (Bioscena::ErrAssertionFailed & e)
		{
			const char * expectedMessage = "MutationByTranslocation: Zero length translocated segment";
			CPPUNIT_ASSERT_EQUAL(expectedMessage, e.what());
		}
	}
	void testMutationByTranslocation_withLengthThatMakesSourceWithNoCodons()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,3);
		try 
		{
			karyotype.mutationByTranslocation(/*source*/3,/*start*/2,/*lenght*/3,/*target*/3,/*start*/2);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (Bioscena::ErrAssertionFailed & e)
		{
			const char * expectedMessage = "MutationByTranslocation: Total migration of codons from source";
			CPPUNIT_ASSERT_EQUAL(expectedMessage, e.what());
		}
	}
};


}


