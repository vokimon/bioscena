// Distribution.test.cxx: regresion test for the Distribution class.
//
//////////////////////////////////////////////////////////////////////
// TODO:
// - Any test
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "Distribution.hxx"
#include <time.h>

using Bioscena::Distribution;

namespace CppUnit {
	template <>
	struct assertion_traits<Bioscena::Distribution> {
		static bool equal( const Bioscena::Distribution& x, const Bioscena::Distribution& y )
		{
			return 
				x.dices() == y.dices() &&
				x.minimum() == y.minimum() &&
				x.dicesMagnitude() == y.dicesMagnitude();
		}

		static std::string toString( const Bioscena::Distribution& x )
		{
			OStringStream ost;
			ost << "(" << x.dices() << "D" << x.dicesMagnitude() << " + " << x.minimum() << ")";
			return ost.str();
		}
	};
}

namespace BioscenaTest {

class DistributionTest;
CPPUNIT_TEST_SUITE_REGISTRATION(DistributionTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( DistributionTest, "ProbabilityTests");

class DistributionTest : public CppUnit::TestCase {
	typedef CppUnit::TestCase super;
	CPPUNIT_TEST_SUITE( DistributionTest );
//	CPPUNIT_TEST(  );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
	}
	void tearDown() 
	{
	}
protected:
	void testSomething_BehavesThisWayInThisCase()
	{
		uint32 histograma[50] = {};
		time_t t= time(NULL);
		for (int i = 0xffffff; i--;) {
			histograma[Distribution(0,10,5)()]++;
		}
		std::cout << "Elapsed: " << (time(NULL)-t) << std::endl;
		for (int i = 50; i--;) {
			std::cout << (double)histograma[i]/0xffffff << std::endl;;
		}
	}
	void testSomething_BehavesThisOtherWayInThisCase()
	{
		uint32 histograma[50] = {};
		time_t t= time(NULL);
		Distribution d(0,10,5);
		for (int i = 0xffffff; i--;) {
			histograma[d()]++;
		}
		std::cout << "Elapsed: " << (time(NULL)-t) << std::endl;
		for (int i = 50; i--;) {
			std::cout << (double)histograma[i]/0xffffff << std::endl;;
		}
	}
};

}
