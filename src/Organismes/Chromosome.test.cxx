// Chromosome.test.cxx: regresion test for the Chromosome class.
//
//////////////////////////////////////////////////////////////////////
// TODO:
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "Chromosome.hxx"

using Bioscena::Chromosome;

namespace BioscenaTest {

class ChromosomeTest;
CPPUNIT_TEST_SUITE_REGISTRATION(ChromosomeTest);

class ChromosomeTest : public CppUnit::TestFixture {
	typedef CppUnit::TestCase super;
	CPPUNIT_TEST_SUITE( ChromosomeTest );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testInitSequence_whenInside );
	CPPUNIT_TEST( testInitSequence_whenOutside );
	CPPUNIT_TEST( testInitSequence_withOffset );
	CPPUNIT_TEST( testCopyConstructor );
	CPPUNIT_TEST( testAsString );
	CPPUNIT_TEST( testDumpOn );
	CPPUNIT_TEST( testDumpOn_resetFormatFlags );
	CPPUNIT_TEST( testFuse );
	CPPUNIT_TEST( testSplit_witOutScopeCentromer );
	CPPUNIT_TEST( testSplit_withZeroCentromer );
	CPPUNIT_TEST( testSplit_withMidCentromer );
	CPPUNIT_TEST( testInsertSegment_whenNoCicle );
	CPPUNIT_TEST( testInsertSegment_whenCicle );
	CPPUNIT_TEST( testInsertSegment_whenInvalidLength );
	CPPUNIT_TEST( testRemoveSegment_whenNoCicle );
	CPPUNIT_TEST( testRemoveSegment_whenCicle );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp()
	{
	}
	void tearDown()
	{
	}
protected:
	void test() {
	}
	void testConstructor() {
		Chromosome chromosome;
		CPPUNIT_ASSERT_EQUAL(uint32(0),chromosome.size());
	}
	void testInitSequence_whenInside()
	{
		Chromosome chromosome;
		chromosome.initSequence(3);
		CPPUNIT_ASSERT_EQUAL(0ul, chromosome[0]);
		CPPUNIT_ASSERT_EQUAL(1ul, chromosome[1]);
		CPPUNIT_ASSERT_EQUAL(2ul, chromosome[2]);
	}
	void testInitSequence_whenOutside()
	{
		Chromosome chromosome;
		chromosome.initSequence(3);
		try
		{
			chromosome[3];
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (Bioscena::ErrAssertionFailed & e)
		{
			std::string expected = 
				"Accessing to a non-existent codon.";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(e.what()));
		}
	}
	void testInitSequence_withOffset()
	{
		Chromosome chromosome;
		chromosome.initSequence(3,6);
		CPPUNIT_ASSERT_EQUAL(6ul, chromosome[0]);
		CPPUNIT_ASSERT_EQUAL(7ul, chromosome[1]);
		CPPUNIT_ASSERT_EQUAL(8ul, chromosome[2]);
	}

	void testCopyConstructor()
	{
		Chromosome original;
		original.initSequence(3,6);

		Chromosome chromosome(original);
		CPPUNIT_ASSERT_EQUAL(6ul, chromosome[0]);
		CPPUNIT_ASSERT_EQUAL(7ul, chromosome[1]);
		CPPUNIT_ASSERT_EQUAL(8ul, chromosome[2]);
	}

	void testAsString()
	{
		Chromosome chromosome;
		chromosome.initSequence(3,9);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"[00000009"
				":0000000a"
				":0000000b"
				"]"),
			chromosome.asString());
	}
	void testDumpOn()
	{
		Chromosome chromosome;
		chromosome.initSequence(3,9);

		std::ostringstream os;
		chromosome.dumpOn(os);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"[00000009"
				":0000000a"
				":0000000b"
				"]"),
			os.str());
	}
	void testDumpOn_resetFormatFlags()
	{
		Chromosome chromosome;
		chromosome.initSequence(3,9);

		std::ostringstream os;
		chromosome.dumpOn(os);
		os << 256;

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"[00000009"
				":0000000a"
				":0000000b"
				"]256"),
			os.str());

	}
	void testFuse()
	{
		Chromosome receptor;
		receptor.initSequence(2,0);
		Chromosome fused;
		fused.initSequence(3,6);
		const Chromosome & constFused=fused;

		receptor.fuse(constFused);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"[00000000"
				":00000001"
				":00000006"
				":00000007"
				":00000008"
				"]"),
			receptor.asString());
		
	}
	void testSplit_witOutScopeCentromer()
	{
		Chromosome original;
		original.initSequence(4,0);
		Chromosome splitted;

		bool changed = original.split(4,splitted);

		CPPUNIT_ASSERT_EQUAL(false, changed);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"[00000000"
				":00000001"
				":00000002"
				":00000003"
				"]"),
			original.asString());
		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"["
				"]"),
			splitted.asString());
		
	}
	void testSplit_withZeroCentromer()
	{
		Chromosome original;
		original.initSequence(4,0);
		Chromosome splitted;

		bool changed = original.split(0,splitted);

		CPPUNIT_ASSERT_EQUAL(false, changed);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"[00000000"
				":00000001"
				":00000002"
				":00000003"
				"]"),
			original.asString());
		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"["
				"]"),
			splitted.asString());
		
	}
	void testSplit_withMidCentromer()
	{
		Chromosome original;
		original.initSequence(4,0);
		Chromosome splitted;

		bool changed = original.split(2,splitted);

		CPPUNIT_ASSERT_EQUAL(true, changed);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"[00000000"
				":00000001"
				"]"),
			original.asString());
		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"[00000002"
				":00000003"
				"]"),
			splitted.asString());
	}
	void testInsertSegment_whenNoCicle()
	{
		Chromosome origin_nonConst;
		origin_nonConst.initSequence(4,0);
		Chromosome target;
		target.initSequence(4,0x10);
		const Chromosome & origin = origin_nonConst;

		target.insertSegment(origin,1,2,3);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"[00000010"
				":00000011"
				":00000012"
				":00000001"
				":00000002"
				":00000013"
				"]"),
			target.asString());
	}
	void testInsertSegment_whenCicle()
	{
		Chromosome origin_nonConst;
		origin_nonConst.initSequence(4,0);
		Chromosome target;
		target.initSequence(4,0x10);
		const Chromosome & origin = origin_nonConst;

		target.insertSegment(origin,3,2,4);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"[00000010"
				":00000011"
				":00000012"
				":00000013"
				":00000003"
				":00000000"
				"]"),
			target.asString());
	}
	void testInsertSegment_whenInvalidLength()
	{
		Chromosome origin;
		origin.initSequence(6,0);
		Chromosome target;
		target.initSequence(4,0x10);

		try
		{
			target.insertSegment(origin,3,7,4);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (Bioscena::ErrAssertionFailed & e)
		{
			std::string expected = 
				"The source chromosome is shorter than the length of the segment";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(e.what()));
		}
	}
	void testRemoveSegment_whenNoCicle()
	{
		Chromosome chromosome;
		chromosome.initSequence(4,0);

		chromosome.removeSegment(1,2);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"[00000000"
				":00000003"
				"]"),
			chromosome.asString());
	}
	void testRemoveSegment_whenCicle()
	{
		Chromosome chromosome;
		chromosome.initSequence(4,0);

		chromosome.removeSegment(3,2);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"[00000001"
				":00000002"
				"]"),
			chromosome.asString());
	}






};


}


