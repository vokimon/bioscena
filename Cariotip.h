// Cariotip.h: interface for the CCariotip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_CARIOTIP_H_INCLUDED)
#define __KKEP_CARIOTIP_H_INCLUDED

#include <vector>
#include "Cromosoma.h"

class CCariotip  
{
public:
// Construccio/Destruccio
public:
	CCariotip();
	virtual ~CCariotip();
// Redefinibles
public:
	virtual void dump(CMissatger & msg);
// Operacions
public:
// Atributs
protected:
	vector<CCromosoma> m_cromosomes;
// Proves
public:
	static void ProvaClasse(void);	
// Implementacio
private:

};

#endif // !defined(__KKEP_CARIOTIP_H_INCLUDED)
