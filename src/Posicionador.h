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

#if !defined(__KKEP_POSICIONADOR_H_INCLUDED)
#define __KKEP_POSICIONADOR_H_INCLUDED

#include "Agent.h"
#include "Topology.hxx"
#include "Substrat.h"
#include "RandomStream.hxx"

//////////////////////////////////////////////////////////////////////
// CPosicionador: Controla una posicio (per defecte fixa)
//////////////////////////////////////////////////////////////////////

class CPosicionador : public CAgent
{
// Tipus interns
public:
	typedef CAgent inherited;
	typedef Bioscena::Topology t_biotop;
	typedef t_biotop::Position Position;
	typedef t_biotop::Displacement t_direccio;
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
	Position pos() {return m_pos;};;
	void pos(Position nova);
	CPosicionador & operator = (Position nova) {m_pos=nova; return *this;};
// Atributs
protected:
	Position m_pos;
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
	typedef Bioscena::Topology t_biotop;
	typedef t_biotop::Position Position;
	typedef t_biotop::Displacement t_direccio;
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
