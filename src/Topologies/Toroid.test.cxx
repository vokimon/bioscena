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
#include "Toroid.hxx"

using Bioscena::Toroid;

namespace BioscenaTest {

class ToroidTest;
CPPUNIT_TEST_SUITE_REGISTRATION(ToroidTest);

class ToroidTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ToroidTest );
	CPPUNIT_TEST( test_isValidPosition_above );
	CPPUNIT_TEST( test_isValidPosition_below );
	CPPUNIT_TEST( test_isValidPosition_first );
	CPPUNIT_TEST( test_isValidPosition_last );
	CPPUNIT_TEST( test_posAt_origin );
	CPPUNIT_TEST( test_posAt_oneRight );
	CPPUNIT_TEST( test_posAt_oneDown );
	CPPUNIT_TEST( test_posAt_rightAndDown );
	CPPUNIT_TEST( test_nilDisplacement );
	CPPUNIT_TEST( test_displaceVector_noDirections );
	CPPUNIT_TEST( test_displaceVector_singleDir );
	CPPUNIT_TEST( test_displaceVector_allElements );
	CPPUNIT_TEST( test_displaceVector_someElements );
	CPPUNIT_TEST( test_displace_null );
	CPPUNIT_TEST( test_displace_forwardAndBackward );
	CPPUNIT_TEST( test_opositeDisplacement );
	CPPUNIT_TEST( test_displace_singleStepN );
	CPPUNIT_TEST( test_displace_singleStepNE );
	CPPUNIT_TEST( test_displace_singleStepE );
	CPPUNIT_TEST( test_displace_singleStepSE );
	CPPUNIT_TEST( test_displace_singleStepS );
	CPPUNIT_TEST( test_displace_singleStepSW );
	CPPUNIT_TEST( test_displace_singleStepW );
	CPPUNIT_TEST( test_displace_singleStepNW );
	CPPUNIT_TEST( test_displace_multipleSteps );
	CPPUNIT_TEST( test_displace_atTheEndOfRowAndE );
	CPPUNIT_TEST( test_displace_atTheEndOfColumnAndS );
	CPPUNIT_TEST( test_displace_atTheEndAndE );
	CPPUNIT_TEST( test_displace_atTheEndAndS );
	CPPUNIT_TEST( test_displace_atTheEndAndSE );
	CPPUNIT_TEST( test_displace_atTheStartAndW );
	CPPUNIT_TEST( test_displace_atTheStartAndN );
	CPPUNIT_TEST( test_wayTo_samePosition );
	CPPUNIT_TEST( test_wayTo_nextE );
	CPPUNIT_TEST( test_wayTo_nextW );
	CPPUNIT_TEST( test_wayTo_nextS );
	CPPUNIT_TEST( test_wayTo_nextN );
	CPPUNIT_TEST( test_wayTo_farE );
	CPPUNIT_TEST( test_wayTo_farW );
	CPPUNIT_TEST( test_wayTo_farS );
	CPPUNIT_TEST( test_wayTo_farN );
	CPPUNIT_TEST( test_wayTo_crossE );
	CPPUNIT_TEST( test_wayTo_crossW );
	CPPUNIT_TEST( test_wayTo_crossS );
	CPPUNIT_TEST( test_wayTo_crossN );
	CPPUNIT_TEST( test_wayTo_SE );
	CPPUNIT_TEST( test_wayTo_SEandE );
	CPPUNIT_TEST( test_distance_same );
	CPPUNIT_TEST( test_distance_nextE );
	CPPUNIT_TEST( test_distance_nextW );
	CPPUNIT_TEST( test_distance_farE );
	CPPUNIT_TEST( test_distance_farW );
	CPPUNIT_TEST( test_distance_combinedEqual );
	CPPUNIT_TEST( test_distance_combinedEFurther );
	CPPUNIT_TEST( test_ClassTest );
	CPPUNIT_TEST_SUITE_END();
private:
	Toroid * t;
