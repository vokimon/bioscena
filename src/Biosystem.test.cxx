#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "Biosistema.h"
#include "Missatger.h"
using Bioscena::Topology;

namespace BioscenaTest {


const std::string opcodes=
"* 00 NoOperacio\n"
"* 10 NoOperacio\n"
"* 01 Mitosi\n"
"* 11 Mitosi\n"
"* 02 Anabol\n"
"* 12 Catabol\n"
"* 03 Engoleix\n"
"* 13 Engoleix\n"
"* 04 Excreta\n"
"* 14 Excreta\n"
"* 05 Ataca\n"
"* 15 Ataca\n"
"* 06 Avanca\n"
"* 16 Avanca\n"
"* 07 SensorI\n"
"* 17 SensorI\n"
"* 08 SensorP\n"
"* 18 SensorP\n"
"* 09 SensorQ\n"
"* 19 SensorQ\n"
"* 0A Random\n"
"* 1A Random\n"
"* 0B Carrega\n"
"* 1B Carrega\n"
"* 0C And\n"
"* 1C Or\n"
"* 0D Xor\n"
"* 1D Copia\n"
"* 0E Oposa\n"
"* 1E Not\n"
"* 0F ShiftR\n"
"* 1F ShiftL\n"
;

class BiosystemTest;
CPPUNIT_TEST_SUITE_REGISTRATION(BiosystemTest);

class BiosystemTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( BiosystemTest );
	CPPUNIT_TEST( test_opcodes_byDefaultAllNops );
	CPPUNIT_TEST( test_addOperation );
	CPPUNIT_TEST( test_addOperation_withInvalidMnemonic );
	CPPUNIT_TEST( test_addOperation_whenCodeBeyondBits );
	CPPUNIT_TEST( test_load_ok );
	CPPUNIT_TEST( test_load_badline );
//	CPPUNIT_TEST( test_ClassTest );
	CPPUNIT_TEST_SUITE_END();
	typedef CBiosistema Biosystem;

	const std::string configFile = "config/opcodes.ini";

public:
	void setUp() {
	}
	void test_opcodes_byDefaultAllNops()
	{
		Biosystem biosystem;
		std::string op = biosystem.operationDescriptor(0x0000000);
		CPPUNIT_ASSERT_EQUAL(op, std::string("NoOperacio"));
		std::string op2 = biosystem.operationDescriptor(0xabdcef1);
		CPPUNIT_ASSERT_EQUAL(op2, std::string("NoOperacio"));
	}
	void test_addOperation()
	{
		Biosystem biosystem;
		biosystem.clearOpcodes();
		biosystem.addOperation(0x03, "Carrega");
		std::string opcode = biosystem.operationDescriptor(0x03);
		CPPUNIT_ASSERT_EQUAL(std::string("Carrega"), opcode);
	}
	void test_addOperation_withInvalidMnemonic()
	{
		Biosystem biosystem;
		biosystem.clearOpcodes();
		try {
			biosystem.addOperation(0x03, "BadMnemonic");
			CPPUNIT_FAIL("Should have thrown");
		} catch (OpcodeConfigError & e) {
			CPPUNIT_ASSERT_EQUAL(std::string(
				"Mnemonic 'BadMnemonic' does not exist"),
				std::string(e.what()));
		}
	}
	void test_addOperation_whenCodeBeyondBits()
	{
		Biosystem biosystem;
		biosystem.clearOpcodes();
		try {
			biosystem.addOperation(0x20, "Carrega");
			CPPUNIT_FAIL("Should have thrown");
		} catch (OpcodeConfigError & e) {
			CPPUNIT_ASSERT_EQUAL(std::string(
				"Operation opcode 0x20 cannot be represented with 5 bits"),
				std::string(e.what()));
		}
	}
	void test_load_ok()
	{
		Biosystem biosystem;
		std::istringstream configFile(opcodes);
		biosystem.carregaOpCodes(configFile, error);
		std::string op = biosystem.operationDescriptor(0x0000);
		CPPUNIT_ASSERT_EQUAL(op, std::string("NoOperacio"));
		std::string op2 = biosystem.operationDescriptor(0x001f);
		CPPUNIT_ASSERT_EQUAL(op2, std::string("ShiftL"));
	}
	void test_load_badline()
	{
		Biosystem biosystem;
		std::istringstream configFile(
			"badline\n"
		);
		try {
			bool ok = biosystem.carregaOpCodes(configFile, error);
			CPPUNIT_FAIL("Should have raised an exception");
		} catch (OpcodeConfigError & e) {
			CPPUNIT_ASSERT_EQUAL(std::string(e.what()),
				std::string("Unexpected 'badline' in opcode configuration"));
		}
	}
};

}// BioscenaTest namespace
