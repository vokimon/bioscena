// Nutrient.cpp: implementation of the CNutrient class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990723 VoK - Creat (Per segona vegada, ets un indecis, david)

#include "Nutrient.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNutrient::CNutrient(uint32 idonietat, uint32 quantitat)
{
	m_idonietat=idonietat;
	m_quantitat=quantitat;
}

CNutrient::CNutrient()
{
	m_idonietat=0;
	m_quantitat=0;
}

CNutrient::~CNutrient()
{

}

CNutrient & CNutrient::operator =(const CNutrient & n)
{
	m_idonietat=n.m_idonietat;
	m_quantitat=n.m_quantitat;
	return *this;
}

bool CNutrient::operator ==(const CNutrient & n)
{
	return m_idonietat==n.m_idonietat;
}
