// Configuracio.cpp: implementation of the CConfiguracio class.
//
//////////////////////////////////////////////////////////////////////

#include "Configuracio.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CConfiguracio Config;

CConfiguracio::CConfiguracio()
{
	set("Organisme/NumeroRegistres",32);
	set("Comunitat/NumeroOrganismes",0x00000200);
	set("Comunitat/NumeroTaxons",0x00000200);
	set("Comunitat/NumeroMarquesPerTaxo",20);
	set("Biotop/CassellesAmplitud",80);
	set("Biotop/CassellesAltitud",20);
}

CConfiguracio::~CConfiguracio()
{

}
