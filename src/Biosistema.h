// Biosistema.h: interface for the CBiosistema class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_BIOSISTEMA_H_INCLUDED)
#define __KKEP_BIOSISTEMA_H_INCLUDED

#include <functional>
#include "Biotop.h"
#include "Substrat.h"
#include "Agent.h"
#include "Comunitat.h"
#include "Probability.hxx"
#include "Taxonomista.h"
#include <memory>
/*
class CParametreOpcode {
	uint32 offset;
	uint32 mascara;
	string nom;
	}
   
class COpCode {
	uint32 m_index;
	string m_mnemonic;
	bool (CBiosistema::*m_accio) (uint32);
	
}
*/

class OpcodeConfigError: public std::exception
{
	std::string _message;
public:
	OpcodeConfigError(const std::string& message)
		: _message(message) {}
	virtual ~OpcodeConfigError() {}
	const char	* what() const throw() {
		return _message.c_str();
	}
};


class CBiosistema  
{
// Tipus propis
public:
	typedef Bioscena::CBiotop<CSubstrat> t_biotop;
	typedef CSubstrat t_substrat;
	typedef CComunitat t_comunitat;
	typedef CAgent t_agent;
	typedef CTaxonomista t_taxonomista;
	typedef bool (CBiosistema::*t_accioOrganisme) (uint32);
// Atributs
protected:
	t_biotop * m_biotop;
	t_comunitat * m_comunitat;
	t_agent * m_agents;
	t_taxonomista * m_taxonomista;

	uint32 m_maxInstruccionsUtils;
	uint32 m_maxInstruccions;
	Bioscena::Probability m_probabilitatGeneracioExpontanea;
	t_accioOrganisme * m_opcodes;
	std::unique_ptr<uint32[]> _opcodeIndexes;
	uint32 m_bitsCodiOperacio;
	uint32 m_nCodisOperacio;
	uint32 m_mascaraCodis;
	uint32 m_mascaraQuimica;
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
	virtual istream & load(istream & str);
	virtual ostream & store(ostream & str);
// Operacions (Access als membres)
public:
	void addOperation(uint32 opcode, const std::string & mnemonic);
	std::string operationDescriptor(uint32 operaion);
	void clearOpcodes();
	bool carregaOpCodes(const std::string & nomFitxer, CMissatger & errors);
	bool carregaOpCodes(std::istream & entrada, CMissatger & errors);
	t_biotop * biotop() const;
	void biotop(t_biotop *);
	void deleteBiotop();
	t_comunitat * comunitat() const;
	void comunitat(t_comunitat *);
	void deleteComunitat();
	t_agent * agents() const;
	void agents(t_agent *);
	void deleteAgents();
	t_taxonomista * taxonomista() const;
	void taxonomista(t_taxonomista *);
	void deleteTaxonomista();
	uint32 temps();
// Operacions (cicle basic)
	bool organismeExpontani();
	bool eliminaOrganismeActiu();
	void canviaOrganismeActiu();
	bool introdueix(COrganisme * org, uint32 pos = ~uint32(0));
	void activaLog(bool actiu);
// Helpers
private:
	uint32 & fenotip(uint32 params, uint32 n);
// Operacions (Instruccions)
public:
	bool organismeMitosi(uint32 parametres);
	bool organismeAvanca(uint32 parametres);
	bool organismeAtaca(uint32 parametres);
	bool organismeEngoleix(uint32 parametres);
	bool organismeExcreta(uint32 parametres);
	bool organismeCatabolitza(uint32 parametres);
	bool organismeAnabolitza(uint32 parametres);
	bool organismeNoOperacio(uint32 parametres);
	bool organismeAnd(uint32 parametres);
	bool organismeOr(uint32 parametres);
	bool organismeXor(uint32 parametres);
	bool organismeShiftRight(uint32 parametres);
	bool organismeShiftLeft(uint32 parametres);
	bool organismeNot(uint32 parametres);
	bool organismeOposa(uint32 parametres);
	bool organismeRandom(uint32 parametres);
	bool organismeCopia(uint32 parametres);
	bool organismeCarrega(uint32 parametres);
	bool organismeSensorQuimic(uint32 parametres);
	bool organismeSensorPresencia(uint32 parametres);
	bool organismeSensorIntern(uint32 parametres);

// Proves
public:
	static void ProvaClasse(void);
// Implementacio
protected:
};

#endif // !defined(__KKEP_BIOSISTEMA_H_INCLUDED)
