// Iterador.h: interface for the CIterador class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990822 VoK - Creat
// 19990822 VoK - Funcions de dump
// 19990823 VoK - Reordenat els fitxers i netejades les proves
// 19990824 VoK - dump ja no posa ':' despres de nom del parametre
// 19990824 VoK - Funcions de configuracio amb parametres
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_ITERADOR_H_INCLUDED)
#define __KKEP_ITERADOR_H_INCLUDED

#include "MultiAgent.h"

class CIterador : public CMultiAgent  
{
// Tipus Propis
	typedef CAgent t_accio;
//	typedef void (t_accio)(); // Aixo es per fer proves nomes

// Construccio/Destruccio
public:
	CIterador();
	virtual ~CIterador();
// Redefinibles
public:
	virtual void operator() (void);
	virtual bool configura(string parametre, istream & valor, t_diccionariAgents & diccionari, CMissatger & errors);
	virtual void dump(CMissatger & msg);
	virtual list<CAgent*> subordinats (void);
// Operacions
public:
	void iteracions(uint32 iteracions, uint32 magDaus=0, uint32 nDaus=0);
protected:
	// No fixat al protocol, son aqui per debug
	void preAccio(t_accio * a);
	void postAccio(t_accio * a);
// Atributs
public:
	uint32 m_iteracionsMinimes;
	uint32 m_nombreDaus;
	uint32 m_magnitudDau;
	uint32 m_accionats;
protected:
	// No fixat al protocol, son aqui per debug
	t_accio * m_preAccio;
	t_accio * m_postAccio;
// Proves
public:
	static void ProvaClasse();
};

#endif // !defined(__KKEP_ITERADOR_H_INCLUDED)
