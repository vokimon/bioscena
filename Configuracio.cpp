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
	set("Organisme/Energia/Caducitat",10);
	set("Organisme/Fenotip/Longitud",32);
	set("Organisme/Energia/Inicial",10);
	set("Comunitat/NumeroOrganismes",0x00000200);
	set("Comunitat/TamanyRegeneracio",27);
	set("Comunitat/NumeroTaxons",0x00000200);
	set("Comunitat/NumeroMarquesPerTaxo",030);
	set("Biotop/CassellesAmplitud",80);
	set("Biotop/CassellesAltitud",20);
	set("Biotop/Substrat/MaximInicial",6);
	set("Biotop/Substrat/MolleculesInicials/Numero",4);
	set("Biotop/Substrat/MolleculesInicials/Element",0);
	set("Biotop/Substrat/MolleculesInicials/Tolerancia",3);
}

CConfiguracio::~CConfiguracio()
{

}
