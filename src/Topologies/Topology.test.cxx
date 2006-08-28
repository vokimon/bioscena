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
	CPPUNIT_TEST( testInvalidPositionAbove );
	CPPUNIT_TEST( testInvalidPositionBelow );
	CPPUNIT_TEST( testValidPositionFirst );
	CPPUNIT_TEST( testValidPositionLast );
	CPPUNIT_TEST( testDisplacingNullDisplacement );
	CPPUNIT_TEST( testDisplacingBack );
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
	void testInvalidPositionAbove() {
		CPPUNIT_ASSERT(!t->isValidPosition(t->size()));
	}
	void testInvalidPositionBelow() {
		CPPUNIT_ASSERT(!t->isValidPosition(-1));
	}
	void testValidPositionFirst() {
		CPPUNIT_ASSERT(t->isValidPosition(0));
	}
	void testValidPositionLast() {
		CPPUNIT_ASSERT(t->isValidPosition(t->size()-1));
	}
	void testDisplacingNullDisplacement() {
		Topology::Position original = 1;
		Topology::Position displaced = t->displace(original, t->nilDisplacement());
		CPPUNIT_ASSERT_EQUAL(original, displaced);
	}
	void testDisplacingBack() {
		Topology::Position original = 1;
		Topology::Displacement forward = 3;
		Topology::Displacement backward = t->opositeDisplacement(forward);
		Topology::Position displaced1 = t->displace(original, forward);
		Topology::Position displaced2 = t->displace(displaced1, backward);
		CPPUNIT_ASSERT_EQUAL(original, displaced2);
	}
};


}


