// Aleaturitzador.h: interface for the CAleaturitzador class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_ALEATURITZADOR_H_INCLUDED)
#define __KKEP_ALEATURITZADOR_H_INCLUDED

#include <string>
#include "BioIncludes.h"
#include "RandomStream.h"
#include "Missatger.h"
#include "MultiAgent.h"

class CAleaturitzador : public CMultiAgent
{
	typedef CAgent t_accio;
//	typedef void (TipusAccio)(); // Aixo es per fer proves nomes
	struct t_probabilitat_saxona
	{
		bool esDona()
		{
			return rnd.get(0,m_mostra-1)<m_encerts;
		};
		uint32 m_encerts;
		uint32 m_mostra;
	};

// Construccio/Destruccio
public:
	CAleaturitzador();
	virtual ~CAleaturitzador();
// Operacions
public:
	virtual list<CAgent*> subordinats();
	void operator()(void);
	void probabilitat(uint32 mostra,uint32 encerts=1);
private:
	// No fixat al protocol, son aqui per debug
	void reAccio(t_accio * a);
// Atributs
public:
	bool m_accionat;
	t_probabilitat_saxona m_probabilitat;
private:
	// No fixat al protocol, son aqui per debug
	t_accio * m_reAccio;
// Proves
public:
	virtual void dump(CMissatger & msg);
	static void ProvaClasse();
};


#endif // !defined(__KKEP_ALEATURITZADOR_H_INCLUDED)
