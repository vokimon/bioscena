// ParserAgents.h: interface for the CParserAgents class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_PARSERAGENTS_H_INCLUDED)
#define __KKEP_PARSERAGENTS_H_INCLUDED

#include "Agent.h"
#include "Topologia.h"
#include "Substrat.h"

class CParserAgents  
{
public:
	static CAgent * CreaAgent(string tipus, CTopologia<CSubstrat>& biotop);
	static void ProvaClasse();
	CParserAgents();
	virtual ~CParserAgents();
	static CAgent * ParsejaArxiuAgents(char * nomArxiu, CTopologia<CSubstrat> & biotop, list<string> & errors);
};

#endif // !defined(__KKEP_PARSERAGENTS_H_INCLUDED)
