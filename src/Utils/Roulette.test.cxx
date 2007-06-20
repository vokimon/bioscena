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

class RouletteConstructionTest;
CPPUNIT_TEST_SUITE_REGISTRATION(RouletteConstructionTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( RouletteConstructionTest, "RouletteTests");

class RouletteOperationTest;
CPPUNIT_TEST_SUITE_REGISTRATION(RouletteOperationTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( RouletteOperationTest, "RouletteTests");

class RouletteDistributionTest;
//CPPUNIT_TEST_SUITE_REGISTRATION(RouletteDistributionTest);
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( RouletteDistributionTest, "RouletteTests");


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
	Roulette _uniformRoulette;
public:
	void setUp()
	{
		_uniformRoulette.sector(10,1);
		_uniformRoulette.sector(11,1);
		_uniformRoulette.sector(12,1);
		_uniformRoulette.sector(13,1);
		_uniformRoulette.sector(14,1);
		_uniformRoulette.sector(15,1);
		_uniformRoulette.sector(16,1);
		_uniformRoulette.sector(17,1);
		_uniformRoulette.sector(18,1);
		_uniformRoulette.sector(19,1);

	}
	void tearDown() {
	}
protected:
	void testSpinUniformWithMinimumForce() {
		CPPUNIT_ASSERT_EQUAL(uint32(10),_uniformRoulette.spin(0));
	}
	void testSpinUniformWithMaximumForce() {
		CPPUNIT_ASSERT_EQUAL(uint32(19),_uniformRoulette.spin(9));
	}
	void testSpinUniformWithMediumForce() {
		CPPUNIT_ASSERT_EQUAL(uint32(15),_uniformRoulette.spin(5));
	}
	void testSpinUniformWithExtraForce() {
		try {
			_uniformRoulette.spin(10);
			CPPUNIT_FAIL("Extra spinned roulette, did not throw an exception");
		}
		catch (Bioscena::ErrAssertionFailed & e) {
			CPPUNIT_ASSERT_EQUAL(
				std::string("Out of bounds of the roulette"),
				std::string(e.what()));
		}
	}
};

class RouletteDistributionTest : public CppUnit::TestCase {
	typedef CppUnit::TestCase super;
	CPPUNIT_TEST_SUITE( RouletteDistributionTest );
	CPPUNIT_TEST( testUniformDistribution );
	CPPUNIT_TEST( testDescompensatedDistribution );
	CPPUNIT_TEST_SUITE_END();
	Roulette _uniformRoulette;
public:
	void setUp()
	{
		_uniformRoulette.sector(10,1);
		_uniformRoulette.sector(11,1);
		_uniformRoulette.sector(12,1);
		_uniformRoulette.sector(13,1);
		_uniformRoulette.sector(14,1);
		_uniformRoulette.sector(15,1);
		_uniformRoulette.sector(16,1);
		_uniformRoulette.sector(17,1);
		_uniformRoulette.sector(18,1);
		_uniformRoulette.sector(19,1);

	}
	void tearDown() {
	}
protected:
	void assertDistributionErrorAcceptable(
			uint32 *histogram, 
			uint32 niterations,
			uint32 nbins) {
		std::stringstream log("Histogram mismatch:\n");
		uint32 tolerance=niterations/10; // A magnitude order
		bool allMatch = true;
		for (uint32 i = 10; i--;) {
			uint32 difference = 
				std::abs(int32(histogram[i]*10-niterations));
			if (difference < tolerance) continue;
			log << "Bin" << 10+i << ": " << histogram[i] 
				<< " Expected: " << niterations/nbins
				<< std::endl;
			allMatch = false;
		}
		CPPUNIT_ASSERT_MESSAGE(log.str().c_str(),allMatch);
	}
protected:
	void testUniformDistribution() {
		const uint32 niterations = 0xfff;
		const uint32 nbins = 10;
		const uint32 offset = 10;
		uint32 histograma[nbins] = {};
//		time_t t= time(NULL);
		for (int i = niterations; i--;) {
			uint32 value=_uniformRoulette;
			CPPUNIT_ASSERT(value>=offset && value <offset+nbins);
			histograma[value-offset]++;
		}

//		std::cout << "Elapsed preallocated object: " << (time(NULL)-t) << std::endl;
		assertDistributionErrorAcceptable(histograma,niterations,nbins);

	}
	void testDescompensatedDistribution() {
		Roulette descompensatedRoulette;
		descompensatedRoulette.sector(1,1);
		// This one is 3 times bigger
		descompensatedRoulette.sector(2,3);

		const uint32 niterations = 0xffff;
		uint32 matchesOne = 0;
		uint32 matchesTwo = 0;
		for (int i = niterations; i--;) {
			uint32 value=descompensatedRoulette;
			if (value==1) matchesOne++;
			else if (value==2) matchesTwo++;
			else CPPUNIT_FAIL("Unexpected value");
		}
		CPPUNIT_ASSERT_DOUBLES_EQUAL(matchesOne*3,matchesTwo,niterations/100.0);

	}
};

}


