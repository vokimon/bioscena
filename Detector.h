// Detector.h: interface for the CDetector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DETECTOR_H__FE3CEB81_5664_11D3_A880_2CDF02C10000__INCLUDED_)
#define AFX_DETECTOR_H__FE3CEB81_5664_11D3_A880_2CDF02C10000__INCLUDED_

#include "Substrat.h"

class CDetector  
{
// Operacions
public:
	virtual bool analitza(CSubstrat & s);
	bool operator()(CSubstrat & s) {return analitza(s);};
// Construccio/Destruccio
public:
	CDetector();
	virtual ~CDetector();
};

class CDetectorQuimic
{
// Operacions
public:
	virtual bool analitza(CSubstrat & s) {
		return s.rastreja(m_clau, m_tolerancia);
	};
	void tolerancia(uint32 tol) {m_tolerancia=tol;};
	void clau(uint32 clau) {m_clau=clau;};
// Atributs
public:
	uint32 m_clau;
	uint32 m_tolerancia;
// Proves
public:
};


// TODO: Fer aquest sensor
class CDetectorPresencia
{
// Operacions
public:
	virtual bool analitza(CSubstrat & s);
// Atributs
public:
};


#endif // !defined(AFX_DETECTOR_H__FE3CEB81_5664_11D3_A880_2CDF02C10000__INCLUDED_)
