// InfoOrganisme.cpp: implementation of the CInfoOrganisme class.
//
//////////////////////////////////////////////////////////////////////

#include "InfoOrganisme.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfoOrganisme::CInfoOrganisme()
{
	m_cos=NULL;
	m_subIdentificador=0;
	m_posicio=0x00ff;
	m_taxo=0x00ff;
	m_representacio="";
}

CInfoOrganisme::~CInfoOrganisme()
{
}