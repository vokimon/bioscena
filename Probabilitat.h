// Probabilitat.h: interface for the CProbabilitat class.
//
//////////////////////////////////////////////////////////////////////
// 19991018 VoK - Creat
// 19991116 VoK - Afegits els accessors de lectura encerts() i mostra()
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_PROBABILITAT_H_INCLUDED)
#define __KKEP_PROBABILITAT_H_INCLUDED

#include "BioIncludes.h"
#include "RandomStream.h"

class CProbabilitat 
{
// Construccio/Destruccio
public:
	CProbabilitat();
	virtual ~CProbabilitat();
// Operacions
public:
	bool esDona()
	{
		return m_encerts >= rnd.get(1,m_mostra);
	};
	void fixa(uint32 mostra, uint32 encerts)
	{
		// La mostra no pot ser 0 (probabilitat infinita??)
		m_mostra = mostra?mostra:1;
		// Els encerts han de ser inferiors o iguals a la mostra
		// (probabilitat fora de linterval [0,1])
		m_encerts = mostra<encerts?mostra:encerts;
	}
	uint32 encerts()
	{
		return m_encerts;
	}
	uint32 mostra()
	{
		return m_mostra;
	}
// Atributs
private:
	uint32 m_encerts;
	uint32 m_mostra;
// Funcions estatiques
public:
	static bool EsDona(uint32 encerts, uint32 mostra)
	{
		return encerts > rnd.get(0,mostra-1);
	}
};

#endif // !defined(__KKEP_PROBABILITAT_H_INCLUDED)
