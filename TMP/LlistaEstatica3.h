// LlistaEstatica.h: interface for the CLlistaEstatica class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_LLISTAESTATICA_H_INCLUDED)
#define __KKEP_LLISTAESTATICA_H_INCLUDED

#include <iterator>
#include <vector>
#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include "BioIncludes.h"

template <class Cont>
class _kkep_index_iterator {
public:
	typedef _kkep_index_iterator t_self;

	typedef Cont t_container;
	typedef bidirectional_iterator_tag iterator_category;
	typedef typename t_container::value_type value_type;
//	typedef typename t_container::pointer pointer;
	typedef typename t_container::reference reference;
//	typedef size_t size_type;
//	typedef ptrdiff_t difference_type;
	typedef typename t_container::t_index t_index;

protected:
	t_container & m_container;
	t_index m_index;

public:
	_kkep_index_iterator(t_self & it)
		: m_container(it.m_container)
	{
		m_index=it.m_index;
	}
	_kkep_index_iterator(t_container * const container, int index) 
		: m_container(*container)
	{
		m_index=index;
	}
	_kkep_index_iterator(t_container & container, int index) 
		: m_container(container)
	{
		m_index=index;
	}
/*	t_self & operator= (_kkep_index_iterator & it)
	{
		m_index=it.m_index;
		m_container=it.m_container;
	}
*/	bool operator== (const _kkep_index_iterator & it) const
	{
		return *m_container==*it.m_container &&
			m_index==it.m_index;
	}
	operator int () const
	{
		return m_index;
	}
	t_self & operator++ ()
	{
		m_index=m_container.nodes[m_index].seguent;
		return *this;
	}
	t_self & operator-- ()
	{
		m_index=m_container.nodes[m_index].anterior;
		return *this;
	}
	t_self operator++ (int)
	{
		t_self tmp(m_container, m_index);
		m_index=m_container.nodes[m_index].seguent;
		return tmp;
	}
	t_self operator-- (int)
	{
		t_self tmp(m_container, m_index);
		m_index=m_container.nodes[m_index].anterior;
		return tmp;
	}
	reference operator* ()
	{
			return m_container[m_index];
	}
};


template <class T> 
class CLlistaEstatica
{
public:
	typedef CLlistaEstatica<T> t_self;
//	typedef CLlistaEstatica<T> t_llista;
	typedef T t_element;
public:
	typedef typename vector<T>::allocator_type allocator_type;
	typedef typename allocator_type::pointer pointer;
	typedef typename vector<T>::size_type size_type;
	typedef typename vector<T>::difference_type difference_type;
	typedef typename vector<T>::reference reference;
	typedef typename vector<T>::const_reference const_reference;
	typedef typename vector<T>::value_type value_type;