public:
	void setUp() {
		t=new Toroid(10, 7);
	}
	void tearDown() {
		delete t;
	}
	bool IsEquivalentDisplacement(Toroid::Displacement one, Toroid::Displacement other) {
		for (uint32 cell=t->size() ; cell--; ) {
			if (t->displace(cell, one) != t->displace(cell, other) )
				return false;
		}
		return true;
	}
	bool IsInvertedDisplacement(Toroid::Displacement one, Toroid::Displacement other) {
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
	void test_posAt_origin()
	{
		Toroid t(10,7);
		Toroid::Position pos = t.posAt(0,0);
		CPPUNIT_ASSERT_EQUAL(uint32(0u),pos);
	}
	void test_posAt_oneRight()
	{
		Toroid t(10,7);
		Toroid::Position pos = t.posAt(0,1);
		CPPUNIT_ASSERT_EQUAL(uint32(1u),pos);
	}
	void test_posAt_oneDown()
	{
		Toroid t(10,7);
		Toroid::Position pos = t.posAt(1,0);
		CPPUNIT_ASSERT_EQUAL(uint32(10u),pos);
	}
	void test_posAt_rightAndDown()
	{
		Toroid t(10,7);
		Toroid::Position pos = t.posAt(2,3);
		CPPUNIT_ASSERT_EQUAL(uint32(23u),pos);
	}
	std::string hex(uint32 value) {
		std::ostringstream os;
		os << std::hex << value;
		return os.str();
	}
	void assertEqualDisplacements(uint32 expected, uint32 result) {
		CPPUNIT_ASSERT_EQUAL(hex(expected), hex(result));
	}
	void test_nilDisplacement() {
		assertEqualDisplacements(0x88888888u,t->nilDisplacement());
	}
	void test_displaceVector_noDirections() {
		Toroid::Displacement result = t->displaceVector();
		assertEqualDisplacements(0x88888888u, result);
	}
	void test_displaceVector_singleDir() {
		Toroid::Displacement result = t->displaceVector(Toroid::NE);
		assertEqualDisplacements(0x88888881u, result);
	}
	void test_displaceVector_allElements()
	{
		Toroid t(10,7);
		Toroid::Displacement displacement = t.displaceVector(
				Toroid::N,Toroid::NE,Toroid::E,Toroid::SE,
				Toroid::S,Toroid::SW,Toroid::W,Toroid::NW);
		assertEqualDisplacements(0x54672310u,displacement);
	}
	void test_displaceVector_someElements()
	{
		Toroid t(10,7);
		Toroid::Displacement displacement = t.displaceVector(
				Toroid::N,Toroid::NE,Toroid::E,Toroid::NoDir,
				Toroid::S);
		assertEqualDisplacements(0x88878310u,displacement);
	}
	void test_displace_null() {
		Toroid::Position original = 10;
		Toroid::Position displaced = t->displace(original, t->nilDisplacement());
		CPPUNIT_ASSERT_EQUAL(original, displaced);
	}
	void test_displace_forwardAndBackward() {
		Toroid::Position original = 1;
		Toroid::Displacement forward = 3;
		Toroid::Displacement backward = t->opositeDisplacement(forward);
		Toroid::Position displaced1 = t->displace(original, forward);
		Toroid::Position displaced2 = t->displace(displaced1, backward);
		CPPUNIT_ASSERT_EQUAL(original, displaced2);
	}
	void test_opositeDisplacement()
	{
		Toroid t(10,7);
		Toroid::Displacement displacement = t.displaceVector(
				Toroid::N,Toroid::NE,Toroid::E,Toroid::SE,
				Toroid::S,Toroid::SW,Toroid::W,Toroid::NW);
		Toroid::Displacement oposite = t.opositeDisplacement(displacement);

		Toroid::Displacement expected = t.displaceVector(
				Toroid::S,Toroid::SW,Toroid::W,Toroid::NW,
				Toroid::N,Toroid::NE,Toroid::E,Toroid::SE);
		CPPUNIT_ASSERT_EQUAL(expected, oposite);
	}

	void assertDisplace(
		uint32 ox, uint32 oy,
		uint32 tx, uint32 ty, 
		Toroid::Direction d0 = Toroid::NoDir,
		Toroid::Direction d1 = Toroid::NoDir,
		Toroid::Direction d2 = Toroid::NoDir,
		Toroid::Direction d3 = Toroid::NoDir,
		Toroid::Direction d4 = Toroid::NoDir,
		Toroid::Direction d5 = Toroid::NoDir,
		Toroid::Direction d6 = Toroid::NoDir,
		Toroid::Direction d7 = Toroid::NoDir
		)
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(ox,oy);
		Toroid::Displacement vector = t.displaceVector(
			d0,d1,d2,d3,d4,d5,d6,d7);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(tx,ty),result);
	}

	void test_displace_singleStepN()
	{
		assertDisplace(5,3, 4,3, Toroid::N);
	}
	void test_displace_singleStepNE()
	{
		assertDisplace(5,3, 4,4, Toroid::NE);
	}
	void test_displace_singleStepE()
	{
		assertDisplace(5,3, 5,4, Toroid::E);
	}
	void test_displace_singleStepSE()
	{
		assertDisplace(5,3, 6,4, Toroid::SE);
	}
	void test_displace_singleStepS()
	{
		assertDisplace(5,3, 6,3, Toroid::S);
	}
	void test_displace_singleStepSW()
	{
		assertDisplace(5,3, 6,2, Toroid::SW);
	}
	void test_displace_singleStepW()
	{
		assertDisplace(5,3, 5,2, Toroid::W);
	}
	void test_displace_singleStepNW()
	{
		assertDisplace(5,3, 4,2, Toroid::NW);
	}
	void test_displace_multipleSteps()
	{
		assertDisplace(5,3, 5,1,
			Toroid::N, Toroid::NW, Toroid::W,
			Toroid::SW, Toroid::NoDir, Toroid::SE);
	}
	void test_displace_atTheEndOfRowAndE()
	{
		assertDisplace(3,9, 4,0, Toroid::E);
	}
	void test_displace_atTheEndOfColumnAndS()
	{
		assertDisplace(6,3, 0,3, Toroid::S);
	}
	void test_displace_atTheEndAndE()
	{
		assertDisplace(6,9, 0,0, Toroid::E);
	}
	void test_displace_atTheEndAndS()
	{
		assertDisplace(6,9, 0,9, Toroid::S);
	}
	void test_displace_atTheEndAndSE()
	{
		assertDisplace(6,9, 1,0, Toroid::SE);
	}
	void test_displace_atTheStartAndW()
	{
		assertDisplace(0,0, 6,9, Toroid::W);
	}
	void test_displace_atTheStartAndN()
	{
		assertDisplace(0,0, 6,0, Toroid::N);
	}
	void assertPathTowards(
		uint32 ox, uint32 oy,
		uint32 tx, uint32 ty, 
		Toroid::Direction d0 = Toroid::NoDir,
		Toroid::Direction d1 = Toroid::NoDir,
		Toroid::Direction d2 = Toroid::NoDir,
		Toroid::Direction d3 = Toroid::NoDir,
		Toroid::Direction d4 = Toroid::NoDir,
		Toroid::Direction d5 = Toroid::NoDir,
		Toroid::Direction d6 = Toroid::NoDir,
		Toroid::Direction d7 = Toroid::NoDir
		) {
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(ox,oy);
		Toroid::Position target = t.posAt(tx,ty);
		Toroid::Displacement result;
		bool reached_result = t.wayTo(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(
			d0,d1,d2,d3,d4,d5,d6,d7);
		assertEqualDisplacements(expected,result);
		CPPUNIT_ASSERT(reached_result);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
	}

	void test_wayTo_samePosition()
	{
		assertPathTowards( 2,1, 2,1, Toroid::NoDir);
	}

	void test_wayTo_nextE()
	{
		assertPathTowards( 0,0, 0,1, Toroid::E);
	}
	void test_wayTo_nextW()
	{
		assertPathTowards( 0,1, 0,0, Toroid::W);
	}
	void test_wayTo_nextS()
	{
		assertPathTowards( 0,0, 1,0, Toroid::S);
	}
	void test_wayTo_nextN()
	{
		assertPathTowards( 1,0, 0,0, Toroid::N);
	}

	void test_wayTo_farE()
	{
		assertPathTowards( 0,0, 0,2, Toroid::E, Toroid::E);
	}
	void test_wayTo_farW()
	{
		assertPathTowards( 0,2, 0,0, Toroid::W, Toroid::W);
	}
	void test_wayTo_farS()
	{
		assertPathTowards( 0,0, 2,0, Toroid::S, Toroid::S);
	}
	void test_wayTo_farN()
	{
		assertPathTowards( 2,0, 0,0, Toroid::N, Toroid::N);
	}

	void test_wayTo_crossE()
	{
		assertPathTowards( 0,9, 1,0, Toroid::E);
	}
	void test_wayTo_crossW()
	{
		assertPathTowards( 1,0, 0,9, Toroid::W);
	}
	void test_wayTo_crossN()
	{
		assertPathTowards( 0,1, 6,1, Toroid::N);
	}
	void test_wayTo_crossS()
	{
		assertPathTowards( 6,1, 0,1, Toroid::S);
	}

	void test_wayTo_SE()
	{
		assertPathTowards( 0,0, 1,1, Toroid::SE);
	}
	void test_wayTo_SEandE()
	{
		assertPathTowards( 0,0, 1,5,
			Toroid::E,
			Toroid::E,
			Toroid::E,
			Toroid::E,
			Toroid::SE
		);
	}

	void test_wayTo_notReached()
	{
		Toroid t(10,31);
		Toroid::Position origin = t.posAt(0,0);
		Toroid::Position target = t.posAt(9,3);
		Toroid::Displacement result;
		bool reached = t.wayTo(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(
				Toroid::S, Toroid::S, Toroid::S, Toroid::S,
				Toroid::S, Toroid::SE, Toroid::SE, Toroid::SE);
		assertEqualDisplacements(expected,result);
		CPPUNIT_ASSERT_EQUAL(t.posAt(8,3), t.displace(origin,result));
		CPPUNIT_ASSERT(!reached);
	}

	void assertDistance(
		uint32 ox, uint32 oy,
		uint32 tx, uint32 ty,
		uint32 expected
		) {
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(ox,oy);
		Toroid::Position target = t.posAt(tx,ty);
		CPPUNIT_ASSERT_EQUAL(
			expected,
			t.distance(origin, target));
	}


	void test_distance_same()
	{
		assertDistance(1,1, 1,1, 0);
	}
	void test_distance_nextE()
	{
		assertDistance(0,0, 0,1, 1);
	}
	void test_distance_nextW()
	{
		assertDistance(0,1, 0,0, 1);
	}
	void test_distance_farE()
	{
		assertDistance(0,0, 0,2, 2);
	}
	void test_distance_farW()
	{
		assertDistance(0,2, 0,0, 2);
	}
	void test_distance_combinedEqual()
	{
		assertDistance(0,0, 1,1, 1);
	}
	void test_distance_combinedEFurther()
	{
		assertDistance(0,0, 1,2, 2);
	}

	void test_ClassTest()
	{
		Toroid::ClassTest();
	}

};


}


