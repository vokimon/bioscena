// MutacioCromosomica.h: interface for the CMutacioCromosomica class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_MUTACIOCROMOSOMICA_H_INCLUDED)
#define __KKEP_MUTACIOCROMOSOMICA_H_INCLUDED

#include <string>
#include "MutacioCariotip.h"

class CMutacioCromosomica: public CMutacioCariotip
{
// Construccio destrucció
public:
	CMutacioCromosomica();
	virtual ~CMutacioCromosomica();
// Redefinibles
public:
	virtual void muta(CCariotip & c);
	virtual void muta(CCromosoma & c)=0;
	virtual string tipus()=0;
// Atributs
protected:
	uint32 * m_codons;
	uint32 m_nCodons;
//	string m_tipus;
// Funcions estatiques
public:
	static uint32 Nombre(void);
	static CMutacioCromosomica * Nova (uint32 n);
	static CMutacioCromosomica * Nova (string idTipus);
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

class CMutacioDesplacament : public CMutacioCromosomica
{
// Redefinibles
public:
	virtual void muta (CCromosoma & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};

class CMutacioInversio : public CMutacioCromosomica
{
// Redefinibles
public:
	virtual void muta (CCromosoma & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};

class CMutacioInsercioReplicada : public CMutacioCromosomica
{
// Redefinibles
public:
	virtual void muta (CCromosoma & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};

class CMutacioInsercioAleatoria : public CMutacioCromosomica
{
// Redefinibles
public:
	virtual void muta (CCromosoma & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};

class CMutacioDeleccio : public CMutacioCromosomica
{
// Redefinibles
public:
	virtual void muta (CCromosoma & c);
	virtual string tipus();
// Funcions estatiques
public:
	static string Tipus();
};


#endif // !defined(__KKEP_MUTACIOCROMOSOMICA_H_INCLUDED)
