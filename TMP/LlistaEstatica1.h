// LlistaEstatica.h: interface for the CLlistaEstatica class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_LLISTAESTATICA_H_INCLUDED)
#define __KKEP_LLISTAESTATICA_H_INCLUDED

#include "BioIncludes.h"

template <class T> class CLlistaEstatica
{
	typedef T t_element;
	typedef uint32 t_index;
	typedef CLlistaEstatica<T> t_propi;
	typedef CLlistaEstatica<T> t_llista;

public:
	class t_node
	{
	friend class CLlistaEstatica<T>;
	protected:
		uint32 seguent;
		uint32 anterior;
	};

	class iterador {
		typedef iterador self;

		CLlistaEstatica<T> & m_llista;
		t_index m_index;
	public:
		iterador(t_llista & l, int index) : m_llista(l) 
		{
			m_index=index;
		}
		iterador & operator++ ()
		{
			m_index=m_llista.nodeSeguent(m_index);
			return *this;
		}
		iterador & operator-- ()
		{
			m_index=m_llista.nodeAnterior(m_index);
			return *this;
		}
		iterador operator++ (int)
		{
			iterador tmp(m_llista, m_index);
			m_index=m_llista.nodeSeguent(m_index);
			return tmp;
		}
		iterador operator-- (int)
		{
			iterador tmp(m_llista, m_index);
			m_index=m_llista.nodeAnterior(m_index);
			return tmp;
		}
		T & operator* ()
		{
			return m_llista[m_index];
		}
	};
// Implementacio interna de la llista de nodes
protected:
	t_element *valors;
	t_node *nodes;
	uint32 nodeLliure;
	uint32 nodeFantasma;
	uint32 maxNodes;
	uint32 m_tamany;
// Construccio/Destruccio
public:
	inline CLlistaEstatica (uint32 nNodes);
	inline virtual ~CLlistaEstatica();
protected:
	inline virtual T* creaNodes(uint32 nNodes);
// Testeig
public:
	inline uint32 tamany() const;
	inline bool nodeHiHaLliures () const;
	inline bool nodeBuida () const;
	inline bool nodeHiHaEnUs () const;
	inline bool nodeEnUs (uint32 node) const;
// Ocupacio i alliberament
public:
	inline uint32 nodeOcupa (void);
	inline uint32 nodeOcupaDespresDe(uint32 pos);
	inline void nodeAllibera (uint32 node);
	inline void nodeDesplacaGrup(uint32 desti, uint32 iniciOr, uint32 finalOr);
// Iteradors cap endavant
public:
	inline uint32 nodePrimer (void) const;
	inline uint32 nodeSeguent (uint32 i) const;
	inline bool nodeFinal (uint32 i) const;
// Iteradors cap enrera
public:
	inline uint32 nodeUltim (void) const;
	inline uint32 nodeAnterior (uint32 i) const;
	inline bool nodeInici (uint32 i) const;
// Access
public:
	inline CLlistaEstatica<T> &insereixAlInici(T& element);
	inline t_element &operator [] (uint32 index) {return valors[index];};
// Proves
public:
	static void ProvaClasse(void);
	void dump (ostream &stream) const;
	bool invariant(void) const;
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

template <class T> CLlistaEstatica<T>::CLlistaEstatica(uint32 nNodes)
	// Pre: nIndividus>
{
	m_tamany=0;
	maxNodes = nNodes;
	nodeFantasma = maxNodes;
	nodes = new t_node[nNodes+1];
	valors = creaNodes(nNodes); // Fem servir una funcio
	// !!! Errors de memoria. !!!
	// Inicialitzem el fantasma de la llista de nodes en us.
	// NodeFantasma indica enllac a no pas enlloc
	nodes[nodeFantasma].seguent = nodeFantasma;
	nodes[nodeFantasma].anterior = nodeFantasma;
	// Encadenem els nodes lliures (tots menys el fantasma)
	nodeLliure=0;
	for (uint32 i=nNodes;i--;)
	{
		nodes[i].seguent=i+1;
		nodes[i].anterior=nodeFantasma; 
		// Per posar-ho a algo, no cal
	}
	nodes[nNodes].seguent = nodeFantasma;
	nodes[nNodes].anterior = nodeFantasma;
}

template <class T> T * CLlistaEstatica<T>::creaNodes(uint32 nNodes)
{
	return new T[nNodes];
}


template <class T> CLlistaEstatica<T>::~CLlistaEstatica()
{
	delete [] nodes;
	delete [] valors;
}

//////////////////////////////////////////////////////////////////////
// Testeig
//////////////////////////////////////////////////////////////////////

template <class T> uint32 CLlistaEstatica<T>::tamany() const {return m_tamany;}

template <class T> bool CLlistaEstatica<T>::nodeEnUs (uint32 node) const
{
	for (uint32 i=nodes[nodeFantasma].seguent; i!=nodeFantasma; i=nodes[i].seguent) 
		if (node==i) return true;
	return false;
}

template <class T> bool CLlistaEstatica<T>::nodeHiHaLliures () const
{
	return nodeLliure != nodeFantasma;
}

template <class T> bool CLlistaEstatica<T>::nodeHiHaEnUs () const
{
	return nodes[nodeFantasma].seguent != nodeFantasma;
}

template <class T> bool CLlistaEstatica<T>::nodeBuida () const
{
	return nodes[nodeFantasma].seguent == nodeFantasma;
}

//////////////////////////////////////////////////////////////////////
// Ocupacio i alliberament de nodes
//////////////////////////////////////////////////////////////////////

template <class T> uint32 CLlistaEstatica<T>::nodeOcupa (void) 
	// Pre: Hi ha nodes lliures.
{
	// Un assert per assegurar la Pre: en debug
	KKEPAssert(nodeHiHaLliures (),
		"LlistaEstatica: Provant d'ocupar nodes sense tenir disponibles");
	// Ho extreiem dels nodes lliures
	// no cal mantenir l'anterior.
	uint32 node=nodeLliure;
	nodeLliure = nodes[node].seguent;
	// I ara ho insertem als nodes en us 
	// despres del fantasma i tenint en compte 
	// els anteriors
	nodes[node].seguent = nodes[nodeFantasma].seguent;
	nodes[node].anterior = nodeFantasma;
	nodes[nodes[node].seguent].anterior = node;
	nodes[nodeFantasma].seguent = node;
	m_tamany++;
	return node;
}
template <class T> uint32 CLlistaEstatica<T>::nodeOcupaDespresDe (uint32 pos) 
	// Pre: Hi ha nodes lliures.
	// Pre: Pos es un node en us o el fantasma
{
	// Un assert per assegurar la Pre: en debug
	KKEPAssert(nodeHiHaLliures (),
		"LlistaEstatica: Provant d'ocupar nodes sense tenir disponibles");
	KKEPAssert(pos==nodeFantasma||nodeEnUs(pos),
		"LlistaEstatica: Node invalid per inserir darrera un de nou");
	// Ho extreiem dels nodes lliures
	// no cal mantenir l'anterior.
	uint32 node=nodeLliure;
	nodeLliure = nodes[node].seguent;
	// I ara ho insertem als nodes en us 
	// despres del fantasma i tenint en compte 
	// els anteriors
	nodes[node].seguent = nodes[pos].seguent;
	nodes[node].anterior = pos;
	nodes[nodes[node].seguent].anterior = node;
	nodes[pos].seguent = node;
	m_tamany++;
	return node;
}
template <class T> void CLlistaEstatica<T>::nodeAllibera (uint32 node) 
	// Pre: El node es un node en us
{
	// Un assert per assegurar la Pre: en debug
	KKEPAssert(nodeEnUs(node),
		"Alliberant un node de LlistaEstatica ja alliberat");
	// Restablim els vincles amb el node alliberat.
	nodes[nodes[node].anterior].seguent = 
		nodes[node].seguent;
	nodes[nodes[node].seguent].anterior = 
		nodes[node].anterior;
	// Ho insertem a la llista de proscrits.
	nodes[node].seguent=nodeLliure;		
	nodeLliure = node;
	m_tamany--;
}

template <class T> void CLlistaEstatica<T>::nodeDesplacaGrup(uint32 desti, uint32 iniciOr, uint32 finalOr)
// Pre: desti no esta inclos dintre de l'interval d'origen
{
	// Separem el grup de nodes en conflicte
	nodes[nodes[iniciOr].anterior].seguent = 
		nodes[finalOr].seguent;
	nodes[nodes[finalOr].seguent].anterior = 
		nodes[iniciOr].anterior;
	// Els peguem darrera del desti.
	nodes[finalOr].seguent=nodes[desti].seguent;
	nodes[iniciOr].anterior=desti;
	nodes[nodes[desti].seguent].anterior=finalOr;
	nodes[desti].seguent = iniciOr;
}

//////////////////////////////////////////////////////////////////////
// Iteradors cap endavant
//////////////////////////////////////////////////////////////////////

template <class T> uint32 CLlistaEstatica<T>::nodePrimer() const
{
	return nodes[nodeFantasma].seguent;
}

template <class T> uint32 CLlistaEstatica<T>::nodeSeguent(uint32 i) const
{
	return nodes[i].seguent;
}

template <class T> bool CLlistaEstatica<T>::nodeFinal(uint32 i) const
{
	return i==nodeFantasma;
}

//////////////////////////////////////////////////////////////////////
// Iteradors cap enrera
//////////////////////////////////////////////////////////////////////

template <class T> uint32 CLlistaEstatica<T>::nodeUltim(void) const
{
	return nodes[nodeFantasma].anterior;
}

template <class T> uint32 CLlistaEstatica<T>::nodeAnterior(uint32 i) const
{
	return nodes[i].anterior;
}

template <class T> bool CLlistaEstatica<T>::nodeInici(uint32 i) const
{
	return i==nodeFantasma;
}

//////////////////////////////////////////////////////////////////////
// Debuging
//////////////////////////////////////////////////////////////////////

template <class T> CLlistaEstatica<T> &CLlistaEstatica<T>::insereixAlInici(T& element)
{
	uint32 nouNode = nodeOcupa();
	valors[nouNode] = element;
	return *this;
}

//////////////////////////////////////////////////////////////////////
// Debuging
//////////////////////////////////////////////////////////////////////

template <class T> bool CLlistaEstatica<T>::invariant(void) const
{
	// !!! Aquest invariant necessita un parell de repasades.
	uint32 i=maxNodes+1; // Numero de nodes enllacats
	uint32 node;
	for (
		node = nodeLliure; 
		node!=nodeFantasma && i; 
		node=nodes[node].seguent,i--);
	if (node!=nodeFantasma)
	// Hi han bucles als nodes lliures o el 
	// valor amb el maxim d'individus s'ha corromput.
		return false;
	for (node = nodeFantasma; i; node=nodes[node].seguent, i--)
		if (nodes[nodes[node].seguent].anterior!=node)
		// Nodes anteriors i seguents no es corresponen
			return false;
	if (node!=nodeFantasma)
	// Hi han bucles als nodes en us o el valor amb el 
	// maxim d'individus s'ha corromput
		return false;
	return true;
}

template <class T> void CLlistaEstatica<T>::dump(ostream &stream) const
{
	stream << "LlistaEstatica: Enllac dels nodes"<< endl;
	stream << "Node fantasma: " << nodeFantasma << endl;
	stream << "Node lliure: " << nodeLliure << endl;
	stream << "Numero de nodes: " << maxNodes << " + el fantasma" << endl;
	for (uint32 i= 0; i<=maxNodes; i++) 
	{
		stream << "Node: " << i << "   "
			<< nodes[i].anterior << " <- " << i
			<< " -> " << nodes[i].seguent
			<< endl;
	}
}

template <class T> void CLlistaEstatica<T>::ProvaClasse(void)
	// Metode de proves per a la classe
{
	CLlistaEstatica<T> com(5);
	cout << "El invariant " << (com.invariant()?"SI ":"NO ") << "es compleix. " << endl;
	com.nodeOcupa();
	com.nodeOcupa();
	com.nodeOcupa();
	com.nodeOcupa();
	com.nodeOcupa();
	com.dump(cout);
	cout << "El invariant " << (com.invariant()?"SI ":"NO ") << "es compleix. " << endl;
	com.nodeAllibera(4);
	com.nodeAllibera(3);
	com.nodeAllibera(2);
	com.nodeAllibera(1);
	com.nodeAllibera(0);
	com.dump(cout);
	cout << "El invariant " << (com.invariant()?"SI ":"NO ") << "es compleix. " << endl;
}

#endif // !defined(__KKEP_LLISTAESTATICA_H_INCLUDED)
