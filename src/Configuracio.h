// Configuracio.h: interface for the CConfiguracio class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_CONFIGURACIO_H_INCLUDED)
#define __KKEP_CONFIGURACIO_H_INCLUDED

#include <string>
#include <map>
#include "BioIncludes.h"
#include "Missatger.h"

static CMissatger errorConfiguracio("Error Configuracio");


/**
* A container for configuration parameters.
*/
class CConfiguracio  
{
// Tipus propis
	typedef uint32 t_valor;
	typedef string t_clau;
	typedef map <t_clau,t_valor> t_diccionari;
	typedef t_diccionari::value_type t_parellClauValor;
	typedef t_diccionari::iterator t_iteradorDiccionari;
// Construccio/Destruccio
public:
	CConfiguracio();
	virtual ~CConfiguracio();
// Operacions
public:
	void dump(CMissatger& msg);
	void parsejaArxiu(const char * nomArxiu, CMissatger & errors);
	void set (t_clau clau, t_valor valor) {
		m_diccionari.erase(clau);
		m_diccionari.insert(t_parellClauValor(clau,valor));
	}
	void get (t_clau clau, t_valor& valor) {
		t_iteradorDiccionari it = m_diccionari.find(clau);
		if (it!=m_diccionari.end())
			valor=it->second;
		else {
			errorConfiguracio << "No hi ha valor per la clau '" << clau << "'" << endl;
			cin.get();
		}
	}
	bool exist(t_clau clau) {
		t_iteradorDiccionari it = m_diccionari.find(clau);
		return it!=m_diccionari.end();
	} 
	// Aquesta funcio es insegura si no existeix la clau
	t_valor get (t_clau clau) {
		t_iteradorDiccionari it = m_diccionari.find(clau);
		if (it!=m_diccionari.end())
			return it->second;
		errorConfiguracio << "No hi ha valor per la clau '" << clau << "'" << endl;
		cin.get();
		return (uint32)-1;
	}
public:
// Atributs
public:
	static void ProvaClasse();
	t_diccionari m_diccionari;
};

extern CConfiguracio Config;

#endif // !defined(__KKEP_CONFIGURACIO_H_INCLUDED)
