// Biosistema.h: interface for the CBiosistema class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990831 VoK - Prototips instruccions organismes
// 19990831 VoK - Ja tenim taxonomista per natros sols
// 19991120 VoK - Implementades les operacions de fenotip
// 19991122 VoK - Proves: Control de teclat + estat visualitzacio
// 19991128 VoK - Els opcodes es llegeixen d'un fitxer
// 19991130 VoK - Implementades les operacions sensorials quimiques
// 19991130 VoK - Reorganitzada la classe
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_BIOSISTEMA_H_INCLUDED)
#define __KKEP_BIOSISTEMA_H_INCLUDED

#include <functional>
#include "Topologia.h"
#include "Substrat.h"
#include "Agent.h"
#include "Comunitat.h"
#include "Probabilitat.h"
//#include "Taxonomista.h"

/*
class COpCode {
	uint32 m_index;
	string m_mnemonic;
}
*/

class CBiosistema  
{
// Tipus propis
public:
	typedef CTopologia<CSubstrat> t_biotop;
	typedef CSubstrat t_substrat;
	typedef CComunitat t_comunitat;
	typedef CAgent t_agent;
//	typedef CTaxonomista t_taxonomista;
	typedef bool (CBiosistema::*t_accioOrganisme) (uint32);
// Atributs
protected:
	t_biotop * m_biotop;
	t_comunitat * m_comunitat;
	t_agent * m_agents;
//	t_taxonomista * m_taxonomista;

	uint32 m_maxInstruccionsUtils;
	uint32 m_maxInstruccions;
	CProbabilitat m_probabilitatGeneracioExpontanea;
	t_accioOrganisme * m_opcodes;
	uint32 m_bitsCodiOperacio;
	uint32 m_nCodisOperacio;
	uint32 m_mascaraCodis;
private:
	uint32 m_instruccionsUtilsRestants;
	uint32 m_instruccionsRestants;
	uint32 m_idOrganismeActiu;
	COrganisme * m_organismeActiu;
	CInfoOrganisme * m_infoOrganismeActiu;
	uint32 m_tempsPerAccionarAgents;
	uint32 m_temps;

// Construccio/Destruccio
public:
	CBiosistema();
	virtual ~CBiosistema();
// Redefinibles
public:
	virtual void operator () (void);
// Operacions (Access als membres)
public:
	bool carregaOpCodes(char * nomFitxer, CMissatger & errors);
	t_biotop * biotop() const;
	void biotop(t_biotop *);
	void deleteBiotop();
	t_comunitat * comunitat() const;
	void comunitat(t_comunitat *);
	void deleteComunitat();
	t_agent * agents() const;
	void agents(t_agent *);
	void deleteAgents();
// Operacions (cicle basic)
	bool organismeExpontani();
	bool eliminaOrganismeActiu();
	void canviaOrganismeActiu();
// Operacions (Instruccions)
public:
	bool organismeMitosi(uint32 parametres);
	bool organismeAvanca(uint32 parametres);
	bool organismeAtaca(uint32 parametres);
	bool organismeEngoleix(uint32 parametres);
	bool organismeExcreta(uint32 parametres);
	bool organismeNoOperacio(uint32 parametres);
	bool organismeAnd(uint32 parametres);
	bool organismeOr(uint32 parametres);
	bool organismeXor(uint32 parametres);
	bool organismeRandom(uint32 parametres);
	bool organismeCopia(uint32 parametres);
	bool organismeCarrega(uint32 parametres);
	bool organismeIdentifica(uint32 parametres);
	bool organismeLocalitza(uint32 parametres);

	bool organismeCreua(uint32 parametres);
	bool organismeCreaSensor(uint32 parametres);
// Proves
public:
	uint32 temps();
	static void ProvaClasse(void);
// Implementacio
protected:
};

#endif // !defined(__KKEP_BIOSISTEMA_H_INCLUDED)
