// Temporitzador.h: interface for the CTemporitzador class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990726 VoK - Creat
// 19990727 VoK - Agrupats els grups de parametres en t_cicle's
// 19990728 VoK - Fix: Si els periodes eren zero -> bucle infinit
// 19990822 VoK - Escindides les funcionalitats ara son a CAleaturitzador
// 19990822 VoK - Funcions de dump
// 19990823 VoK - Reordenat els fitxers i netejades les proves
// 19990824 VoK - dump ja no posa ':' despres de nom del parametre
// 19990824 VoK - Funcions de configuracio amb parametres
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_TEMPORITZADOR_H_INCLUDED)
#define __KKEP_TEMPORITZADOR_H_INCLUDED

#include "MultiAgent.h"
#include "RandomStream.h"

class CTemporitzador : public CMultiAgent
{
// Tipus Propis
	typedef CAgent t_accio;
//	typedef void (t_accio)(); // Aixo es per fer proves nomes
	struct t_cicle 
	{
		uint32 m_periodeMinim;
		uint32 m_magnitudDau;
		uint32 m_nombreDaus;
		uint32 periode ()
		{
			uint32 periode;
			// Inicialitzem am el periode minim del cicle
			periode=m_periodeMinim;
			// I li afegim l'extra dels daus
			for (uint32 i=m_nombreDaus; i--;)
				periode+=rnd.get(0,m_magnitudDau);
			return periode;
		}
	};

// Construccio/Destruccio
public:
	CTemporitzador();
	virtual ~CTemporitzador();
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (void);
	virtual void dump(CMissatger & msg);
	bool configura(string parametre, istream & valor, t_diccionariAgents & diccionari, CMissatger & errors);
	virtual list<CAgent*> subordinats (void);
// Operacions
public:
	void cicleActiu(uint32 periode, uint32 margeDau=1, uint32 daus=0);
	void cicleInactiu(uint32 periode, uint32 margeDau=1, uint32 daus=0);
	void cicleActual(uint32 m_restantPeriode);
	void cicleActual(bool actiu, uint32 m_restantPeriode);
private:
	// No fixat al protocol, son aqui per debug
	void antiAccio(t_accio * a);
// Atributs
public:
	bool m_actiu;
	uint32 m_periodeActual;
	t_cicle m_cicleActiu;
	t_cicle m_cicleInactiu;
private:
	// No fixat al protocol, son aqui per debug
	t_accio * m_antiAccio;
// Proves
public:
	static void ProvaClasse();
// Implementacio
private:
	void recalculaActual();
};


#endif // !defined(__KKEP_TEMPORITZADOR_H_INCLUDED)
