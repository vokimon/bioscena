// MutacioGenica.h: interface for the CMutacioGenica class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_MUTACIOGENICA_H_INCLUDED)
#define __KKEP_MUTACIOGENICA_H_INCLUDED

#include <string>
#include "Cromosoma.h"

class CMutacioGenica 
{
// Construccio destrucció
public:
	CMutacioGenica();
	virtual ~CMutacioGenica();
// Redefinibles
public:
	virtual void muta(CCromosoma & c)=0;
	virtual string tipus()=0;
// Atributs
protected:
	uint32 * m_codons;
	uint32 m_nCodons;
	string m_tipus;
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
// Implementacio 
protected:
	void agafaInformacio(CCromosoma &c);
	void fixaInformacio(CCromosoma &c);
};

//////////////////////////////////////////////////////////////////////
// Particularitzacions
//////////////////////////////////////////////////////////////////////

class CMutacioPuntualBinariaGaussiana: public CMutacioGenica
{
// Redefinibles
public:
	virtual void muta (CCromosoma & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};

class CMutacioPuntualBinaria : public CMutacioGenica
{
// Redefinibles
public:
	virtual void muta (CCromosoma & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};

class CMutacioPuntualDrastica : public CMutacioGenica
{
// Redefinibles
public:
	virtual void muta (CCromosoma & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};

class CMutacioDesplacament : public CMutacioGenica
{
// Redefinibles
public:
	virtual void muta (CCromosoma & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};

class CMutacioInversio : public CMutacioGenica
{
// Redefinibles
public:
	virtual void muta (CCromosoma & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};

class CMutacioInsercioReplicada : public CMutacioGenica
{
// Redefinibles
public:
	virtual void muta (CCromosoma & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};

class CMutacioInsercioAleatoria : public CMutacioGenica
{
// Redefinibles
public:
	virtual void muta (CCromosoma & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};

class CMutacioDeleccio : public CMutacioGenica
{
// Redefinibles
public:
	virtual void muta (CCromosoma & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};


#endif // !defined(__KKEP_MUTACIOGENICA_H_INCLUDED)
