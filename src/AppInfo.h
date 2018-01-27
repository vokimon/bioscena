/////////////////////////////////////////////////////////////////////
// Change Log
// 19990717 VoK - Creat
// 19990721 VoK - Fa servir un ostream en comptes d'un template

#ifndef _KKEP_APPINFO_H
#define _KKEP_APPINFO_H
#include "Missatger.h"

class CAppInfo {
// Constructor
public:
	CAppInfo(const char *name, unsigned int major, unsigned int minor, 
		const char *designer, const char *enterprise, 
		unsigned int buildNumber, const char *buildDate);
	friend ostream &operator<<(ostream& stream, CAppInfo &info);
// Atributes
private:
	const char *m_name;
	unsigned int m_major, m_minor;
	const char *m_designer, *m_enterprise;
	unsigned int m_buildNumber;
	const char *m_buildDate;
// Static member data
public:
	static CAppInfo MyAppInfo;
};

ostream &operator<<(ostream& stream, CAppInfo &info);

#endif //_KKEP_APPINFO_H

