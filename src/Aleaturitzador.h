// Aleaturitzador.h: interface for the CAleaturitzador class.
//
//////////////////////////////////////////////////////////////////////
// Change Log:
// 19990822 VoK - Creat a partir de l'escisió CTemporitzador que abans
//                incloia tambe aquesta funcionalitat
// 19990822 VoK - Funcions de dump
// 19990823 VoK - Reordenat els fitxers i netejades les proves
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_ALEATURITZADOR_H_INCLUDED)
#define __KKEP_ALEATURITZADOR_H_INCLUDED

#include "RandomStream.h"
#include "MultiAgent.h"

class CAleaturitzador : public CMultiAgent
{
// Tipus Propis
	typedef CAgent t_accio;
//	typedef void (t_accio)(); // Aixo es per fer proves nomes
	struct t_probabilitat_saxona
	{
		bool esDona()
		{
			return rnd.get(0,m_mostra-1)<m_encerts;
		};
		uint32 m_encerts;
		uint32 m_mostra;
	};

// Construccio/Destruccio
public:
	CAleaturitzador();
	virtual ~CAleaturitzador();
// Virtuals redefinibles a les subclasses
public:
	virtual void operator() (void);
	virtual void dump(CMissatger & msg);
	virtual list<CAgent*> subordinats (void);
// Operacions
public:
	void probabilitat(uint32 mostra,uint32 encerts=1);
protected:
	// No fixat al protocol, son aqui per debug
	void reAccio(t_accio * a);
// Atributs
public:
	bool m_accionat;
	t_probabilitat_saxona m_probabilitat;
protected:
	// No fixat al protocol, son aqui per debug
	t_accio * m_reAccio;
// Proves
public:
	static void ProvaClasse();
};


#endif // !defined(__KKEP_ALEATURITZADOR_H_INCLUDED)
