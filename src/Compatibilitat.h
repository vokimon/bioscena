// Compatibilitat.h: interface for the Compatibilitat templates.
//
//////////////////////////////////////////////////////////////////////
// Change Log: 
// 19990307 VoK - Integrada al projecte en forma de classe
// 19990307 VoK - Convertida en un template
// 19990308 VoK - Enmascarament precalculat 
// 19990308 VoK - Procediments inline
// 19990718 VoK - Es fa servir fstream en comptes de stdio
// 19990725 VoK - A pendre per cul la classe
//////////////////////////////////////////////////////////////////////
// Pendent Log:
//////////////////////////////////////////////////////////////////////

#if !defined(KKEP_COMPATIBILITAT_H_INCLUDED_)
#define KKEP_COMPATIBILITAT_H_INCLUDED_

//#include <time.h>
//#include <stdlib.h>
#include <fstream>
#include "RandomStream.h"

typedef uint32 t_clau;

//////////////////////////////////////////////////////////////////////
// Estatiques
//////////////////////////////////////////////////////////////////////

inline unsigned comptaUns (t_clau n)
{
	int res;
	for (res=0; n; n>>=1) 
		if (n&1) res++;
	return res;
};

inline uint32 nibble(uint32 numero, uint32 valor) 
{
	return (valor >> (numero<<2)) & 0x0000000F;
}


//////////////////////////////////////////////////////////////////////
// Funcio de compatibilitat de claus no 1
//    Tolerancia al numero d'uns
//    Rendiment: un 'rand' i un ComptaUns
//    Funcio: ??
//    Comentari: ??
//////////////////////////////////////////////////////////////////////

inline bool compat1 (t_clau k1, t_clau k2, uint32 tolerancia) {
//	t_clau ruleta = (rand()>>10);
	t_clau ruleta = rnd.get();
	t_clau coincidencia = comptaUns( ~(k1 ^ k2));
//	return (ruleta>>tolerancia)<(coincidencia);
	return ruleta>>(tolerancia&0x7) < (coincidencia<<((tolerancia>>3)&0xf));
	}

//////////////////////////////////////////////////////////////////////
// Funcio de compatibilitat no 2
//    Tolerancia a la posicio dels uns
//    Rendiment: dos 'rand'
//    Funcio: Massa restrictiva i poc color
//    Comentari: Per lo optima que es no esta gens malament.
//////////////////////////////////////////////////////////////////////

inline bool compat2 (t_clau k1, t_clau k2, t_clau tolerancia) {
	t_clau ruleta = rnd.get();
	return (ruleta&(k1^k2)&~tolerancia) == 0;
	}

//////////////////////////////////////////////////////////////////////
// Funcio de compatibilitat 3
//    Tolerancia a la quantitat dels uns
//    Rendiment: un 'ComptaUns' i dos 'rand'
//    Comentari: La coincidencia i la tolerancia no son simetriques
//////////////////////////////////////////////////////////////////////

inline bool compat3 (t_clau k1, t_clau k2, uint32 tolerancia) {
	t_clau ruleta = rnd.get();
	return comptaUns(ruleta&(k1^k2))<tolerancia;
	}

//////////////////////////////////////////////////////////////////////
// Funcio de compatibilitat no 4
//    Tolerancia a la posicio dels uns
//    Rendiment: un 'ComptaUns' i tres 'rand'
//    Comentari: Modifica la 2 per ser menys restrictiva a la zona 
//////////////////////////////////////////////////////////////////////

inline bool compat4 (t_clau k1, t_clau k2, t_clau tolerancia) {
	t_clau ruleta = rnd.get();
	t_clau ruleta2 = rnd.get(0,6);
	return comptaUns(ruleta&(k1^k2)&~tolerancia) <= ruleta2;
	}

//////////////////////////////////////////////////////////////////////
// Em quedo de moment amb la 2
//////////////////////////////////////////////////////////////////////
#define sonCompatibles compat2


//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////
void ProvaCompatibilitat ();



#endif // !defined(KKEP_COMPATIBILITAT_H_INCLUDED_)
