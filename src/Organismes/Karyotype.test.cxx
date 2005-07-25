// Karyotype.test.cxx: regresion test for the Karyotype class.
//
//////////////////////////////////////////////////////////////////////
// TODO:
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "Karyotype.hxx"

namespace BioscenaTest {

class KaryotypeTest;
CPPUNIT_TEST_SUITE_REGISTRATION(KaryotypeTest);

class KaryotypeTest : public CppUnit::TestFixture {
	typedef CppUnit::TestCase super;
	CPPUNIT_TEST_SUITE( KaryotypeTest );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testInitSequence_whenInside );
	CPPUNIT_TEST( testInitSequence_whenOutside );
	CPPUNIT_TEST( testInitSequence_withOffset );
	CPPUNIT_TEST( testCopyConstructor );
	CPPUNIT_TEST( testAssign );
	CPPUNIT_TEST( testAsString );
	CPPUNIT_TEST( testDumpOn );
/*
	CPPUNIT_TEST( testDumpOn_resetFormatFlags );
*/
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp()
	{
	}
	void tearDown()
	{
	}
protected:
	void testConstructor() {
		Bioscena::Karyotype karyotype;
		CPPUNIT_ASSERT_EQUAL(uint32(0),karyotype.size());
	}
	void testInitSequence_whenInside()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(2,3,0);
		CPPUNIT_ASSERT_EQUAL(0x00ul, karyotype[0][0]);
		CPPUNIT_ASSERT_EQUAL(0x01ul, karyotype[0][1]);
		CPPUNIT_ASSERT_EQUAL(0x02ul, karyotype[0][2]);
		CPPUNIT_ASSERT_EQUAL(0x10ul, karyotype[1][0]);
		CPPUNIT_ASSERT_EQUAL(0x11ul, karyotype[1][1]);
		CPPUNIT_ASSERT_EQUAL(0x12ul, karyotype[1][2]);
	}
	void testInitSequence_whenOutside()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(2,3);
		try
		{
			karyotype[2][0];
		}
		catch (Bioscena::ErrAssertionFailed & e)
		{
			std::string expected = 
				"Accessing to a non-existent chromosome.";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(e.what()));
		}
	}
	void testInitSequence_withOffset()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(2,3,6);
		CPPUNIT_ASSERT_EQUAL(0x60ul, karyotype[0][0]);
		CPPUNIT_ASSERT_EQUAL(0x61ul, karyotype[0][1]);
		CPPUNIT_ASSERT_EQUAL(0x62ul, karyotype[0][2]);
		CPPUNIT_ASSERT_EQUAL(0x70ul, karyotype[1][0]);
		CPPUNIT_ASSERT_EQUAL(0x71ul, karyotype[1][1]);
		CPPUNIT_ASSERT_EQUAL(0x72ul, karyotype[1][2]);
	}
	void testCopyConstructor()
	{
		Bioscena::Karyotype original;
		original.initSequence(2,3,6);

		Bioscena::Karyotype karyotype(original);

		CPPUNIT_ASSERT_EQUAL(0x60ul, karyotype[0][0]);
		CPPUNIT_ASSERT_EQUAL(0x61ul, karyotype[0][1]);
		CPPUNIT_ASSERT_EQUAL(0x62ul, karyotype[0][2]);
		CPPUNIT_ASSERT_EQUAL(0x70ul, karyotype[1][0]);
		CPPUNIT_ASSERT_EQUAL(0x71ul, karyotype[1][1]);
		CPPUNIT_ASSERT_EQUAL(0x72ul, karyotype[1][2]);
	}
	void testAssign()
	{
		Bioscena::Karyotype original;
		original.initSequence(2,3,6);

		Bioscena::Karyotype karyotype;
		karyotype.initSequence(4,5,6);
		karyotype = original;

		CPPUNIT_ASSERT_EQUAL(0x60ul, karyotype[0][0]);
		CPPUNIT_ASSERT_EQUAL(0x61ul, karyotype[0][1]);
		CPPUNIT_ASSERT_EQUAL(0x62ul, karyotype[0][2]);
		CPPUNIT_ASSERT_EQUAL(0x70ul, karyotype[1][0]);
		CPPUNIT_ASSERT_EQUAL(0x71ul, karyotype[1][1]);
		CPPUNIT_ASSERT_EQUAL(0x72ul, karyotype[1][2]);
	}
	void testAsString()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(2,3,9);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"["
				"[00000090"
				":00000091"
				":00000092"
				"]:"
				"[000000a0"
				":000000a1"
				":000000a2"
				"]"
				"]"),
			karyotype.asString());
	}
	void testDumpOn()
	{
		Bioscena::Karyotype karyotype;
		karyotype.initSequence(2,3,9);

		std::ostringstream os;
		karyotype.dumpOn(os);

		CPPUNIT_ASSERT_EQUAL(
			std::string(
				"["
				"[00000090"
				":00000091"
				":00000092"
				"]:"
				"[000000a0"
				":000000a1"
				":000000a2"
				"]"
				"]"),
			os.str());
	}
};


}


