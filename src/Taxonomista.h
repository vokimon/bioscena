// Taxonomista.h: interface for the CTaxonomista class.
//
//////////////////////////////////////////////////////////////////////
// Change Log: 
// 19990306 VoK - Implementat 'parentesc'
// 19990306 VoK - Implementat 'discrimina'
// 19990306 VoK - Adaptat 'taxoAmbIndex' per les modificacions que es
//          VoK - van fer 'CLlistaEstatica'
// 20000113 VoK - Adaptat per un taxonomista simplificat
// 20000221 VoK - Serialitzacio
//////////////////////////////////////////////////////////////////////
// TODO Log:
// TODO: Implementar-ho amb una llista indexada
// TODO: Independitzar el modul d'acces per part del biosistema de la implementacio
//////////////////////////////////////////////////////////////////////

#if !defined(__KKEP_TAXONOMISTA_H_INCLUDED)
#define __KKEP_TAXONOMISTA_H_INCLUDED

#include <map>

// TODO: Posar InfoTaxo en un fitxer independent
class CInfoTaxo 
{
public:
	CInfoTaxo(uint32 primi=0, uint32 pred=0) {
		cens=0;
		primigeni=primi;
		predecesor=pred;
	}
	uint32 operator++() {return ++cens;}
	uint32 operator--() {return --cens;}
	uint32 cens; // Nombre d'organismes vius que hi pertanyen
	uint32 primigeni; // Taxo primigeni del que evoluciona
	uint32 predecesor; // Taxo existent del que evoluciona directament
};

class CTaxonomista 
{
// Construccio/Destruccio
public:
	CTaxonomista();
	virtual ~CTaxonomista();
// Redefinibles
public:
	virtual istream & load(istream & str);
	virtual ostream & store(ostream & str);
// Operacions (protocol amb Biosistema)
public:
	virtual bool defuncio (uint32 tx);
	virtual uint32 naixement (uint32 tx, bool mutat);
	virtual uint32 nouTaxoIndependent ();
	virtual bool creuaTaxons (uint32 tx1, uint32 tx2);
	virtual void envelleixTaxons ();
	virtual uint32 discrimina (uint32 taxo);
	void activaEspeciacio(bool especiar);
// Operacions (Consulta)
public:
	virtual uint32 parentesc (uint32 taxo1, uint32 taxo2) const;
	bool esValid(uint32 index) const;
	uint32 tamany() const;
// Atributs
private:
	bool m_calEspeciar;
	uint32 m_ultimTaxo;
	map<uint32,CInfoTaxo> m_taxons;
// Proves
public:
	static void ProvaClasse (void);
	virtual CMissatger & dump (CMissatger & stream);
};

#endif // !defined(__KKEP_TAXONOMISTA_H_INCLUDED)
