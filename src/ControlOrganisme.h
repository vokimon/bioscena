// ControlOrganisme.h: interface for the CControlOrganisme class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19991214 VoK - Creada com a abstraccio de CGenotip
// 19991214 VoK - Afegides funcions per fer-la una factoria
// 20000708 VoK - Podem obtenir el tipus d'un control
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_CONTROLORGANISME_H_INCLUDED)
#define __KKEP_CONTROLORGANISME_H_INCLUDED

#include "BioIncludes.h"
#include "Missatger.h"
#include "Gen.h"
#include "Cariotip.h"

/**
* Abstracts the control structure that issues biological 
* instructions for an organism.
* An organism control is an structure that is able to issue 
* instruction codes for an organism to execute.
* When the organism borns, the control structure is 
* initialized from the content of a Cariotip.
* During life, the organism control uses the fenotipical
* information to decide the instructions that will be issued.
* <p>
* Being both, the fenotype and the issued instruction simple 
* integers, they are decoupled from their semantic meaning 
* and gives some freedom on the concrete control implementation
* to use.
*/
class CControlOrganisme
{
// Tipus propis
	typedef void inherited;
// Construccio/Destruccio
public:
	CControlOrganisme() {m_tipus+="ControlOrganisme";}
	virtual ~CControlOrganisme() {}
// Operacions
public:
	/** 
	* Constructs the control structure from a cariotype
	* @param cariotip The molding cariotype
	* @returns Whether the init has been succesfull
	*/
	virtual bool init(const CCariotip & cariotip)=0;
	/**
	* Resets the state of the control like if an init
	* was never called. Is needed to call init a second 
	* time.
	*/
	virtual void clear()=0;
	/**
	* Selects an action code to execute.
	* @param the fenotype
	* @returns the action code
	*/
	virtual uint32 seguentInstruccio(const uint32 * fenotip)=0;
	/**
	* Dumps its internal status on an output stream for debug purposes.
	* @param out the output stream
	*/
	virtual void dump(CMissatger & out)=0;
// Operacions no redefinibles
public:
	/**
	* Gives the string representing the Control type
	* @returns A string that identifies the concrete Control type
	*/
	const string & tipus() {return m_tipus;};
// Atributs
protected:
	string m_tipus;
// Factoria
public:
	static uint32 Nombre(void);
	static CControlOrganisme * Crea(uint32 n);
	static CControlOrganisme * Crea(const string&  tipus);
// Proves
public:
	//* No es pot fer virtual pura pero caldria fer-la en totes les subclasses
	static void ProvaClasse();
};

#endif // !defined(__KKEP_CONTROLORGANISME_H_INCLUDED)

