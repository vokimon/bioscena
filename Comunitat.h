// Comunitat.h: interface for the CComunitat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_COMUNITAT_H_INCLUDED)
#define __KKEP_COMUNITAT_H_INCLUDED

#include <vector>
#include "Missatger.h"
#include "InfoOrganisme.h"

class CComunitat  
{
// Construccio/Destruccio
public:
	CComunitat();
	virtual ~CComunitat();
// Redefinibles
public:
// Redefinibles
public:
	virtual istream & load(istream & str);
	virtual ostream & store(ostream & str);
	virtual void dump(CMissatger & msg);
	void dumpDisponibles(CMissatger & msg);
// Operacions
public:
	CInfoOrganisme & operator[](uint32 index);
	uint32 introdueix(COrganisme* org, uint32 posicio, uint32 taxo);
	void extreu (uint32 index);
	uint32 organismeAleatori();
// Atributs
public:
	vector<CInfoOrganisme> m_organismes;
	vector<uint32> m_disponibles;
	uint32 m_nOrganismes;
// Proves
public:
	bool esValid(uint32 index);
	uint32 tamany();
	static void ProvaClasse (void);
};

#endif // !defined(__KKEP_COMUNITAT_H_INCLUDED)
