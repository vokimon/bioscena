//////////////////////////////////////////////////////////////////////
// Build Number Generator v1.3
// Coded by Vokimon (vokimon@jet.es)
// Compile it with any standard C++ compiler
//////////////////////////////////////////////////////////////////////
// Its execution generates two files:
// * "build.h" A header file defining two constants with the current 
//   build number and date-stamps
// * Previous "build.h" is used to calculate the next buildnumber, so 
//   be careful by altering it.
// * If "build.h" its not present, current buildnumber will be 1.
// * "buildlog.h" A history file containing all the build numbers and 
//   date-stamps as they are generated.

#include <fstream.h>
#include <iomanip.h>
#include <time.h>

int main (int argc, char**argv) {

	char *logfilename="buildlog.txt";
	char *buildfilename="build.h";

	unsigned int build=0;

	char buffer[256];

	cout 
		<< "Build Number Generator v1.3\t"
		<< "Copyright (c) 1998 Vokimon/KKEPerians UNLTD"<<endl;

	ofstream historia(logfilename, ios::app);
	if (!historia) cerr << "Warning: Couldn't append build log entry at file "<< logfilename << endl;

	ifstream entrada(buildfilename, ios::nocreate);
	if (!entrada) {
		cout << "New project: creating build.h"<<endl;
		if (historia) historia << "Building with no build.h present" << endl;
		}
	else {
		entrada.getline(buffer,256);
		entrada.getline(buffer,256);
		entrada >> buffer >> buffer >> build;
		}		

	build++;

	ofstream sortida(buildfilename);
//	ostream &sortida=cout; // line for debugging
	if (!sortida) 
		cerr << "Error: Can't open " << buildfilename << " to modify it" << endl;

	sortida 
		<< "// Build Number Generator v1.0"<<endl
		<< "// Copyright (c) 1998 Vokimon/KKEPerians UNLTD"<<endl
		<< "#define BuildNumber 0x" 
		<< setfill('0')<<setw(4)<<hex<<build<<dec<<endl;
	time_t ara;
	time(&ara);
	struct tm *today = localtime( &ara);
	strftime( buffer, 128, "%Y%m%d %H:%M:%S", today );
	sortida 
		<< "#define BuildDate \"" << buffer << "\""<< endl
		<< "// Don't modify anything in this file but the hexadecimal number above"<<endl
		<< "// If something gets wrong, delete this file, run the generator, and"<<endl
		<< "// modify the hex number acordantly with the last version you remember"<<endl;

	if (historia) {
		historia 
			<< "Building " 
			<< "0x"<< setfill('0')<<setw(4)<< hex<<build<<dec
			<<" - "	<< buffer << endl;
		}

	cout << "Generating build number" << endl
		<<"\t0x" << setfill('0') << hex << setw(4) << build << dec << " - " << buffer << endl;
	return 0;
	}

