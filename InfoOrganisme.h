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
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_INFOORGANISME_H_INCLUDED)
#define __KKEP_INFOORGANISME_H_INCLUDED

#include "Organisme.h"

class CInfoOrganisme  
{
// Construccio/Destruccio
public:
	CInfoOrganisme ();
	~CInfoOrganisme ();
// Atributs
public:
	COrganisme *m_organisme;
	uint32 m_posicio;
	uint32 m_taxo;
// Operacions (Acces)
public:
	inline void taxo (uint32 tx) {m_taxo=tx;};
	inline uint32 taxo () {return m_taxo;};
	inline void posicio (uint32 pos) {m_posicio=pos;};
	inline uint32 posicio () {return m_posicio;};
	inline void organisme (COrganisme* org) {
		if (m_organisme) delete m_organisme;
		m_organisme=org;
	}
	inline COrganisme* organisme () {return m_organisme;};
};

#endif // !defined(__KKEP_INFOORGANISME_H_INCLUDED)
