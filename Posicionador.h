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

#if !defined(__KKEP_ITINERARI_H_INCLUDED)
#define __KKEP_ITINERARI_H_INCLUDED

#include "Agent.h"
#include "Topologia.h"
#include "Substrat.h"

//////////////////////////////////////////////////////////////////////
// CPosicionador: Controla una posicio (per defecte fixa)
//////////////////////////////////////////////////////////////////////

class CPosicionador : public CAgent
{
// Tipus
	typedef CSubstrat tipus_substrat;
	typedef CTopologia<tipus_substrat> tipus_biotop;
	typedef uint32 tipus_posicio;
	typedef uint32 tipus_direccio;
// Construccio/Destruccio
public:
	CPosicionador(tipus_biotop& biotop):m_biotop(biotop){m_pos=0;};
	virtual ~CPosicionador() {};
// Operacions
public:
	tipus_posicio pos() {return m_pos;};;
	void pos(tipus_posicio nova) {m_pos=nova;};
	CPosicionador & operator = (tipus_posicio nova) {m_pos=nova; return *this;};
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (void) {};
// Atributs
protected:
	tipus_posicio m_pos;
	tipus_biotop & m_biotop;
};

//////////////////////////////////////////////////////////////////////
// CDireccionador: Controla una direccio (per defecte fixa)
//////////////////////////////////////////////////////////////////////

class CDireccionador : public CAgent
{
// Tipus
	typedef CSubstrat tipus_substrat;
	typedef CTopologia<tipus_substrat> tipus_biotop;
	typedef uint32 tipus_posicio;
	typedef uint32 tipus_direccio;
// Construccio/Destruccio
public:
	CDireccionador(tipus_biotop& biotop):m_biotop(biotop){m_dir=0;};
	virtual ~CDireccionador() {};
// Operacions
public:
	tipus_direccio dir() {return m_dir;};
	void dir(tipus_direccio nova) {m_dir=nova;};
	CDireccionador & operator = (tipus_direccio nova) {m_dir=nova; return *this;};
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (void) {};
// Atributs
protected:
	tipus_posicio m_dir;
	tipus_biotop & m_biotop;
};


#endif // !defined(__KKEP_ITINERARI_H_INCLUDED)
