// MutacioGenica.h: interface for the CMutacioGenica class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_MUTACIOGENICA_H_INCLUDED)
#define __KKEP_MUTACIOGENICA_H_INCLUDED

#include <string>
#include "MutacioCromosomica.h"

class CMutacioGenica : public CMutacioCromosomica
{
// Tipus propis
public:
	typedef CCromosoma::t_codo t_codo;
// Construccio destrucció
public:
	CMutacioGenica();
	virtual ~CMutacioGenica();
// Redefinibles
public:
	virtual bool muta(CCromosoma & c);
	virtual bool muta(t_codo & c)=0;
	virtual string tipus()=0;
// Atributs
protected:
//	string m_tipus;
// Funcions estatiques
public:
	static uint32 Nombre(void);
	static CMutacioGenica * Nova (uint32 n);
	static CMutacioGenica * Nova (string idTipus);
// Dades estatiques
//	static s_inicialitzador;
// Proves
public:
	static void ProvaClasse (void);
};

//////////////////////////////////////////////////////////////////////
// Particularitzacions
//////////////////////////////////////////////////////////////////////

class CMutacioPuntualBinariaGaussiana: public CMutacioGenica
{
// Redefinibles
public:
	virtual bool muta (t_codo & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};

class CMutacioPuntualBinaria : public CMutacioGenica
{
// Redefinibles
public:
	virtual bool muta (t_codo & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};

class CMutacioPuntualDrastica : public CMutacioGenica
{
// Redefinibles
public:
	virtual bool muta (t_codo & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};


#endif // !defined(__KKEP_MUTACIOGENICA_H_INCLUDED)
