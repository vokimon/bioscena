#include <iomanip>
#include "AppInfo.h"
#include "Missatger.h"

#ifndef _KKEP_BUSCANT_DEPENDENCIES
#include "build.h"
#endif

/////////////////////////////////////////////////////////////////////
// Construcctor
/////////////////////////////////////////////////////////////////////
CAppInfo::CAppInfo(char *name, unsigned int major, unsigned int minor, 
	char *designer, char *enterprise, 
	unsigned int buildNumber, char *buildDate)
{
	m_name=name;
	m_major=major;
	m_minor=minor;
	m_designer=designer;
	m_enterprise=enterprise;
	m_buildDate=buildDate;
	m_buildNumber=buildNumber;
	printOn(out);
}
/////////////////////////////////////////////////////////////////////
// Operacions
/////////////////////////////////////////////////////////////////////
template<class output> void CAppInfo::printOn (output& sortida)
{
	sortida << "\n" << m_name << " v" << m_major << "." << m_minor;
	sortida << " Build 0x" << setfill('0') << setw(4) << hex;
	sortida << m_buildNumber <<" - " << m_buildDate <<endl;
	sortida << setfill('0') << setw(0) << dec;
	sortida << "Copyright (c) 1998 "<<m_designer<<"/"<<m_enterprise<<endl<<endl;
}

/////////////////////////////////////////////////////////////////////
// Operacions
/////////////////////////////////////////////////////////////////////
CAppInfo CAppInfo::MyAppInfo("Bioscena",1,1,"Vokimon","KKEPerians UNLTD",BuildNumber,BuildDate);


