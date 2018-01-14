#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/CompilerOutputter.h>

//#define GUI_ENABLED

#ifdef GUI_ENABLED
#	include <cppunit/ui/qt/TestRunner.h>
#	include <qapplication.h>
#else
#	include <cppunit/ui/text/TestRunner.h>
#endif
#include <cppunit/TestListener.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestFailure.h>
#include <cppunit/TextOutputter.h>

#include "Assert.hxx"


void SilentAssertHandler(const char* message, const char* filename, int lineNumber )
{
}

class AnsiListener: public CppUnit::TestListener {
	bool _pending;
	std::string _run;
	std::string _fail;
	std::string _error;
	std::string _ok;
public:
	AnsiListener(bool unicode=true)
		: _pending(false)
		, _run(unicode?"\u2699":"R")
		, _fail(unicode?"\u2717":"F")
		, _error(unicode?"!":"E")
		, _ok(unicode?"\u2713":"O")
	{
		// Notes:
		// \033[2K deletes the current line
		// \r moves the cursor to the begining
		// \u2699 is unicode for a gear
		// \u2713 is unicode for a check mark
		// \u2717 is unicode for a cross mark
		// \033[3xm sets the fg color
		// \033[5m sets blinking attribute
		// \033[0m resets colors and attributes
	}
	void startTest(CppUnit::Test * test) {
		_pending=true;
		std::cout << "\033[2K\r \033[34;5m["<<_run<<"]\033[0m "
			<< test->getName() << std::flush;
	}
	void addFailure(const CppUnit::TestFailure & failure) {
		_pending=false;
		if (failure.isError())
			std::cout << "\033[2K\r \033[33m["<<_error<<"]\033[0m ";
		else
			std::cout << "\033[2K\r \033[31m["<<_fail<<"]\033[0m ";

		std::cout
			<< failure.failedTestName() << std::endl;
	}
	void endTest(CppUnit::Test * test) {
		if (not _pending) return;
		_pending=false;
		std::cout << "\033[2K\r\033[32m ["<<_ok<<"] \033[0m"
			<< test->getName() << std::endl;
	}
};


int main( int argc, char* argv[] )
{

	Bioscena::SetAssertFailedHandler(SilentAssertHandler);

#ifdef GUI_ENABLED
	QApplication app(argc,argv); // Needed to run qt app
	CppUnit::QtUi::TestRunner runner;
#else
	CppUnit::TextTestRunner runner;
	
#endif
	AnsiListener listener(true);
	runner.eventManager().addListener(&listener);

	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
	runner.addTest( registry.makeTest() );
	// Run the test.
	return !runner.run("");

}



