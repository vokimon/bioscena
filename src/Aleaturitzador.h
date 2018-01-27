// Aleaturitzador.h: interface for the CAleaturitzador class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990822 VoK - Creat a partir de l'escisi� CTemporitzador que abans
//                incloia tambe aquesta funcionalitat
// 19990822 VoK - Funcions de dump
// 19990823 VoK - Reordenat els fitxers i netejades les proves
// 19990824 VoK - dump ja no posa ':' despres de nom del parametre
// 19990824 VoK - Funcions de configuracio amb parametres
// 19991116 VoK - Fa servir CProbabilitat en comptes de l'intern 
//                t_probabilitat_saxona. Adaptat al nou tipos.
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_ALEATURITZADOR_H_INCLUDED)
#define __KKEP_ALEATURITZADOR_H_INCLUDED

#include "RandomStream.hxx"
#include "Probability.hxx"
#include "MultiAgent.h"

class CAleaturitzador : public CMultiAgent
{
// Tipus interns
public:
	typedef CMultiAgent inherited;
	typedef CAgent t_accio;
//	typedef void (t_accio)(); // Aixo es per fer proves nomes
// Construccio/Destruccio
public:
	CAleaturitzador();
	virtual ~CAleaturitzador();
// Redefinibles
public:
	virtual void operator() (void);
	virtual void dump(CMissatger & msg);
	virtual bool configura(string parametre, istream & valor, t_diccionariAgents & diccionari, CMissatger & errors);
	virtual list<CAgent*> subordinats (void);
// Operacions
public:
	void probabilitat(uint32 mostra,uint32 encerts=1);
protected:
	// No fixat al protocol, son aqui per debug
	void reAccio(t_accio * a);
// Atributs
public:
	bool m_accionat;
	Bioscena::Probability m_probabilitat;
protected:
	// No fixat al protocol, son aqui per debug
	t_accio * m_reAccio;
// Proves
public:
	static void ProvaClasse();
};


#endif // !defined(__KKEP_ALEATURITZADOR_H_INCLUDED)
