// Topologia.cpp: implementation of the Topology class.
//
//////////////////////////////////////////////////////////////////////
// Change Log
// 19990722 VoK - Fa servir templates per a les cel·les
// 19990723 VoK - Rebautizat: BiotopToroidal -> TopologiaToroidal
// 20000220 VoK - Ja no es un template, independitzat de les cel·les
// 20000708 VoK - esValidaCassella -> esPosicioValida

#include <iomanip>
#include "Topologia.h"
#include "RandomStream.h"
#include "Color.h"

using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Topology::Topology(uint32 tamany)
{
	m_totalCasselles=tamany;
}

Topology::~Topology()
{
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

uint32 Topology::tamany() const
{
	return m_totalCasselles;
}

bool Topology::esPosicioValida(t_posicio pos) const
{
	return (pos<m_totalCasselles)&&(pos>=0);
}

Topology::t_posicio Topology::desplacament (t_posicio origen, t_desplacament movimentRelatiu) const
// Retorna la posicio resultant de fer el desplacament des de l'origen
{
	// TODO: Per defecte aillades o indeterministic?
	return movimentRelatiu ? posicioAleatoria() : origen;
//	return origen;
}

Topology::t_posicio Topology::desplacamentAleatori (t_posicio origen, uint32 radi) const
// Retorna la posicio resultant de tants desplacaments aleatoris des de l'origen com indiqui el radi
{
	while (radi--) origen = desplacament (origen, rnd.get());
	return origen;
}

Topology::t_desplacament Topology::invers (t_desplacament desp) const
// Retorna un desplacament invers al desplacament
{
	// TODO: Per defecte aillades o indeterministic?
	t_desplacament inverse_displacement;
	rnd >> inverse_displacement;
	return inverse_displacement;
}

Topology::t_desplacament Topology::desplacamentNul () const
// Retorna un desplacament que aplicat a una posicio, retorna la posicio
{
	// TODO: Per defecte aillades o indeterministic?
	return 0;
}

bool Topology::unio (t_posicio posOrigen, t_posicio posDesti, t_desplacament & desp) const
// Retorna cert si es posible unir-les amb un sol desplacament, a desp hi es
// el desplacament per unir-les o apropar-les
{
	rnd >> desp;
	return false;
}

Topology::t_posicio Topology::posicioAleatoria() const
{
	CRandomStream rnd;
	return rnd.get(0,m_totalCasselles-1);
}

uint32 Topology::distancia(t_posicio posOrigen, t_posicio posDesti) const
{
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void Topology::ProvaClasse(void) {
	// TODO: Proves Topologia::unio
	uint32 i;
	int escala[]={0,6,3,4,12,14,15,7};
	Topology topologia(400);
	uint32 * celles = new uint32[topologia.tamany()];
	for (i=topologia.tamany();i--;) celles[i]=0;
	t_posicio cuc[7]={130,130,130,130,130,130,130};
	while (cuc[0]!=8) {
		for (i=7;i--;) celles[cuc[i]]=escala[6-i];
		celles[topologia.posicioAleatoria()]=8;
		out << gotoxy(1,1) << hex;
		for (i=0; i<topologia.tamany(); i++)
			out << setw(1) << CColor(celles[i]) << "#";
		out << dec << endl;
		CRandomStream rnd;
		t_desplacament direccio;
		rnd>>direccio;
		for (i=6;i--;) cuc[i+1]=cuc[i];
		cuc[0]=topologia.desplacament(cuc[0],direccio);
		if (cuc[0]==cuc[6]) cuc[0]=topologia.posicioAleatoria();
		out<<blanc.fosc()<<"Celles"<<setw(5)<<setfill(' ');
		for (i=7;i--;) out << cuc[i] << " - ";
		out<<setw(0)<<endl;
	}
}

//////////////////////////////////////////////////////////////////////
// Funcions estatiques
//////////////////////////////////////////////////////////////////////
/*
Topology * Topology::ParsejaArxiu(istream & str, CMissatger & errors)
{
	string nom, tipus, parametre, valor;
	string prefetch;
	CAgent * agentArrel = NULL;

	out << "Carregant la configuracio dels agents externs [" << nomArxiu << "]" << endl;
	if (!entrada) {
		errors 
			<< "Error obrint '" << nomArxiu << endl;
		return NULL;
	}

	entrada >> prefetch;
	while (prefetch=="*") {	
		entrada >> nom >> tipus >> prefetch;
		// TODO: Esborrar aquesta traca (o no)
//		out << '\t' << nom << '\t' << tipus << endl;
		CAgent * ag = CreaAgent(tipus, biotop);
		if (!agentArrel) agentArrel= ag;
		if (!ag) {
			errors 
				<< "No esta soportat importar agents de tipus '" << tipus 
				<< "' com l'especificat per a l'agent '" << nom 
				<< "' o no hi ha memoria suficient." << endl;
			continue;
		}
		if (diccionari.find(nom)!=diccionari.end()) {
			errors 
				<< "El nom de l'agent '" << nom << "' esta duplicat a l'arxiu. " 
				<< "Renombrant-ho com a '" << ag->nom() << "'." 
				<< "Les possibles referencies es perdran." << endl;
			continue;
		}
		diccionari[nom]=ag;
		if (!ag->nom(nom)) {
			errors 
				<< "El nom de l'agent '" << nom << "' ja existeix al sistema. " 
				<< "Renombrant-ho com a '" << ag->nom() << "'." << endl;
			continue;
		}
	}
	while (entrada && prefetch=="+") {
		entrada >> nom;
		entrada >> prefetch;
		t_diccionariAgents::iterator it = diccionari.find(nom);
		if (it==diccionari.end()) {
			errors 
				<< "Configurant l'agent '" << nom 
				<< "' sense haver especificat el seu tipus abans. " << endl;
			do entrada >> prefetch;
			while (entrada && prefetch!="+");
			continue;
		}
		while (entrada && prefetch=="-") {
			string parametre, valor;
			entrada >> parametre;
			getline(entrada, valor);
			entrada >> prefetch;
			strstream stream;
			stream << valor;
			if (!((*it).second->configura(parametre, stream, diccionari, errors))) {
				errors 
					<< "L'agent '" << nom 
					<< "' del tipus '" << (*it).second->tipus() 
					<< "' no respon al parametre de configuracio '" << parametre 
					<< "'"<< endl;
			}
		}
	}
	if (entrada) {
		errors << "No s'esperava '" << prefetch << "'"<< endl;
	}

	// TODO: Comprovacions de que es tracta d'una estructura arborea correcta

	out << "Configuracio dels agents externs [" << nomArxiu << "] carregada" << endl;

	return agentArrel;
}

CAgent * CAgent::CreaAgent(string tipus, CBiotop<CSubstrat> &biotop)
{
	if (tipus=="Agent/Multiple") return new CMultiAgent;
	if (tipus=="Agent/Multiple/Temporitzador") return new CTemporitzador;
	if (tipus=="Agent/Multiple/Iterador") return new CIterador;
	if (tipus=="Agent/Multiple/Aleaturitzador") return new CAleaturitzador;
	if (tipus=="Agent/Posicionador") return new CPosicionador(*(biotop.topologia()));
	if (tipus=="Agent/Posicionador/Aleatori") return new CPosicionadorAleatori(*(biotop.topologia()));
	if (tipus=="Agent/Posicionador/Zonal") return new CPosicionadorZonal(*(biotop.topologia()));
	if (tipus=="Agent/Posicionador/Direccional") return new CItinerari(*(biotop.topologia()));
	if (tipus=="Agent/Direccionador") return new CDireccionador(*(biotop.topologia()));
	if (tipus=="Agent/Direccionador/Aleatori") return new CDireccionadorAleatori(*(biotop.topologia()));
	if (tipus=="Agent/Actuador/Nutridor") return new CNutridor(biotop);
	if (tipus=="Agent/Actuador/Nutridor/Invers") return new CDesnutridor(biotop);

	return NULL;
}
*/
