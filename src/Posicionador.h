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

#include "Topologia.h"
#include "Substrat.h"

class CPosicionador  
{
// Tipus
	typedef CTopologia<CSubstrat> tipus_biotop;
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
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (void) {};
// Atributs
protected:
	tipus_posicio m_pos;
	tipus_biotop & m_biotop;
};


#endif // !defined(__KKEP_ITINERARI_H_INCLUDED)
