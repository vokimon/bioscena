// Posicionador.h: interface for the CPosicionador class.
//
//////////////////////////////////////////////////////////////////////
// Class Description:
// 
// Es una classe abstracta que defineix el protocol bàsic per controlar
// una posició en una Topologia.
// - Els objectes que en depenguin d'un posicionador poden coneixer la 
// seva posicio amb la funcio pos()
// - Es pot canviar explicitament amb el membre pos(unaPosicio)
// - L'operador ()(void) permet especificar a les subclasses la forma
// de anar modificant la posició (per defecte es mante igual)
// 
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990822 VoK - Funcions de dump
// 19990823 VoK - Reordenat els fitxers
// 19990824 VoK - dump ja no posa ':' despres de nom del parametre
// 19990824 VoK - Funcions de configuracio amb parametres
// 20000220 VoK - Adaptat topologia sense templates
// 20000220 VoK - Canviat el nom dels tipus interns tipus_X -> t_X
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_POSICIONADOR_H_INCLUDED)
#define __KKEP_POSICIONADOR_H_INCLUDED

#include "Agent.h"
#include "Topologia.h"
#include "Substrat.h"

//////////////////////////////////////////////////////////////////////
// CPosicionador: Controla una posicio (per defecte fixa)
//////////////////////////////////////////////////////////////////////

class CPosicionador : public CAgent
{
// Tipus interns
public:
	typedef CAgent inherited;
	typedef CTopologia t_biotop;
	typedef CTopologia::t_posicio t_posicio;
	typedef CTopologia::t_desplacament t_direccio;
// Construccio/Destruccio
public:
	CPosicionador(t_biotop& biotop);
	virtual ~CPosicionador() {};
// Redefinibles
public:
	virtual void operator() (void) {};
	virtual void dump(CMissatger& msg);
	virtual bool configura(string parametre, istream & nom, t_diccionariAgents & diccionari, CMissatger & errors);
// Operacions
public:
	t_posicio pos() {return m_pos;};;
	void pos(t_posicio nova);
	CPosicionador & operator = (t_posicio nova) {m_pos=nova; return *this;};
// Atributs
protected:
	t_posicio m_pos;
	t_biotop & m_biotop;
};

//////////////////////////////////////////////////////////////////////
// CDireccionador: Controla una direccio (per defecte fixa)
//////////////////////////////////////////////////////////////////////

class CDireccionador : public CAgent
{
// Tipus interns
public:
	typedef CAgent inherited;
	typedef CTopologia t_biotop;
	typedef CTopologia::t_posicio t_posicio;
	typedef CTopologia::t_desplacament t_direccio;
// Construccio/Destruccio
public:
	CDireccionador(t_biotop& biotop);
	virtual ~CDireccionador() {};
// Redefinibles
public:
	virtual void operator() (void) {};
	virtual void dump (CMissatger & msg);
	virtual bool configura(string parametre, istream & nom, t_diccionariAgents & diccionari, CMissatger & errors);
// Operacions
public:
	t_direccio dir() {return m_dir;};
	void dir(t_direccio nova) {m_dir=nova;};
	CDireccionador & operator = (t_direccio nova) {m_dir=nova; return *this;};
// Atributs
protected:
	t_direccio m_dir;
	t_biotop & m_biotop;
// Proves
public:
	static void ProvaClasse();
};


#endif // !defined(__KKEP_POSICIONADOR_H_INCLUDED)
