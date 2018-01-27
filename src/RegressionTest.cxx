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
#include <cppunit/TestResultCollector.h>
#include <cppunit/SourceLine.h>

#include "Assert.hxx"


void SilentAssertHandler(const char* message, const char* filename, int lineNumber )
{
}

class AnsiListener: public CppUnit::TestListener {
	bool _pending;
	const std::string _run;
	const std::string _fail;
	const std::string _error;
	const std::string _ok;
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

class AnsiOutputter : public CppUnit::TextOutputter {
	CppUnit::OStream & _stream;
	
public:
	AnsiOutputter(CppUnit::TestResultCollector * result, CppUnit::OStream & stream)
		: CppUnit::TextOutputter(result, stream)
		, _stream(stream)
	{
	}

	void printFailureWarning()
	{
		_stream  << "\033[31m!!!FAILURES!!!\033[0m\n";
	}

	void printHeader()
	{
		_stream << "Test Results:\n";
		_stream << "\033[34m[\u2699] Run:  "  <<  m_result->runTests() << "\033[0m";
		unsigned passed = m_result->runTests()-m_result->testFailuresTotal();
		if (passed)
			_stream <<  "   \033[32m[\u2713] Passed: "  <<  passed << "\033[0m";
		if (m_result->testFailures())
			_stream <<  "   \033[31;1m[\u2717] Failed: "  <<  m_result->testFailures() << "\033[0m";
		if (m_result->testErrors())
			_stream <<  "   \033[33m[!] Errors: "  <<  m_result->testErrors() << "\033[0m";
		_stream <<  "\n";
		if (m_result->testFailuresTotal())
			_stream <<  "\n\033[31;1mFAILURES!!!!\033[0m\n";
		else
			_stream <<  "\n\033[32mAll OK\n\033[0m";
	}

	void printFailure(CppUnit::TestFailure *failure, int failureNumber )
	{
		_stream << (failure->isError()?"\033[33m":"\033[31;1m");
		printFailureListMark( failureNumber );
		_stream << ' ';
		printFailureTestName( failure );
		_stream << ' ';
		printFailureType( failure );
		_stream << ' ';
		printFailureLocation( failure->sourceLine() );
		_stream << "\033[0m";
		_stream << "\n";
		printFailureDetail( failure->thrownException() );
		_stream << "\n";
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

	runner.setOutputter(new AnsiOutputter(&runner.result(), std::cerr));

	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
	runner.addTest( registry.makeTest() );
	// Run the test.
	return !runner.run("");

}



