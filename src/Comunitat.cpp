// Comunitat.cpp: implementation of the CComunitat class.
//
//////////////////////////////////////////////////////////////////////

#include "Comunitat.h"
#include "Configuracio.h"
#include "RandomStream.h"
#include "Color.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComunitat::CComunitat()
//	: m_txmist(Config.get("Comunitat/NumeroMarquesPerTaxo"))
{
}

CComunitat::~CComunitat()
{
	vector<CInfoOrganisme>::iterator it = m_organismes.begin();
	for (; it!=m_organismes.end(); ++it)
		it->cos(NULL);
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

void CComunitat::dump(CMissatger & msg)
{
	m_nOrganismes=0;
	msg << "Comunitat de " << (m_organismes.size() - m_disponibles.size()) << " organismes." << endl;
	vector<CInfoOrganisme>::iterator it = m_organismes.begin();
	for (uint32 index=0; it!=m_organismes.end(); ++it, ++index) {
//		if (it->cos()) {
			msg 
				<< groc.brillant() << "* Organisme #" << index << blanc.fosc()
				<< " Posicio: " << it->posicio() 
				<< " Taxo: " << it->taxo() 
				<< endl;
			msg << " Punter " << it->cos() << endl;
			//it->cos()->dump(msg);
//		}
	}
	dumpDisponibles(msg);
}

void CComunitat::dumpDisponibles(CMissatger & msg)
{
	vector<uint32>::iterator it = m_disponibles.begin();
	for (uint32 index=0; it!=m_disponibles.end(); ++it, ++index)
		msg << *it << "-";
	msg << endl;
}
//////////////////////////////////////////////////////////////////////
// Operacions
//////////////////////////////////////////////////////////////////////

uint32 CComunitat::introdueix(COrganisme* org, uint32 posicio, uint32 taxo)
	// PRE: org és un punter vàlid
{
	uint32 index;
	if (m_disponibles.size()) {
		index = m_disponibles.front();
		pop_heap(m_disponibles.begin(), m_disponibles.end(), greater<uint32>());
		m_disponibles.pop_back();
	} 
	else {
		index = m_organismes.size();
		m_organismes.push_back(CInfoOrganisme());
	}
	out << "Introduint organisme " << index << endl;
	CInfoOrganisme & nouOrganisme = m_organismes[index];
	nouOrganisme.taxo(taxo);
	nouOrganisme.posicio(posicio);
	nouOrganisme.cos(org);
	++m_nOrganismes;
	return index;
}

void CComunitat::extreu (uint32 index)
	// Pre: index es un organisme anteriorment introduit
{
	out << "Extreient organisme " << index << endl;
	CInfoOrganisme & vellOrganisme = m_organismes[index];
	// Eliminem l'organisme, si encara esta
	vellOrganisme.cos(NULL);
	// Posem el node 'index' disponible
	m_disponibles.push_back(index);
	push_heap(m_disponibles.begin(), m_disponibles.end(), greater<uint32>());
	// TODO: Alliberar espai quan hi hagi molt de lliure
	--m_nOrganismes;
}

uint32 CComunitat::organismeAleatori()
{
	KKEPAssert (m_organismes.size()>m_disponibles.size(), 
		"Comunitat: No hi ha organismes per escollir");
	while (true) {
		uint32 org=rnd.get(0,m_organismes.size()-1);
		if (m_organismes[org].cos()) return org;
	}
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CComunitat::ProvaClasse (void)
{
	CComunitat com;
	com.dump(out); cin.get();
	COrganisme * org;
	org = new COrganisme();
	com.introdueix(org,1,11);
	com.dump(out); cin.get();
	org = new COrganisme();
	com.introdueix(org,2,12);
	com.dump(out); cin.get();
	org = new COrganisme();
	com.introdueix(org,3,13);
	com.dump(out); cin.get();
	org = new COrganisme();
	com.introdueix(org,4,14);
	com.dump(out); cin.get();
	org = new COrganisme();
	com.introdueix(org,2,12);
	org = new COrganisme();
	com.introdueix(org,2,12);
	org = new COrganisme();
	com.introdueix(org,2,12);
	org = new COrganisme();
	com.introdueix(org,2,12);
	org = new COrganisme();
	com.introdueix(org,2,12);
	com.dump(out); cin.get();
	com.extreu(com.organismeAleatori());
	com.dump(out); cin.get();
	com.extreu(com.organismeAleatori());
	com.dump(out); cin.get();
	com.extreu(com.organismeAleatori());
	com.extreu(com.organismeAleatori());
	com.extreu(com.organismeAleatori());
	com.extreu(com.organismeAleatori());
	com.extreu(com.organismeAleatori());
	com.extreu(com.organismeAleatori());
	com.extreu(com.organismeAleatori());
//	com.extreu(com.organismeAleatori());
	com.dump(out); cin.get();
	org = new COrganisme();
	com.introdueix(org,5,15);
	com.dump(out); cin.get();
	org = new COrganisme();
	com.introdueix(org,6,16);
	com.dump(out); cin.get();
	org = new COrganisme();
	com.introdueix(org,7,17);
	com.dump(out); cin.get();
}


uint32 CComunitat::tamany()
{
	return m_organismes.size() - m_disponibles.size();
}
