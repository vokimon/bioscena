// Topologia.h: interface for the CTopologia class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_TOPOLOGIA_H_INCLUDED)
#define __KKEP_TOPOLOGIA_H_INCLUDED

#include "BioIncludes.h"

class CTopologia  
{
// Tipus propis
public:
	typedef uint32 t_posicio;
	typedef uint32 t_desplacament;
// Atributs
protected:
	uint32 m_totalCasselles;
// Construccio/Destruccio
public:
	CTopologia(uint32 tamany=0);
	virtual ~CTopologia();

// Redefinibles
public:
	virtual t_posicio desplacament(t_posicio origen, t_desplacament movimentRelatiu) const;
		// Retorna la posicio resultant de fer el desplacament des de l'origen
	virtual t_posicio desplacamentAleatori(t_posicio origen, uint32 radi) const;
		// Retorna la posicio resultant de tants desplacaments aleatoris des de l'origen com indiqui el radi
	virtual bool esValidaCassella(t_posicio cassella) const;
		// Indica si la posicio es valida
	virtual bool unio(t_posicio posOrigen, t_posicio posDesti, t_desplacament & desp) const;
		// Retorna cert si es posible unir-les amb un sol desplacament, a desp hi es
		// el desplacament per unir-les o apropar-les
	virtual t_posicio posicioAleatoria() const;
		// Retorna una posicio qualsevol valida per a la topologia
	virtual uint32 tamany() const;
		// Retorna el nombre de posicions que te la topologia
// Funcions estatiques
public:
// Proves
public:
	static void ProvaClasse(void);
};

#endif // !defined(__KKEP_TOPOLOGIA_H_INCLUDED)
