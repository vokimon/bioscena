// Comunitat.h: interface for the CComunitat class.
//
//////////////////////////////////////////////////////////////////////
// Change Log: 
// 199909181 VoK - Recreat
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_COMUNITAT_H_INCLUDED)
#define __KKEP_COMUNITAT_H_INCLUDED

#include <algorithm>
#include <functional>
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
	void dump(CMissatger & msg);
	void dumpDisponibles(CMissatger & msg);
// Operacions
public:
	CInfoOrganisme & operator[](uint32 index) {
		return m_organismes[index];
	}
	uint32 introdueix(COrganisme* org, uint32 taxo, uint32 posicio);
	void extreu (uint32 index);
	uint32 organismeAleatori();
// Atributs
public:
	vector<CInfoOrganisme> m_organismes;
	vector<uint32> m_disponibles;
	uint32 m_nOrganismes;
// Proves
public:
	uint32 tamany();
	static void ProvaClasse (void);
};

#endif // !defined(__KKEP_COMUNITAT_H_INCLUDED)