	typedef uint32 t_index;
	typedef _kkep_index_iterator<t_self> iterator;

public:
	friend iterator;
	typedef reverse_bidirectional_iterator
		<iterator, value_type, reference, /*_Tptr,*/ difference_type> 
			reverse_iterator;
//	class const_iterator;
//	friend class const_iterator;
//	typedef reverse_bidirectional_iterator
//		<const_iterator, value_type, const_reference, /*_Ctptr,*/ difference_type>
//			const_reverse_iterator;
// Implementacio interna de la llista de nodes
protected:
	list<t_index> sequencia;
	list<t_index> lliures;
	vector<t_element> valors;
//	uint32 nodeLliure;
//	uint32 nodeFantasma;
	uint32 maxNodes;
	uint32 m_tamany;
// Construccio/Destruccio
public:
	inline CLlistaEstatica ();
	inline virtual ~CLlistaEstatica();
// Testeig
public:
	inline uint32 size() const;
	inline uint32 capacity() const;
	inline bool empty () const;
	inline bool nodeHiHaLliures () const;
	inline bool nodeHiHaEnUs () const;
	inline bool nodeEnUs (uint32 node) const;
// Ocupacio i alliberament
public:
	inline void afegeixLliures (void);
	inline t_index nodeOcupa (void);
	inline void nodeAllibera (t_index node);
	inline void nodeDesplacaGrup(t_index desti, t_index iniciOr, t_index finalOr);
	inline void nodeInsertaAbans (t_index node, t_index desti);
	inline void nodeInsertaDespres (t_index node, t_index desti);
	inline void nodeExtreu (t_index node);
// Iteradors
public:
	inline iterator begin (void);
	inline iterator end (void);
	inline reverse_iterator rbegin (void) const;
	inline reverse_iterator rend (void) const;
// Access
public:
	inline T& back() const;
	inline T& front() const;
	inline void push_back (const T& element);
	inline void push_front (const T& element);
	inline void pop_back ();
	inline void pop_front ();
	inline t_element & operator [] (uint32 index) {return valors[index];};
	iterator insert(iterator pred, T & element);
	void insert(iterator it, size_type n, const T& x);
//	void insert(iterator it, const_iterator first, const_iterator last);
// Proves
public:
	static void ProvaClasse(void);
	void dump (ostream &stream);// const;
	bool invariant(void) const;
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

template <class T>
CLlistaEstatica<T>::CLlistaEstatica()
	: nodes(2), valors(2)
	// Pre: nIndividus>
{
	m_tamany=0;
	maxNodes = 1;
	// Inicialitzem el fantasma de la llista de nodes en us.
	// NodeFantasma indica enllac a no pas enlloc
	nodeFantasma = 0;
	nodes[nodeFantasma].seguent = nodeFantasma;
	nodes[nodeFantasma].anterior = nodeFantasma;
	// Encadenem els nodes lliures (tots menys el fantasma)
	nodeLliure=1;
	nodes[nodeLliure].seguent = nodeFantasma;
	nodes[nodeLliure].anterior = nodeFantasma;
}

template <class T> 
CLlistaEstatica<T>::~CLlistaEstatica()
{
}

//////////////////////////////////////////////////////////////////////
// Acces
//////////////////////////////////////////////////////////////////////

template <class T> 
CLlistaEstatica<T>::iterator CLlistaEstatica<T>::insert(iterator pred, T & element)
	// Pre: pred es un node en us.
{
	KKEPAssert(nodeEnUs(pred),
		"Inserint darrera dùn node no valid");
	t_index node = nodeOcupa();
	nodeInsertaDespres(pred);
	valors[node] = element;
	return iterator(node, *this);
}

template <class T> 
void CLlistaEstatica<T>::insert(iterator it, size_type n, const T& x)
{
	KKEPAssert(nodeEnUs(pred),
		"Inserint darrera dùn node no valid");
	t_index node = nodeOcupa();
	nodeInsertaDespres(pred);
	valors[node] = element;
	return iterator(node, *this);
}

/*
template <class T> 
void CLlistaEstatica<T>::insert(iterator it, const_iterator first, const_iterator last)
{
}
*/

template <class T> 
T& CLlistaEstatica<T>::back() const
{
	return valors[nodes[nodeFantasma].anterior];
}

template <class T> 
T& CLlistaEstatica<T>::front() const
{
	return valors[nodes[nodeFantasma].seguent];
}

template <class T> 
void CLlistaEstatica<T>::push_back(const T & element)
{
	t_index node = nodeOcupa();
	nodeInsertaAbans(node, nodeFantasma);
	valors[node] = element;
}

template <class T> 
void CLlistaEstatica<T>::push_front(const T & element)
{
	t_index node = nodeOcupa();
	nodeInsertaDespres(node, nodeFantasma);
	valors[node] = element;
}

template <class T> 
void CLlistaEstatica<T>::pop_back()
{
	KKEPAssert(nodeHiHaEnUs(),
		"Eliminant sense elements en us");
	t_index node = nodes[nodeFantasma].anterior;
	nodeExtreu(node);
	nodeAllibera(node);
}

template <class T> 
void CLlistaEstatica<T>::pop_front()
{
	KKEPAssert(nodeHiHaEnUs(),
		"Eliminant sense elements en us");
	t_index node = nodes[nodeFantasma].seguent;
	nodeExtreu(node);
	nodeAllibera(node);
}


//////////////////////////////////////////////////////////////////////
// Testeig
//////////////////////////////////////////////////////////////////////

template <class T> 
uint32 CLlistaEstatica<T>::size() const {return sequencia.size();}

template <class T> 
uint32 CLlistaEstatica<T>::capacity() const {return valors.size();}

template <class T> 
bool CLlistaEstatica<T>::nodeEnUs (uint32 node) const
{
//	for (uint32 i=nodes[nodeFantasma].seguent; i!=nodeFantasma; i=nodes[i].seguent) 
//		if (node==i) return true;
	return false;
}

template <class T> 
bool CLlistaEstatica<T>::nodeHiHaLliures () const
{
	return !lliures.empty();
}

template <class T> 
bool CLlistaEstatica<T>::nodeHiHaEnUs () const
{
	return !sequencia.empty();
}

template <class T> 
bool CLlistaEstatica<T>::empty () const
{
	return sequencia.empty();
}

//////////////////////////////////////////////////////////////////////
// Ocupacio i alliberament de nodes
//////////////////////////////////////////////////////////////////////

template <class T> 
void CLlistaEstatica<T>::afegeixLliures (size_t n) 
{
	// Ens guardem el numero de valors actuals
	uint32 nNodesInicialitzats = capacity();
	// Inserim els espais nous pels valors
	valors.insert(valors.end(), n, value_type());
	// Inserim numeros de node lliures
	list<t_index>::iterator it = lliures.end();
	// Sense el zero es pensa que es un altre constructor
	lliures.insert(it,n,0);
	for (; n; ++it,--n) 
		*it = nNodesInicialitzats++;
//	maxNodes = nNodesInicialitzats;
}

template <class T> 
uint32 CLlistaEstatica<T>::nodeOcupa (void) 
{
	// Assegurar-nos que hi ha nodes lliures
	if (!nodeHiHaLliures())
		afegeixLliures();
	// Ho extreiem dels nodes lliures
	// no cal mantenir l'anterior.
	uint32 node = lliures.back();
	lliures.pop_back();
	m_tamany++;
	return node;
}

template <class T> 
void CLlistaEstatica<T>::nodeAllibera (uint32 node) 
	// Pre: El node es un node en us
{
	// Ho insertem a la llista de proscrits.
	lliures.push_back(node);
	m_tamany--;
}

template <class T> 
void CLlistaEstatica<T>::nodeInsertaAbans (t_index node, t_index desti) 
{
	nodes[node].anterior = nodes[desti].anterior;
	nodes[node].seguent = desti;
	nodes[nodes[node].anterior].seguent = node;
	nodes[desti].anterior = node;
}

template <class T> 
void CLlistaEstatica<T>::nodeInsertaDespres (t_index node, t_index desti) 
{
	nodes[node].seguent = nodes[desti].seguent;
	nodes[node].anterior = desti;
	nodes[nodes[node].seguent].anterior = node;
	nodes[desti].seguent = node;
}

template <class T> 
void CLlistaEstatica<T>::nodeExtreu (t_index node) 
{
	// Un assert per assegurar la Pre: en debug
	KKEPAssert(nodeEnUs(node),
		"Alliberant un node de LlistaEstatica ja alliberat");
	// Restablim els vincles amb el node alliberat.
	nodes[nodes[node].anterior].seguent = nodes[node].seguent;
	nodes[nodes[node].seguent].anterior = nodes[node].anterior;
}

template <class T> 
void CLlistaEstatica<T>::nodeDesplacaGrup(uint32 desti, uint32 iniciOr, uint32 finalOr)
// Pre: desti no esta inclos dintre de l'interval d'origen
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
// Iteradors
//////////////////////////////////////////////////////////////////////

template <class T> 
CLlistaEstatica<T>::iterator CLlistaEstatica<T>::begin(void)
{
	iterator it(this, nodes[nodeFantasma].seguent);
	return it;
}

template <class T> 
CLlistaEstatica<T>::iterator CLlistaEstatica<T>::end(void)
{
	iterator it(this, nodeFantasma);
	return it;
}

template <class T> 
CLlistaEstatica<T>::reverse_iterator CLlistaEstatica<T>::rbegin(void) const
{
	return reverse_iterator(*this, nodes[nodeFantasma].anterior);
}

template <class T> 
CLlistaEstatica<T>::reverse_iterator CLlistaEstatica<T>::rend(void) const
{
	return reverse_iterator(*this, nodeFantasma);
}

//////////////////////////////////////////////////////////////////////
// Debuging
//////////////////////////////////////////////////////////////////////

template <class T> bool CLlistaEstatica<T>::invariant(void) const
{
	// !!! Aquest invariant necessita un parell de repasades.
	uint32 i=maxNodes+1; // Numero de nodes enllacats
	uint32 node;
	for (node = nodeLliure; node!=nodeFantasma && i; node=nodes[node].seguent,i--)
		if (nodeEnUs(node)) {
			cout << "El node " << node << "esta a la pila de lliures i esta en us" << endl;
			return false;
		}
	if (node!=nodeFantasma){
		cout << "Hi han bucles als nodes lliures o el maxim s'ha corromput." << endl;
		return false;
	}
	for (node = nodeFantasma; i; node=nodes[node].seguent, i--)
		if (nodes[nodes[node].seguent].anterior!=node) {
			cout << "Nodes anteriors i seguents de " 
				<< node << " i "<< nodes[node].seguent 
				<< " no es corresponen" << endl;
			return false;
		}
	if (node!=nodeFantasma) {
		cout << "Hi han bucles als nodes en us o el maxim s'ha corromput" << endl;
		return false;
	}
	return true;
}

template <class T> void CLlistaEstatica<T>::dump(ostream &stream)
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
	iterator it(begin());
	for (; !(it==end()); it++);
		stream << *it;
}

