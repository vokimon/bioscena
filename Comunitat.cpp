// Comunitat.cpp: implementation of the CComunitat class.
//
//////////////////////////////////////////////////////////////////////

#include "Comunitat.h"
#include "Configuracio.h"
#include "RandomStream.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComunitat::CComunitat(uint32 maxOrganismes):
	m_organismes(maxOrganismes),
	m_txmist(Config.get("Comunitat/NumeroTaxons"), 
		Config.get("Comunitat/NumeroMarquesPerTaxo"))
{
	m_maxOrganismes=maxOrganismes;
}

CComunitat::~CComunitat()
{
	
}

uint32 CComunitat::organismeAleatori()
{
	if (m_organismes.nodeBuida()) 
		return NULL;
	while (true) {
		uint32 org=rnd.get(0,m_maxOrganismes);
		if (m_organismes.nodeEnUs(org)) return org;
	}

		
}
