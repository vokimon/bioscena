// EnergyContainer.test.cxx: regresion test for the EnergyContainer class.
//
//////////////////////////////////////////////////////////////////////
// TODO:
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "EnergyContainer.hxx"
#include <sstream>

using Bioscena::EnergyContainer;

namespace BioscenaTest {

class EnergyContainerTest;
CPPUNIT_TEST_SUITE_REGISTRATION(EnergyContainerTest);

class EnergyContainerTest : public CppUnit::TestFixture
{
	typedef CppUnit::TestCase super;
	CPPUNIT_TEST_SUITE( EnergyContainerTest );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testFeed );
	CPPUNIT_TEST( testFeed_twice );
	CPPUNIT_TEST( testFeed_disipating );
	CPPUNIT_TEST( testFeed_disipatingNotAll );
	CPPUNIT_TEST( testConsume_changesTheTotalEnergy );
	CPPUNIT_TEST( testConsume_notEnough );
	CPPUNIT_TEST( testDisipate_afterConsuming );
	CPPUNIT_TEST( testDisipate_afterConsumingSeveralSlots );
	CPPUNIT_TEST( testConsume_notEnoughSeveralSlots );
	CPPUNIT_TEST( testDump );
	CPPUNIT_TEST( testPassivation );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp()
	{
	}
	void tearDown()
	{
	}
protected:
	void testConstructor()
	{
		EnergyContainer energy(1);
		CPPUNIT_ASSERT_EQUAL(uint32(0),energy.total());
	}
	void testFeed()
	{
		EnergyContainer energy(1);
		energy.feed(3);
		CPPUNIT_ASSERT_EQUAL(uint32(3),energy.total());
	}
	void testFeed_twice()
	{
		EnergyContainer energy(1);
		energy.feed(3);
		energy.feed(2);
		CPPUNIT_ASSERT_EQUAL(uint32(5),energy.total());
	}
	void testFeed_disipating()
	{
		EnergyContainer energy(1);
		energy.feed(3);
		energy.disipate();
		CPPUNIT_ASSERT_EQUAL(uint32(0),energy.total());
	}
	void testFeed_disipatingNotAll()
	{
		EnergyContainer energy(2);
		energy.feed(3);
		energy.disipate();
		CPPUNIT_ASSERT_EQUAL(uint32(3),energy.total());
	}
	void testConsume_changesTheTotalEnergy()
	{
		EnergyContainer energy(2);
		energy.feed(3);
		uint32 got = energy.consume(2);
		CPPUNIT_ASSERT_EQUAL(uint32(2),got);
		CPPUNIT_ASSERT_EQUAL(uint32(1),energy.total());
	}
	void testConsume_notEnough()
	{
		EnergyContainer energy(2);
		energy.feed(3);
		uint32 got = energy.consume(4);
		CPPUNIT_ASSERT_EQUAL(uint32(3),got);
		CPPUNIT_ASSERT_EQUAL(uint32(0),energy.total());
	}
	void testDisipate_afterConsuming()
	{
		EnergyContainer energy(1);
		energy.feed(3);
		uint32 got = energy.consume(2);
		energy.disipate();
		CPPUNIT_ASSERT_EQUAL(uint32(2),got);
		CPPUNIT_ASSERT_EQUAL(uint32(0),energy.total());
	}
	void testDisipate_afterConsumingSeveralSlots()
	{
		EnergyContainer energy(2);
		energy.feed(3);
		energy.disipate();
		energy.feed(6);
		uint32 got = energy.consume(5);
		energy.disipate();
		CPPUNIT_ASSERT_EQUAL(uint32(5),got);
		CPPUNIT_ASSERT_EQUAL(uint32(4),energy.total());
	}
	void testConsume_notEnoughSeveralSlots()
	{
		EnergyContainer energy(2);
		energy.feed(3);
		energy.disipate();
		energy.feed(6);
		uint32 got = energy.consume(20);
		energy.disipate();
		CPPUNIT_ASSERT_EQUAL(uint32(9),got);
		CPPUNIT_ASSERT_EQUAL(uint32(0),energy.total());
	}
	std::string dumpString(const EnergyContainer & energy)
	{
		std::ostringstream output;
		energy.dump(output);
		return output.str();
	}
	void testDump()
	{
		EnergyContainer energy(4);
		CPPUNIT_ASSERT_EQUAL(std::string("[0,0,0,0]=0"), dumpString(energy));
		energy.feed(8);
		CPPUNIT_ASSERT_EQUAL(std::string("[0,0,0,8]=8"), dumpString(energy));
		energy.consume(2);
		CPPUNIT_ASSERT_EQUAL(std::string("[0,0,0,6]=6"), dumpString(energy));
		energy.disipate();
		CPPUNIT_ASSERT_EQUAL(std::string("[0,0,6,0]=6"), dumpString(energy));
		energy.consume(2);
		CPPUNIT_ASSERT_EQUAL(std::string("[0,0,4,0]=4"), dumpString(energy));
		energy.feed(7);
		CPPUNIT_ASSERT_EQUAL(std::string("[0,0,4,7]=11"), dumpString(energy));
		energy.disipate();
		CPPUNIT_ASSERT_EQUAL(std::string("[0,4,7,0]=11"), dumpString(energy));
		energy.feed(3);
		CPPUNIT_ASSERT_EQUAL(std::string("[0,4,7,3]=14"), dumpString(energy));
		energy.disipate();
		CPPUNIT_ASSERT_EQUAL(std::string("[4,7,3,0]=14"), dumpString(energy));
		energy.feed(20);
		CPPUNIT_ASSERT_EQUAL(std::string("[4,7,3,20]=34"), dumpString(energy));
		energy.disipate();
		CPPUNIT_ASSERT_EQUAL(std::string("[7,3,20,0]=30"), dumpString(energy));
	}
	void testPassivation()
	{
		EnergyContainer energy(3);
		energy.feed(8);
		energy.disipate();
		energy.feed(7);
		energy.disipate();
		energy.feed(3);
		std::ostringstream os;
		energy.store(os);

		std::istringstream is(os.str());
		EnergyContainer activated(1);
		activated.load(is);
		CPPUNIT_ASSERT_EQUAL(dumpString(energy), dumpString(activated));
		
	}
};


}