template <class T> void CLlistaEstatica<T>::ProvaClasse(void)
	// Metode de proves per a la classe
{
	int r=10;
	CLlistaEstatica<T> com;
	com.dump(cout);	com.invariant(); cin.get();
	com.push_back(r++);
	com.dump(cout);	com.invariant(); cin.get();
	com.push_back(r++);
	com.dump(cout);	com.invariant(); cin.get();
	com.push_back(r++);
	com.dump(cout);	com.invariant(); cin.get();
	com.push_back(r++);
	com.dump(cout);	com.invariant(); cin.get();
	com.push_front(r++);
	com.dump(cout);	com.invariant(); cin.get();

//	com.dump(cout);	com.invariant(); cin.get();
	
	com.pop_front();
	com.dump(cout);	com.invariant(); cin.get();
	com.pop_back();
	com.dump(cout);	com.invariant(); cin.get();
	com.pop_front();
	com.dump(cout);	com.invariant(); cin.get();
	com.pop_back();
	com.dump(cout);	com.invariant(); cin.get();
	com.pop_back();
	com.dump(cout);	com.invariant(); cin.get();
	com.pop_front();
	com.dump(cout);	com.invariant(); cin.get();
}



#endif // !defined(__KKEP_LLISTAESTATICA_H_INCLUDED)
