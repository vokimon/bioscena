// Cromosome.test.cxx: regresion test for the Cromosome class.
//
//////////////////////////////////////////////////////////////////////
// TODO:
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "Cromosome.hxx"

using Bioscena::Cromosome;

namespace BioscenaTest {

class CromosomeTest;
CPPUNIT_TEST_SUITE_REGISTRATION(CromosomeTest);

class CromosomeTest : public CppUnit::TestFixture {
	typedef CppUnit::TestCase super;
	CPPUNIT_TEST_SUITE( CromosomeTest );
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
		Cromosome cromosome;
		CPPUNIT_ASSERT_EQUAL(uint32(0),cromosome.size());
	}
	void testInitSequence_whenInside()
	{
		Cromosome cromosome;
		cromosome.initSequence(3);
		CPPUNIT_ASSERT_EQUAL(0ul, cromosome[0]);
		CPPUNIT_ASSERT_EQUAL(1ul, cromosome[1]);
		CPPUNIT_ASSERT_EQUAL(2ul, cromosome[2]);
	}
	void testInitSequence_whenOutside()
	{
		Cromosome cromosome;
		cromosome.initSequence(3);
		try
		{
			cromosome[3];
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
		Cromosome cromosome;
		cromosome.initSequence(3,6);
		CPPUNIT_ASSERT_EQUAL(6ul, cromosome[0]);
		CPPUNIT_ASSERT_EQUAL(7ul, cromosome[1]);
		CPPUNIT_ASSERT_EQUAL(8ul, cromosome[2]);
	}

	void testCopyConstructor()
	{
		Cromosome original;
		original.initSequence(3,6);

		Cromosome cromosome(original);
		CPPUNIT_ASSERT_EQUAL(6ul, cromosome[0]);
		CPPUNIT_ASSERT_EQUAL(7ul, cromosome[1]);
		CPPUNIT_ASSERT_EQUAL(8ul, cromosome[2]);
	}

	void testAsString()
	{
		Cromosome cromosome;
		cromosome.initSequence(3,9);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"[00000009"
				":0000000a"
				":0000000b"
				"]"),
			cromosome.asString());
	}
	void testDumpOn()
	{
		Cromosome cromosome;
		cromosome.initSequence(3,9);

		std::ostringstream os;
		cromosome.dumpOn(os);

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
		Cromosome cromosome;
		cromosome.initSequence(3,9);

		std::ostringstream os;
		cromosome.dumpOn(os);
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
		Cromosome receptor;
		receptor.initSequence(2,0);
		Cromosome fused;
		fused.initSequence(3,6);
		const Cromosome & constFused=fused;

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
		Cromosome original;
		original.initSequence(4,0);
		Cromosome splitted;

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
		Cromosome original;
		original.initSequence(4,0);
		Cromosome splitted;

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
		Cromosome original;
		original.initSequence(4,0);
		Cromosome splitted;

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






};


}


