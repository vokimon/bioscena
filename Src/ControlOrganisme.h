// ControlOrganisme.h: interface for the CControlOrganisme class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19991214 VoK - Creada com a abstraccio de CGenotip
// 19991214 VoK - Afegides funcions per fer-la una factoria
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_CONTROLORGANISME_H_INCLUDED)
#define __KKEP_CONTROLORGANISME_H_INCLUDED

#include "BioIncludes.h"
#include "Missatger.h"
#include "Gen.h"
#include "Cariotip.h"

class CControlOrganisme
{
// Construccio/Destruccio
public:
	CControlOrganisme() {}
	virtual ~CControlOrganisme() {}
// Operacions
public:
	// Construeix l'estructura de control a partir d'un cariotip
	virtual bool init(CCariotip& c)=0;
	// Dona una instruccio generada pel sistema de control
	virtual uint32 seguentInstruccio(uint32 * fenotip)=0;
	// Representa al CMissatger l'estructura de control (Estat actual?)
	virtual void dump(CMissatger & msg)=0;
// Factoria
public:
	static uint32 Nombre(void);
	static CControlOrganisme * Crea(uint32 n);
	static CControlOrganisme * Crea(string tipus);
// Proves
public:
	// No es pot fer virtual pura pero caldria fer-la en totes les subclasses
	static void ProvaClasse();
};

#endif // !defined(__KKEP_CONTROLORGANISME_H_INCLUDED)
