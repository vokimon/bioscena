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
	CPPUNIT_TEST( test_pathTowards_samePosition );
	CPPUNIT_TEST( test_pathTowards_nextE );
	CPPUNIT_TEST( test_pathTowards_nextW );
	CPPUNIT_TEST( test_pathTowards_nextS );
	CPPUNIT_TEST( test_pathTowards_nextN );
	CPPUNIT_TEST( test_pathTowards_farE );
	CPPUNIT_TEST( test_pathTowards_farW );
	CPPUNIT_TEST( test_pathTowards_farS );
	CPPUNIT_TEST( test_pathTowards_farN );
	CPPUNIT_TEST( test_pathTowards_crossE );
	CPPUNIT_TEST( test_pathTowards_crossW );
	CPPUNIT_TEST( test_pathTowards_crossS );
	CPPUNIT_TEST( test_pathTowards_crossN );
//	CPPUNIT_TEST( test_pathTowards_combined );
//	CPPUNIT_TEST( test_pathTowards_YGreaterToSE );
//	CPPUNIT_TEST( test_pathTowards_XGreaterToSE );
//	CPPUNIT_TEST( test_pathTowards_XGreaterToNW );
//	CPPUNIT_TEST( test_pathTowards_XGreaterToSW );
//	CPPUNIT_TEST( test_pathTowards_XGreaterToNE );
//	CPPUNIT_TEST( test_pathTowards_crossingBoundaryAtN );
//	CPPUNIT_TEST( test_pathTowards_crossingBoundaryAtS );
//	CPPUNIT_TEST( test_pathTowards_crossingBoundaryAtW );
//	CPPUNIT_TEST( test_pathTowards_crossingBoundaryAtE );
//	CPPUNIT_TEST( test_pathTowards_crossingBoundaryAtNE );
//	CPPUNIT_TEST( test_pathTowards_crossingBoundaryAtEnd );
//	CPPUNIT_TEST( test_pathTowards_crossingBoundaryAtOrigin );
//	CPPUNIT_TEST( test_pathTowards_notReached );
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
		assertEqualDisplacements(0x45673210u,displacement);
	}
	void test_displaceVector_someElements()
	{
		Toroid t(10,7);
		Toroid::Displacement displacement = t.displaceVector(
				Toroid::N,Toroid::NE,Toroid::E,Toroid::NoDir,
				Toroid::S);
		assertEqualDisplacements(0x88878210u,displacement);
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
	void test_displace_singleStepN()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(5,3);
		Toroid::Displacement vector = t.displaceVector(Toroid::N);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(4,3),result);
	}
	void test_displace_singleStepNE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(5,3);
		Toroid::Displacement vector = t.displaceVector(Toroid::NE);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(4,4),result);
	}
	void test_displace_singleStepE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(5,3);
		Toroid::Displacement vector = t.displaceVector(Toroid::E);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(5,4),result);
	}
	void test_displace_singleStepSE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(5,3);
		Toroid::Displacement vector = t.displaceVector(Toroid::SE);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(6,4),result);
	}
	void test_displace_singleStepS()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(5,3);
		Toroid::Displacement vector = t.displaceVector(Toroid::S);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(6,3),result);
	}
	void test_displace_singleStepSW()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(5,3);
		Toroid::Displacement vector = t.displaceVector(Toroid::SW);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(6,2),result);
	}
	void test_displace_singleStepW()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(5,3);
		Toroid::Displacement vector = t.displaceVector(Toroid::W);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(5,2),result);
	}
	void test_displace_singleStepNW()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(5,3);
		Toroid::Displacement vector = t.displaceVector(Toroid::NW);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(4,2),result);
	}
	void test_displace_multipleSteps()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(5,3);
		Toroid::Displacement vector = t.displaceVector(
				Toroid::N, Toroid::NW, Toroid::W, Toroid::SW, Toroid::NoDir, Toroid::SE);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(5,1),result);
	}
	void test_displace_atTheEndOfRowAndE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(3,9);
		Toroid::Displacement vector = t.displaceVector(Toroid::E);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(4,0),result);
	}
	void test_displace_atTheEndOfColumnAndS()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(6,3);
		Toroid::Displacement vector = t.displaceVector(Toroid::S);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(0,3),result);
	}
	void test_displace_atTheEndAndE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(6,9);
		Toroid::Displacement vector = t.displaceVector(Toroid::E);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(0,0),result);
	}
	void test_displace_atTheEndAndS()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(6,9);
		Toroid::Displacement vector = t.displaceVector(Toroid::S);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(0,9),result);
	}
	void test_displace_atTheEndAndSE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(6,9);
		Toroid::Displacement vector = t.displaceVector(Toroid::SE);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(1,0),result);
	}
	void test_displace_atTheStartAndW()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(0,0);
		Toroid::Displacement vector = t.displaceVector(Toroid::W);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(6,9),result);
	}
	void test_displace_atTheStartAndN()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(0,0);
		Toroid::Displacement vector = t.displaceVector(Toroid::N);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(6,0),result);
	}
	void assertPathTowards(
		uint32 ox, uint32 oy,
		uint32 tx, uint32 ty, 
		bool reach,
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
		bool reached_result = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(
			d0,d1,d2,d3,d4,d5,d6,d7);
		assertEqualDisplacements(expected,result);
		CPPUNIT_ASSERT_EQUAL(reach, reached_result);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
	}

	void test_pathTowards_samePosition()
	{
		assertPathTowards(
			2,1,
			2,1,
			true,
			Toroid::NoDir
		);
	}
	void test_pathTowards_nextE()
	{
		assertPathTowards(
			0,0,
			0,1,
			true,
			Toroid::E
		);
	}

	void test_pathTowards_nextW()
	{
		assertPathTowards(
			0,1,
			0,0,
			true,
			Toroid::W
		);
	}

	void test_pathTowards_nextS()
	{
		assertPathTowards(
			0,0,
			1,0,
			true,
			Toroid::S
		);
	}
	void test_pathTowards_nextN()
	{
		assertPathTowards(
			1,0,
			0,0,
			true,
			Toroid::N
		);
	}

	void test_pathTowards_farE()
	{
		assertPathTowards(
			0,0,
			0,2,
			true,
			Toroid::E,
			Toroid::E
		);
	}

	void test_pathTowards_farW()
	{
		assertPathTowards(
			0,2,
			0,0,
			true,
			Toroid::W,
			Toroid::W
		);
	}

	void test_pathTowards_farS()
	{
		assertPathTowards(
			0,0,
			2,0,
			true,
			Toroid::S,
			Toroid::S
		);
	}
	void test_pathTowards_farN()
	{
		assertPathTowards(
			2,0,
			0,0,
			true,
			Toroid::N,
			Toroid::N
		);
	}

	void test_pathTowards_crossE()
	{
		assertPathTowards(
			0,9,
			1,0,
			true,
			Toroid::E
		);
	}

	void test_pathTowards_crossW()
	{
		assertPathTowards(
			1,0,
			0,9,
			true,
			Toroid::W
		);
	}

	void test_pathTowards_crossN()
	{
		assertPathTowards(
			0,1,
			6,1,
			true,
			Toroid::N
		);
	}

	void test_pathTowards_crossS()
	{
		assertPathTowards(
			6,1,
			0,1,
			true,
			Toroid::S
		);
	}

	void test_pathTowards_YGreaterToSE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(0,0);
		Toroid::Position target = t.posAt(2,1);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::S,Toroid::SE);
		CPPUNIT_ASSERT(reached);
		assertEqualDisplacements(expected,result);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
	}
	void test_pathTowards_XGreaterToSE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(0,0);
		Toroid::Position target = t.posAt(1,2);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::E,Toroid::SE);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		assertEqualDisplacements(expected,result);
	}
	void test_pathTowards_XGreaterToNW()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(1,2);
		Toroid::Position target = t.posAt(0,0);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::W,Toroid::NW);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		assertEqualDisplacements(expected,result);
	}
	void test_pathTowards_XGreaterToSW()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(0,2);
		Toroid::Position target = t.posAt(1,0);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::W,Toroid::SW);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		assertEqualDisplacements(expected,result);
	}
	void test_pathTowards_XGreaterToNE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(1,0);
		Toroid::Position target = t.posAt(0,2);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::E,Toroid::NE);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		assertEqualDisplacements(expected,result);
	}
	void test_pathTowards_crossingBoundaryAtN()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(0,4);
		Toroid::Position target = t.posAt(6,4);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::N);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		assertEqualDisplacements(expected,result);
	}
	void test_pathTowards_crossingBoundaryAtS()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(6,4);
		Toroid::Position target = t.posAt(0,4);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::S);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		assertEqualDisplacements(expected,result);
	}
	void test_pathTowards_crossingBoundaryAtW()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(4,0);
		Toroid::Position target = t.posAt(3,9);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::W);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		assertEqualDisplacements(expected,result);
	}
	void test_pathTowards_crossingBoundaryAtE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(3,9);
		Toroid::Position target = t.posAt(4,0);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::E);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		assertEqualDisplacements(expected,result);
	}
	void test_pathTowards_crossingBoundaryAtNE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(2,9);
		Toroid::Position target = t.posAt(2,0);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::NE);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		assertEqualDisplacements(expected,result);
	}
	void test_pathTowards_crossingBoundaryAtEnd()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(6,9);
		Toroid::Position target = t.posAt(0,0);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::E);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		assertEqualDisplacements(expected,result);
	}
	void test_pathTowards_crossingBoundaryAtOrigin()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(0,0);
		Toroid::Position target = t.posAt(6,9);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::W);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		assertEqualDisplacements(expected,result);
	}
	void test_pathTowards_notReached()
	{
		Toroid t(10,31);
		Toroid::Position origin = t.posAt(0,0);
		Toroid::Position target = t.posAt(8,3);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(
				Toroid::S, Toroid::S, Toroid::S, Toroid::S,
				Toroid::S, Toroid::SE, Toroid::SE, Toroid::SE);
		CPPUNIT_ASSERT(!reached);
//		std::cout << std::endl << std::hex << expected;
//		std::cout << std::endl << std::hex << result;
//		std::cout << std::dec << std::endl;
		assertEqualDisplacements(expected,result);
	}
};


}


