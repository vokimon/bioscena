// ParserAgents.h: interface for the CParserAgents class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_PARSERAGENTS_H_INCLUDED)
#define __KKEP_PARSERAGENTS_H_INCLUDED

#include <map>
#include <string>
#include "Agent.h"
#include "Topologia.h"
#include "Substrat.h"

class CParserAgents  
{
// Tipus Propis
	typedef map<string, CAgent*> t_diccionariAgents; 
public:
	CParserAgents();
	virtual ~CParserAgents();
};

#endif // !defined(__KKEP_PARSERAGENTS_H_INCLUDED)
