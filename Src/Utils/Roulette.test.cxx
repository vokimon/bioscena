// Roulette.test.cxx: regresion test for the Roulette class.
//
//////////////////////////////////////////////////////////////////////
// TODO:
// - Distribution tests
//   - Distribution test assertion
//   - Distribution test construction
// - Performance tests
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "Roulette.hxx"
#include <time.h>

using Bioscena::Roulette;

namespace BioscenaTest {

class RouletteConstructionTest : public CppUnit::TestCase {
	typedef CppUnit::TestCase super;
	CPPUNIT_TEST_SUITE( RouletteConstructionTest );
	CPPUNIT_TEST( testQueryVoidReturnsZero );
	CPPUNIT_TEST( testSetSectorFirstTime );
	CPPUNIT_TEST( testResetSectorValue);
	CPPUNIT_TEST( testDeleteSector);
	CPPUNIT_TEST_SUITE_END();
	Roulette * _roulette0;
public:
	void setUp()
	{
		_roulette0 = new Roulette;
	}
	void tearDown() {
//		_roulette0->dump();
		delete _roulette0;
	}
protected:
	void testQueryVoidReturnsZero() {
		CPPUNIT_ASSERT_EQUAL(uint32(0),_roulette0->sector(3));
		CPPUNIT_ASSERT(_roulette0->checkInvariant());
	}
	void testSetSectorFirstTime() {
		_roulette0->sector(3,5);
		CPPUNIT_ASSERT_EQUAL(uint32(5),_roulette0->sector(3));
		CPPUNIT_ASSERT(_roulette0->checkInvariant());
	}
	void testResetSectorValue() {
		_roulette0->sector(3,5);
		_roulette0->sector(3,4);
		CPPUNIT_ASSERT_EQUAL(uint32(4),_roulette0->sector(3));
		CPPUNIT_ASSERT(_roulette0->checkInvariant());
	}
	void testDeleteSector() {
		_roulette0->sector(3,5);
		_roulette0->sector(3,0);
		CPPUNIT_ASSERT_EQUAL(uint32(0),_roulette0->sector(3));
		CPPUNIT_ASSERT(_roulette0->checkInvariant());
	}
};

class RouletteOperationTest : public CppUnit::TestCase {
	typedef CppUnit::TestCase super;
	CPPUNIT_TEST_SUITE( RouletteOperationTest );
	CPPUNIT_TEST( testSpinUniformWithMinimumForce );
	CPPUNIT_TEST( testSpinUniformWithMaximumForce );
	CPPUNIT_TEST( testSpinUniformWithMediumForce );
	CPPUNIT_TEST( testSpinUniformWithExtraForce );
	CPPUNIT_TEST_SUITE_END();
	Roulette * _uniformRoulette;
public:
	void setUp()
	{
		_uniformRoulette = new Roulette;
		_uniformRoulette->sector(10,1);
		_uniformRoulette->sector(11,1);
		_uniformRoulette->sector(12,1);
		_uniformRoulette->sector(13,1);
		_uniformRoulette->sector(14,1);
		_uniformRoulette->sector(15,1);
		_uniformRoulette->sector(16,1);
		_uniformRoulette->sector(17,1);
		_uniformRoulette->sector(18,1);
		_uniformRoulette->sector(19,1);

	}
	void tearDown() {
		delete _uniformRoulette;
	}
protected:
	void testSpinUniformWithMinimumForce() {
		CPPUNIT_ASSERT_EQUAL(uint32(10),_uniformRoulette->spin(0));
	}
	void testSpinUniformWithMaximumForce() {
		CPPUNIT_ASSERT_EQUAL(uint32(19),_uniformRoulette->spin(9));
	}
	void testSpinUniformWithMediumForce() {
		CPPUNIT_ASSERT_EQUAL(uint32(15),_uniformRoulette->spin(5));
	}
	void testSpinUniformWithExtraForce() {
		try {
			_uniformRoulette->spin(10);
			CPPUNIT_FAIL("Extra spinned roulette, did not throw an exception");
		}
		catch (Bioscena::ErrAssertionFailed) {
			//TODO: Check that the assertion is ok
		}
	}
};

class RouletteDistributionTest : public CppUnit::TestCase {
	typedef CppUnit::TestCase super;
	CPPUNIT_TEST_SUITE( RouletteDistributionTest );
	CPPUNIT_TEST( testUniformSectors );
	CPPUNIT_TEST_SUITE_END();
	Roulette * _uniformRoulette;
public:
	void setUp()
	{
		_uniformRoulette = new Roulette;
		_uniformRoulette->sector(10,1);
		_uniformRoulette->sector(11,1);
		_uniformRoulette->sector(12,1);
		_uniformRoulette->sector(13,1);
		_uniformRoulette->sector(14,1);
		_uniformRoulette->sector(15,1);
		_uniformRoulette->sector(16,1);
		_uniformRoulette->sector(17,1);
		_uniformRoulette->sector(18,1);
		_uniformRoulette->sector(19,1);

	}
	void tearDown() {
		delete _uniformRoulette;
	}
protected:
	bool assertDistributionErrorAcceptable(double * ) {
		return false;
	}
protected:
	void testUniformSectors() {
		const uint32 niterations = 0xfffff;
		uint32 histograma[10] = {};
		time_t t= time(NULL);
		for (int i = niterations; i--;) {
			uint32 value=(*_uniformRoulette);
			CPPUNIT_ASSERT(value>=10 && value <20);
			histograma[value-10]++;
		}
/*
		std::cout << "Elapsed preallocated object: " << (time(NULL)-t) << std::endl;
		for (int i = 10; i--;) {
			std::cout << (double)histograma[i]/niterations << std::endl;;
		}
*/
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(RouletteConstructionTest);
CPPUNIT_TEST_SUITE_REGISTRATION(RouletteOperationTest);
CPPUNIT_TEST_SUITE_REGISTRATION(RouletteDistributionTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( RouletteConstructionTest, "RouletteTests");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( RouletteOperationTest, "RouletteTests");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( RouletteDistributionTest, "RouletteTests");

}
/*
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ExceptionTest,
	CppUnitTest::coreSuiteName() );
*/


