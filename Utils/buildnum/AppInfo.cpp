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
	out << *this;
}

/////////////////////////////////////////////////////////////////////
// Amigues
/////////////////////////////////////////////////////////////////////
ostream &operator<<(ostream& stream, CAppInfo &info)
{
	stream << info.m_name << " v" << info.m_major << "." << info.m_minor;
	stream << " Build 0x" << setfill('0') << setw(4) << hex;
	stream << info.m_buildNumber <<" - " << info.m_buildDate <<endl;
	stream << setfill('0') << setw(0) << dec;
	stream << "Copyright (c) 1998 "<<info.m_designer<<"/"<<info.m_enterprise<<endl;
	return stream;
}

/////////////////////////////////////////////////////////////////////
// Inicialitzem l'informacio de la nostra aplicacio 
// (Modifiqueu-la com convingui)
/////////////////////////////////////////////////////////////////////
CAppInfo CAppInfo::MyAppInfo("Bioscena",1,1,"Vokimon","KKEPerians UNLTD",BuildNumber,BuildDate);

