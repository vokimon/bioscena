// MutacioGenica.h: interface for the CMutacioGenica class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_MUTACIOCCARIOTIPICA_H_INCLUDED)
#define __KKEP_MUTACIOCCARIOTIPICA_H_INCLUDED

#include <string>
#include "Cariotip.h"

class CMutacioCariotip
{
// Construccio destrucció
public:
	CMutacioCariotip();
	virtual ~CMutacioCariotip();
// Redefinibles
public:
	virtual void muta(CCariotip & c)=0;
	virtual string tipus()=0;
// Atributs
protected:
//	uint32 * m_codons;
//	uint32 m_nCodons;
	string m_tipus;
// Funcions estatiques
public:
	static uint32 Nombre(void);
	static CMutacioCariotip * Nova (uint32 n);
	static CMutacioCariotip * Nova (string idTipus);
// Dades estatiques
//	static s_inicialitzador;
// Proves
public:
	static void ProvaClasse (void);
// Implementacio 
protected:
//	void agafaInformacio(CCariotip &c);
//	void fixaInformacio(CCariotip &c);
};

//////////////////////////////////////////////////////////////////////
// Particularitzacions
//////////////////////////////////////////////////////////////////////

class CMutacioPerEscisio: public CMutacioCariotip
{
// Redefinibles
public:
	virtual void muta (CCariotip & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};

class CMutacioPerFusio: public CMutacioCariotip
{
// Redefinibles
public:
	virtual void muta (CCariotip & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};

class CEuploidiaPositiva: public CMutacioCariotip
{
// Redefinibles
public:
	virtual void muta (CCariotip & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};

class CAneuploidiaPositiva: public CMutacioCariotip
{
// Redefinibles
public:
	virtual void muta (CCariotip & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};

class CEuploidiaNegativa: public CMutacioCariotip
{
// Redefinibles
public:
	virtual void muta (CCariotip & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};

class CAneuploidiaNegativa: public CMutacioCariotip
{
// Redefinibles
public:
	virtual void muta (CCariotip & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};

#endif // !defined(__KKEP_MUTACIOCCARIOTIPICA_H_INCLUDED)
