#ifndef _KKEP_APPINFO_H
#define _KKEP_APPINFO_H
#include "Missatger.h"

class CAppInfo {
// Constructor
public:
	CAppInfo(char *name, unsigned int major, unsigned int minor, 
		char *designer, char *enterprise, 
		unsigned int buildNumber, char *buildDate);
	template<class output> void printOn (output & sortida);
// Atributes
private:
	char *m_name;
	unsigned int m_major, m_minor;
	char *m_designer, *m_enterprise;
	unsigned int m_buildNumber;
	char *m_buildDate;
// Static member data
public:
	static CAppInfo MyAppInfo;
};

#endif //_KKEP_APPINFO_H

