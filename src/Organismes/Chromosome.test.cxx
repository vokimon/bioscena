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
	CPPUNIT_TEST( test );
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
		CPPUNIT_ASSERT_EQUAL(1,1);
	}
	void testConstructor() {
		Cromosome cromosome;
		CPPUNIT_ASSERT_EQUAL(uint32(0),cromosome.size());
	}
};


}
/*
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ExceptionTest,
	CppUnitTest::coreSuiteName() );
*/


