// InfoOrganisme.h: interface for the CInfoOrganisme class.
//
// Proposit:
//  Mante informacio associada a un organisme en la relacio amb 
//  el seu entorn: posicio, taxo, estadistiques, progenitor...
//////////////////////////////////////////////////////////////////////
// Change Log: 
// 19990814 VoK - Creat del que abans era CNodeComunitat
// 19990814 VoK - Afegides funcions membres d'acces
// 19990814 VoK - Responsabilitzat d'eliminar l'organisme si en penja un
// 19990918 VoK - Renombrat l'organisme com a 'cos'
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_INFOORGANISME_H_INCLUDED)
#define __KKEP_INFOORGANISME_H_INCLUDED

#include <string>
#include <iostream>
#include <iomanip>
#include "Organisme.h"

class CInfoOrganisme  
{
// Construccio/Destruccio
public:
	CInfoOrganisme ();
	~CInfoOrganisme ();
// Atributs
public:
	COrganisme *m_cos;
	uint32 m_posicio;
	uint32 m_taxo;
//	bool m_propietari;
	uint32 m_subIdentificador;
	string m_representacio;
// Operacions (Acces)
public:
	inline void taxo (uint32 tx) {m_taxo=tx;};
	inline uint32 taxo () {return m_taxo;};
	inline void posicio (uint32 pos) {m_posicio=pos;};
	inline uint32 posicio () {return m_posicio;};
	inline void descripcio(string rep) {m_representacio=rep;};
	inline string descripcio() {return m_representacio;};
	inline void cos (COrganisme* org) {
		if (m_cos) delete m_cos;
		if (org) ++m_subIdentificador;
		m_cos=org;
	}
	inline COrganisme* cos() {/*m_propietari=true;*/ return m_cos;};
	inline uint32 subidentificador () {return m_subIdentificador;};
	// TODO: Borrar esto si no hace falta
//	bool operator< (const COrganisme & o) const {return false;};
//	bool operator< (const COrganisme & o) {return false;};
};

#endif // !defined(__KKEP_INFOORGANISME_H_INCLUDED)
