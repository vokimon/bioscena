#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/CompilerOutputter.h>

//#define GUI_ENABLED

#ifdef GUI_ENABLED
#	include <cppunit/ui/qt/TestRunner.h>
#	include <qapplication.h>
#else
#	include <cppunit/ui/text/TestRunner.h>
#endif

#include "Assert.hxx"

void SilentAssertHandler(const char* message, const char* filename, int lineNumber )
{
}

int main( int argc, char* argv[] )
{

	Bioscena::SetAssertFailedHandler(SilentAssertHandler);

#ifdef GUI_ENABLED
	QApplication app(argc,argv); // Needed to run qt app
	CppUnit::QtUi::TestRunner runner;
#else
	CppUnit::TextTestRunner runner;
	
#endif

	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
	runner.addTest( registry.makeTest() );
	// Run the test.
	return !runner.run("");

}



