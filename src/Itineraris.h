// Itineraris.h: CPosicionador & CDireccionador subclasses' interface
//
//////////////////////////////////////////////////////////////////////
// Class Description:
// Aquesta capcelera descriu els prototips per a les especialitzacions
// de CPosicionador i CDireccionador
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990822 VoK - Funcions de dump
// 19990823 VoK - Reordenat els fitxers i netejades les proves
// 19990823 VoK - Eliminadas funciones inline que passan al cpp
// 19990823 VoK - Completadas las funcionalidades estandard de CPosicionadorZonal
// 19990824 VoK - dump ja no posa ':' despres de nom del parametre
// 19990824 VoK - Funcions de configuracio amb parametres
// 19991130 VoK - CPosicionador Zonal adaptat per fer servir la funcio
//                'desplacamentAleatori' de biotop especificant radi
//////////////////////////////////////////////////////////////////////
// TODO: Fer Direccionadors i Posicionadors segons una sequencia.
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_ITINERARIS_H_INCLUDED)
#define __KKEP_ITINERARIS_H_INCLUDED

#include "Posicionador.h"
#include "RandomStream.h"

//////////////////////////////////////////////////////////////////////
// CItinerari: Varia la posicio respecte la posició anterior, segons 
// un direccionador (del qual depen).
//////////////////////////////////////////////////////////////////////

class CItinerari : public CPosicionador
{
// Tipus interns
public:
	typedef CPosicionador inherited;
// Construccio/Destruccio
public:
	CItinerari(tipus_biotop & biotop);
// Redefinibles
public:
	virtual void operator() (void);
	virtual void dump(CMissatger & msg);
	virtual bool configura(string parametre, istream & nom, t_diccionariAgents & diccionari, CMissatger & errors);
	virtual list<CAgent*> dependencies();
// Operacions
public:
	void direccionador(CDireccionador * dir);
	CDireccionador * direccionador();
	uint32 radi();
	void radi(uint32 rad);
// Atributs
protected:
	CDireccionador * m_direccionador;
	uint32 m_radi;
// Proves
public:
	static void ProvaClasse();
};

//////////////////////////////////////////////////////////////////////
// CPosicionadorZonal: Varia la posicio dintre d'un zona
//////////////////////////////////////////////////////////////////////

class CPosicionadorZonal : public CPosicionador
{
// Tipus interns
public:
	typedef CPosicionador inherited;
// Construccio/Destruccio
public:
	CPosicionadorZonal(tipus_biotop & biotop);
// Redefinibles
public:
	virtual void operator() (void);
	virtual void dump(CMissatger & msg);
	virtual bool configura(string parametre, istream & nom, t_diccionariAgents & diccionari, CMissatger & errors);
	virtual list<CAgent*> dependencies();
// Operacions
public:
	void posicionador(CPosicionador * pos);
	CPosicionador * posicionador();
	uint32 radi();
	void radi(uint32 rad);
// Atributs
protected:
	CPosicionador * m_posicionador;
	uint32 m_radi;
// Proves
public:
	static void ProvaClasse();
};

//////////////////////////////////////////////////////////////////////
// CPosicionadorAleatori: Varia la Posicio aleatoriament
//////////////////////////////////////////////////////////////////////

class CPosicionadorAleatori : public CPosicionador
{
// Tipus interns
public:
	typedef CPosicionador inherited;
// Construccio/Destruccio
public:
	CPosicionadorAleatori(tipus_biotop & biotop);
// Redefinibles
public:
	virtual void operator() (void);
// Proves
public:
	static void ProvaClasse();
};

//////////////////////////////////////////////////////////////////////
// CDireccionadorAleatori: Varia la direccio aleatoriament
//////////////////////////////////////////////////////////////////////

class CDireccionadorAleatori : public CDireccionador
{
// Tipus interns
public:
	typedef CDireccionador inherited;
// Construccio/Destruccio
public:
	CDireccionadorAleatori(tipus_biotop & biotop);
// Redefinibles
public:
	virtual void operator() (void);
// Proves
public:
	static void ProvaClasse();
};

#endif // !defined(__KKEP_ITINERARIS_H_INCLUDED)
