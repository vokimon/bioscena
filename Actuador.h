// Actuador.h: interface for the CActuador class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990819 VoK - Creat (a partir de l'escisio de l'antic Cumulador)
// 19990822 VoK - Funcions de dump
// 19990823 VoK - Reordenat els fitxers i netejades les proves
// 19990824 VoK - dump ja no posa ':' despres de nom del parametre
// 19990824 VoK - Funcions de configuracio amb parametres
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_ACTUADOR_H_INCLUDED)
#define __KKEP_ACTUADOR_H_INCLUDED

#include "Agent.h"
#include "Posicionador.h"

class CActuador : public CAgent  
{
// Tipus Propis
	typedef CAgent super;
// Construccio/Destruccio
public:
	CActuador();
	virtual ~CActuador();
// Redefinibles
public:
	virtual void operator() (void);
	virtual void operator() (CSubstrat & s)=0;
	virtual void dump(CMissatger & msg);
	virtual bool configura(string parametre, istream & nom, t_diccionariAgents & diccionari, CMissatger & errors);
	virtual list<CAgent*> dependencies();
// Operacions
public:
	void posicionador (CPosicionador* pos);
	CPosicionador* posicionador ();
// Atributs
public:
	CPosicionador * m_posicionador;
};

#endif // !defined(__KKEP_ACTUADOR_H_INCLUDED)
