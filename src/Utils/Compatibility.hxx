// Compatibilitat.h: interface for the Compatibilitat templates.
//
//////////////////////////////////////////////////////////////////////
// TODO:
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_COMPATIBILITAT_H_INCLUDED)
#define __KKEP_COMPATIBILITAT_H_INCLUDED

//#include <time.h>
//#include <stdlib.h>
#include "RandomStream.h"

namespace Bioscena {

typedef uint32 t_clau;

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

/**
* Compatibility keys are used to know how two numbers correspond each
* other.
* The probability for one key to match another is proportional to the 
* binary coincidence and should be 
* 
*/

class Key {
	public:
		bool isCompatibleWith(Key otherKey, t_tolerance tolerance) {
			return compat<selectedAlgoritm>(m_key, otherKey.m_key, tolerance);
		}
		static bool areCompatibles(Key aKey, Key otherKey, t_tolerance tolerance) {
			return compat<selectedAlgoritm>(aKey.m_key, otherKey.m_key, tolerance);
		}
	private:
		typedef uint32 t_tolerance;

	private:
		enum {selectedAlgoritm=2};
		uint32 m_key;
		
		template <unsigned algoritm=selectedAlgoritm>
		static bool compat(t_clau k1, t_clau k2, uint32 tolerancia);

		///////////////////////////////////////////////////////////////////////
		/// Funcio de compatibilitat de claus no 1
		///    Tolerancia al numero d'uns
		///    Rendiment: un 'rand' i un ComptaUns
		///    Funcio: ??
		///    Comentari: ??
		///////////////////////////////////////////////////////////////////////
		template <>
		static bool compat<1> (t_clau k1, t_clau k2, uint32 tolerancia) {
		//	t_clau ruleta = (rand()>>10);
			t_clau ruleta = rnd.get();
			t_clau coincidencia = comptaUns( ~(k1 ^ k2));
		//	return (ruleta>>tolerancia)<(coincidencia);
			return ruleta>>(tolerancia&0x7) < (coincidencia<<((tolerancia>>3)&0xf));
			}

		//////////////////////////////////////////////////////////////////////
		/// Funcio de compatibilitat no 2
		///    Tolerancia a la posicio dels uns
		///    Rendiment: dos 'rand'
		///    Funcio: Massa restrictiva i poc color
		///    Comentari: Per lo optima que es no esta gens malament.
		//////////////////////////////////////////////////////////////////////
		template <>
		static bool compat<2> (t_clau k1, t_clau k2, t_clau tolerancia) {
			t_clau ruleta = rnd.get();
			return (ruleta&(k1^k2)&~tolerancia) == 0;
			}

		///////////////////////////////////////////////////////////////////////
		/// Funcio de compatibilitat 3
		///    Tolerancia a la quantitat dels uns
		///    Rendiment: un 'ComptaUns' i dos 'rand'
		///    Comentari: La coincidencia i la tolerancia no son simetriques
		///////////////////////////////////////////////////////////////////////
		template <>
		static bool compat<3> (t_clau k1, t_clau k2, uint32 tolerancia) {
			t_clau ruleta = rnd.get();
			return comptaUns(ruleta&(k1^k2))<tolerancia;
			}

		///////////////////////////////////////////////////////////////////////
		/// Funcio de compatibilitat no 4
		///    Tolerancia a la posicio dels uns
		///    Rendiment: un 'ComptaUns' i tres 'rand'
		///    Comentari: Modifica la 2 per ser menys restrictiva a la zona 
		///////////////////////////////////////////////////////////////////////
		template <>
		static bool compat<4> (t_clau k1, t_clau k2, t_clau tolerancia) {
			t_clau ruleta = rnd.get();
			t_clau ruleta2 = rnd.get(0,6);
			return comptaUns(ruleta&(k1^k2)&~tolerancia) <= ruleta2;
			}

};


}

#endif // !defined(__KKEP_COMPATIBILITAT_H_INCLUDED)
