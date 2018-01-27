// An out of limits position is invalid
// A negative position is invalid
// Any resulting position is a valid one
// Apply a null displacement to a position results in the same position
// Apply a displacement and its inverse results in the same position
// Distance from a point to itself is 0
// Distance from a point to itself displaced once is 1
// Distance from a point to itself displaced n times is n
// Applying a distance 
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "Topology.hxx"

using Bioscena::Topology;

namespace BioscenaTest {

class TopologyTest;
CPPUNIT_TEST_SUITE_REGISTRATION(TopologyTest);

class TopologyTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TopologyTest );
	CPPUNIT_TEST( test_isValidPosition_above );
	CPPUNIT_TEST( test_isValidPosition_below );
	CPPUNIT_TEST( test_isValidPosition_first );
	CPPUNIT_TEST( test_isValidPosition_last );
	CPPUNIT_TEST( test_displace );
	CPPUNIT_TEST( test_displace_nullDisplacement );
	CPPUNIT_TEST( test_distance_different );
	CPPUNIT_TEST( test_distance_same );
//	CPPUNIT_TEST( test_ClassTest );
	CPPUNIT_TEST_SUITE_END();
private:
	Topology * t;
public:
	void setUp() {
		t=new Topology(7);
	}
	void tearDown() {
		delete t;
	}
	bool IsEquivalentDisplacement(Topology::Displacement one, Topology::Displacement other) {
		for (uint32 cell=t->size() ; cell--; ) {
			if (t->displace(cell, one) != t->displace(cell, other) )
				return false;
		}
		return true;
	}
	bool IsInvertedDisplacement(Topology::Displacement one, Topology::Displacement other) {
		for (uint32 cell=t->size() ; cell--; ) {
			if (t->displace(t->displace(cell, one), other)==cell)
				return false;
			if (t->displace(t->displace(cell, other), one)==cell)
				return false;
		}
		return true;
	}
	void test_isValidPosition_above() {
		CPPUNIT_ASSERT(!t->isValidPosition(t->size()));
	}
	void test_isValidPosition_below() {
		CPPUNIT_ASSERT(!t->isValidPosition(-1));
	}
	void test_isValidPosition_first() {
		CPPUNIT_ASSERT(t->isValidPosition(0));
	}
	void test_isValidPosition_last() {
		CPPUNIT_ASSERT(t->isValidPosition(t->size()-1));
	}
	void test_displace() {
		Topology::Position original = 1;
		Topology::Position displaced = t->displace(original, 3);
		CPPUNIT_ASSERT(t->isValidPosition(displaced));
	}
	void test_displace_nullDisplacement() {
		Topology::Position original = 1;
		Topology::Position displaced = t->displace(original, t->nilDisplacement());
		CPPUNIT_ASSERT_EQUAL(original, displaced);
	}
	void test_distance_different() {
		Topology::Position one = 1;
		Topology::Position other = 10;
		CPPUNIT_ASSERT_GREATER(uint32(0), t->distance(one,other));
	}
	void test_distance_same() {
		Topology::Position one = 1;
		CPPUNIT_ASSERT_EQUAL(uint32(0), t->distance(one,one));
	}
	void test_ClassTest() {
		Topology::ClassTest();
	}
};


}


