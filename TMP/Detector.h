// Detector.h: interface for the CDetector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_DETECTOR_H_INCLUDED)
#define __KKEP_DETECTOR_H_INCLUDED

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
// Metodes estatics
public:
	static CDetector * NouDetector(uint32 tipus);
};

class CDetectorQuimic: public CDetector
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
class CDetectorPresencia: public CDetector
{
// Operacions
public:
	virtual bool analitza(CSubstrat & s) {return true;};
// Atributs
public:
	uint32 m_clau;
	uint32 m_tolerancia;
	uint32 m_registre;
};


#endif // !defined(__KKEP_DETECTOR_H_INCLUDED)
