// Biosistema.h: interface for the CBiosistema class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990831 VoK - Prototips instruccions organismes
// 19990831 VoK - Ja tenim taxonomista per natros sols
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_BIOSISTEMA_H_INCLUDED)
#define __KKEP_BIOSISTEMA_H_INCLUDED

#include "Topologia.h"
#include "Substrat.h"
#include "Agent.h"
#include "Comunitat.h"
//#include "Taxonomista.h"

class CBiosistema  
{
// Tipus propis
public:
	typedef CTopologia<CSubstrat> t_biotop;
	typedef CSubstrat t_substrat;
	typedef CComunitat t_comunitat;
	typedef CAgent t_agent;
//	typedef CTaxonomista t_taxonomista;
// Atributs
protected:
	t_biotop * m_biotop;
	t_comunitat * m_comunitat;
	t_agent * m_agents;
//	t_taxonomista * m_taxonomista;
private:
	uint32 m_instruccionsUtilsRestants;
	uint32 m_maxInstruccionsUtils;
	uint32 m_instruccionsRestants;
	uint32 m_maxInstruccions;
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
	t_biotop * biotop();
	void biotop(t_biotop *);
	void deleteBiotop();
	t_comunitat * comunitat();
	void comunitat(t_comunitat *);
	void deleteComunitat();
	t_agent * agents();
	void agents(t_agent *);
	void deleteAgents();
	bool organismeExpontani();
	bool eliminaOrganismeActiu();
	void canviaOrganismeActiu();
// Operacions (Instruccions)
public:
	bool organismeMitosi(uint32 desp, uint32 energia);
	bool organismeAvanca(uint32 desp, uint32 energia, uint32 clauMoviment);
	bool organismeAtaca(uint32 desp, uint32 elementBase, uint32 tolerancia, uint32 energia, uint32 clauAtac);
	bool organismeCreua(uint32 desp, uint32 energia, uint32 clauCreuament);
	bool organismeEngoleix(uint32 desp, uint32 elementBase, uint32 tolerancia);
	bool organismeExcreta(uint32 desp, uint32 elementBase, uint32 tolerancia);
	bool organismeBiparteix(uint32 desp, uint32 elementBase, uint32 tolerancia);
	bool organismeCreaSensor(uint32 sensor, uint32 vector);
// Proves
public:
	static void ProvaClasse(void);
// Implementacio
protected:
};

#endif // !defined(__KKEP_BIOSISTEMA_H_INCLUDED)
