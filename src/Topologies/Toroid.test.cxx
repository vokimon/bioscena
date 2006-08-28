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
	CPPUNIT_TEST( testInvalidPositionAbove );
	CPPUNIT_TEST( testInvalidPositionBelow );
	CPPUNIT_TEST( testValidPositionFirst );
	CPPUNIT_TEST( testValidPositionLast );
	CPPUNIT_TEST( testDisplacingNullDisplacement );
	CPPUNIT_TEST( testDisplacingBack );
	CPPUNIT_TEST( testPosAt );
	CPPUNIT_TEST( testBuildDisplacement_withAllElementsEnabled );
	CPPUNIT_TEST( testBuildDisplacement_withSomeElementsDisabled );
	CPPUNIT_TEST( testOpositeDisplacement );
	CPPUNIT_TEST( testDisplace_singleStepN );
	CPPUNIT_TEST( testDisplace_singleStepNE );
	CPPUNIT_TEST( testDisplace_singleStepE );
	CPPUNIT_TEST( testDisplace_singleStepSE );
	CPPUNIT_TEST( testDisplace_singleStepS );
	CPPUNIT_TEST( testDisplace_singleStepSW );
	CPPUNIT_TEST( testDisplace_singleStepW );
	CPPUNIT_TEST( testDisplace_singleStepNW );
	CPPUNIT_TEST( testDisplace_multipleSteps );
	CPPUNIT_TEST( testDisplace_atTheEndOfRowAndE );
	CPPUNIT_TEST( testDisplace_atTheEndOfColumnAndS );
	CPPUNIT_TEST( testDisplace_atTheEndAndE );
	CPPUNIT_TEST( testDisplace_atTheEndAndS );
	CPPUNIT_TEST( testDisplace_atTheEndAndSE );
	CPPUNIT_TEST( testDisplace_atTheStartAndW );
	CPPUNIT_TEST( testDisplace_atTheStartAndN );
	CPPUNIT_TEST( testPathTowards_XGreaterToSE );
	CPPUNIT_TEST( testPathTowards_YGreaterToSE );
	CPPUNIT_TEST( testPathTowards_XGreaterToNW );
	CPPUNIT_TEST( testPathTowards_XGreaterToSW );
	CPPUNIT_TEST( testPathTowards_XGreaterToNE );
	CPPUNIT_TEST( testPathTowards_crossingBoundaryAtN );
	CPPUNIT_TEST( testPathTowards_crossingBoundaryAtS );
	CPPUNIT_TEST( testPathTowards_crossingBoundaryAtW );
	CPPUNIT_TEST( testPathTowards_crossingBoundaryAtE );
	CPPUNIT_TEST( testPathTowards_crossingBoundaryAtOrigin );
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
		Toroid::Position original = 10;
		Toroid::Position displaced = t->displace(original, t->nilDisplacement());
		CPPUNIT_ASSERT_EQUAL(original, displaced);
	}
	void testDisplacingBack() {
		Toroid::Position original = 1;
		Toroid::Displacement forward = 3;
		Toroid::Displacement backward = t->opositeDisplacement(forward);
		Toroid::Position displaced1 = t->displace(original, forward);
		Toroid::Position displaced2 = t->displace(displaced1, backward);
		CPPUNIT_ASSERT_EQUAL(original, displaced2);
	}
	void testPosAt()
	{
		Toroid t(10,7);
		Toroid::Position pos = t.posAt(2,3);
		CPPUNIT_ASSERT_EQUAL(uint32(23u),pos);
	}
	void testBuildDisplacement_withAllElementsEnabled()
	{
		Toroid t(10,7);
		Toroid::Displacement displacement = t.displaceVector(
				Toroid::N,Toroid::NE,Toroid::E,Toroid::SE,
				Toroid::S,Toroid::SW,Toroid::W,Toroid::NW);
		CPPUNIT_ASSERT_EQUAL(Toroid::Displacement(0x45673210u),displacement);
	}
	void testBuildDisplacement_withSomeElementsDisabled()
	{
		Toroid t(10,7);
		Toroid::Displacement displacement = t.displaceVector(
				Toroid::N,Toroid::NE,Toroid::E,Toroid::NoDir,
				Toroid::S);
		CPPUNIT_ASSERT_EQUAL(Toroid::Displacement(0x88878210u),displacement);
	}
	void testOpositeDisplacement()
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
	void testDisplace_singleStepN()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(5,3);
		Toroid::Displacement vector = t.displaceVector(Toroid::N);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(4,3),result);
	}
	void testDisplace_singleStepNE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(5,3);
		Toroid::Displacement vector = t.displaceVector(Toroid::NE);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(4,4),result);
	}
	void testDisplace_singleStepE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(5,3);
		Toroid::Displacement vector = t.displaceVector(Toroid::E);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(5,4),result);
	}
	void testDisplace_singleStepSE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(5,3);
		Toroid::Displacement vector = t.displaceVector(Toroid::SE);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(6,4),result);
	}
	void testDisplace_singleStepS()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(5,3);
		Toroid::Displacement vector = t.displaceVector(Toroid::S);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(6,3),result);
	}
	void testDisplace_singleStepSW()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(5,3);
		Toroid::Displacement vector = t.displaceVector(Toroid::SW);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(6,2),result);
	}
	void testDisplace_singleStepW()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(5,3);
		Toroid::Displacement vector = t.displaceVector(Toroid::W);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(5,2),result);
	}
	void testDisplace_singleStepNW()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(5,3);
		Toroid::Displacement vector = t.displaceVector(Toroid::NW);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(4,2),result);
	}
	void testDisplace_multipleSteps()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(5,3);
		Toroid::Displacement vector = t.displaceVector(
				Toroid::N, Toroid::NW, Toroid::W, Toroid::SW, Toroid::NoDir, Toroid::SE);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(5,1),result);
	}
	void testDisplace_atTheEndOfRowAndE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(3,9);
		Toroid::Displacement vector = t.displaceVector(Toroid::E);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(4,0),result);
	}
	void testDisplace_atTheEndOfColumnAndS()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(6,3);
		Toroid::Displacement vector = t.displaceVector(Toroid::S);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(0,3),result);
	}
	void testDisplace_atTheEndAndE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(6,9);
		Toroid::Displacement vector = t.displaceVector(Toroid::E);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(0,0),result);
	}
	void testDisplace_atTheEndAndS()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(6,9);
		Toroid::Displacement vector = t.displaceVector(Toroid::S);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(0,9),result);
	}
	void testDisplace_atTheEndAndSE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(6,9);
		Toroid::Displacement vector = t.displaceVector(Toroid::SE);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(1,0),result);
	}
	void testDisplace_atTheStartAndW()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(0,0);
		Toroid::Displacement vector = t.displaceVector(Toroid::W);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(6,9),result);
	}
	void testDisplace_atTheStartAndN()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(0,0);
		Toroid::Displacement vector = t.displaceVector(Toroid::N);
		Toroid::Position result = t.displace(origin, vector);
		CPPUNIT_ASSERT_EQUAL(t.posAt(6,0),result);
	}
	void testPathTowards_YGreaterToSE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(0,0);
		Toroid::Position target = t.posAt(2,1);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::S,Toroid::SE);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		CPPUNIT_ASSERT_EQUAL(expected,result);
	}
	void testPathTowards_XGreaterToSE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(0,0);
		Toroid::Position target = t.posAt(1,2);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::E,Toroid::SE);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		CPPUNIT_ASSERT_EQUAL(expected,result);
	}
	void testPathTowards_XGreaterToNW()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(1,2);
		Toroid::Position target = t.posAt(0,0);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::W,Toroid::NW);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		CPPUNIT_ASSERT_EQUAL(expected,result);
	}
	void testPathTowards_XGreaterToSW()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(0,2);
		Toroid::Position target = t.posAt(1,0);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::W,Toroid::SW);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		CPPUNIT_ASSERT_EQUAL(expected,result);
	}
	void testPathTowards_XGreaterToNE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(1,0);
		Toroid::Position target = t.posAt(0,2);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::E,Toroid::NE);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		CPPUNIT_ASSERT_EQUAL(expected,result);
	}
	void testPathTowards_crossingBoundaryAtN()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(0,4);
		Toroid::Position target = t.posAt(6,4);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::N);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		CPPUNIT_ASSERT_EQUAL(expected,result);
	}
	void testPathTowards_crossingBoundaryAtS()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(6,4);
		Toroid::Position target = t.posAt(0,4);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::S);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		CPPUNIT_ASSERT_EQUAL(expected,result);
	}
	void testPathTowards_crossingBoundaryAtW()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(4,0);
		Toroid::Position target = t.posAt(3,9);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::W);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		CPPUNIT_ASSERT_EQUAL(expected,result);
	}
	void testPathTowards_crossingBoundaryAtE()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(3,9);
		Toroid::Position target = t.posAt(4,0);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::E);
		CPPUNIT_ASSERT(reached);
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		CPPUNIT_ASSERT_EQUAL(expected,result);
	}
	void testPathTowards_crossingBoundaryAtOrigin()
	{
		Toroid t(10,7);
		Toroid::Position origin = t.posAt(0,0);
		Toroid::Position target = t.posAt(9,6);
		Toroid::Displacement result;
		bool reached = t.pathTowards(origin, target, result);
		Toroid::Displacement expected = t.displaceVector(Toroid::E);
		CPPUNIT_ASSERT(reached);
		std::cout << std::endl << std::hex << expected;
		std::cout << std::endl << std::hex << result;
		std::cout << std::dec << std::endl;
		CPPUNIT_ASSERT_EQUAL(target, t.displace(origin,result));
		CPPUNIT_ASSERT_EQUAL(expected,result);
	}
};


}


