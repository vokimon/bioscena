// FuncioAgent.h: interface for the CFuncioAgent class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990828 VoK - Separat del fitxer de CAgent
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_FUNCIOAGENT_H_INCLUDED)
#define __KKEP_FUNCIOAGENT_H_INCLUDED

#include "Agent.h"

using namespace std;

// Aquesta classe es per poder fer un Agent de qualsevol objecte valid 
// Valid=respongui al operador () Per exemple una funcio.

//template <class T>
class CFuncioAgent : public CAgent
{
// Tipus Propis
//	typedef T t_enmascarat;
	typedef void (t_enmascarat)();

// Construccio/Destruccio
public:
	CFuncioAgent(t_enmascarat * accio)
	{
		m_tipus+="/Funcio Usuari";
		m_accio=accio;
	}
// Redefinibles
public:
	virtual void operator() (void)
	{
		(*m_accio)();
	}
// Atributs
public:
	t_enmascarat * m_accio;
// Proves
public:
	static void ProvaClasse();
};

#endif // !defined(__KKEP_FUNCIOAGENT_H_INCLUDED)
