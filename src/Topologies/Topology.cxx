// Topologia.cpp: implementation of the Topology class.
//
//////////////////////////////////////////////////////////////////////

#include <iomanip>
#include "Topology.hxx"
#include "RandomStream.hxx"

using namespace Bioscena;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Topology::Topology(uint32 size)
{
	m_totalCasselles=size;
}

Topology::~Topology()
{
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

uint32 Topology::size() const
{
	return m_totalCasselles;
}

bool Topology::isValidPosition(Position pos) const
{
	return (pos<m_totalCasselles)&&(pos>=0);
}

Topology::Position Topology::displace (Position origin, Displacement movimentRelatiu) const
// Retorna la posicio resultant de fer el displacement des de l'origin
{
	// TODO: Per defecte aillades o indeterministic?
	return movimentRelatiu ? randomPosition() : origin;
//	return origin;
}

Topology::Position Topology::displaceRandomly (Position origin, uint32 radi) const
// Retorna la posicio resultant de tants displacements aleatoris des de l'origin com indiqui el radi
{
	while (radi--) origin = displace (origin, rnd.get());
	return origin;
}

Topology::Displacement Topology::opositeDisplacement (Displacement desp) const
// Retorna un displacement invers al displacement
{
	// TODO: Per defecte aillades o indeterministic?
	Displacement inverse_displacement=rnd.get();
	return inverse_displacement;
}

Topology::Displacement Topology::nilDisplacement () const
// Retorna un displacement que aplicat a una posicio, retorna la posicio
{
	// TODO: Per defecte aillades o indeterministic?
	return 0;
}

bool Topology::pathTowards (Position posOrigen, Position posDesti, Displacement & desp) const
// Retorna cert si es posible unir-les amb un sol displacement, a desp hi es
// el displacement per unir-les o apropar-les
{
	desp = rnd.get();
	return false;
}

Topology::Position Topology::randomPosition() const
{
	CRandomStream rnd;
	return rnd.get(0,m_totalCasselles-1);
}

uint32 Topology::distance(Position posOrigen, Position posDesti) const
{
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////
//#include "Color.h"
//using namespace AnsiCodes;

void Topology::ProvaClasse(void) {
#if 0
	// TODO: Proves Topologia::pathTowards
	uint32 i;
	int escala[]={0,6,3,4,12,14,15,7};
	Topology topologia(400);
	uint32 * celles = new uint32[topologia.size()];
	for (i=topologia.size();i--;) celles[i]=0;
	Position cuc[7]={130,130,130,130,130,130,130};
	while (cuc[0]!=8) {
		for (i=7;i--;) celles[cuc[i]]=escala[6-i];
		celles[topologia.randomPosition()]=8;
		std::cout << gotoxy(1,1) << std::hex;
		for (i=0; i<topologia.size(); i++)
			std::cout << std::setw(1) << CColor(celles[i]) << "#";
		std::cout << std::dec << std::endl;
		CRandomStream rnd;
		Displacement direccio;
		rnd>>direccio;
		for (i=6;i--;) cuc[i+1]=cuc[i];
		cuc[0]=topologia.displace(cuc[0],direccio);
		if (cuc[0]==cuc[6]) cuc[0]=topologia.randomPosition();
		std::cout<<blanc.fosc()<<"Celles"<<std::setw(5)<<std::setfill(' ');
		for (i=7;i--;) std::cout << cuc[i] << " - ";
		std::cout<<std::setw(0)<<std::endl;
	}
#endif 
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

	std::cout << "Carregant la configuracio dels agents externs [" << nomArxiu << "]" << std::endl;
	if (!entrada) {
		errors 
			<< "Error obrint '" << nomArxiu << std::endl;
		return NULL;
	}

	entrada >> prefetch;
	while (prefetch=="*") {	
		entrada >> nom >> tipus >> prefetch;
		// TODO: Esborrar aquesta traca (o no)
//		std::cout << '\t' << nom << '\t' << tipus << std::endl;
		CAgent * ag = CreaAgent(tipus, biotop);
		if (!agentArrel) agentArrel= ag;
		if (!ag) {
			errors 
				<< "No esta soportat importar agents de tipus '" << tipus 
				<< "' com l'especificat per a l'agent '" << nom 
				<< "' o no hi ha memoria suficient." << std::endl;
			continue;
		}
		if (diccionari.find(nom)!=diccionari.end()) {
			errors 
				<< "El nom de l'agent '" << nom << "' esta duplicat a l'arxiu. " 
				<< "Renombrant-ho com a '" << ag->nom() << "'." 
				<< "Les possibles referencies es perdran." << std::endl;
			continue;
		}
		diccionari[nom]=ag;
		if (!ag->nom(nom)) {
			errors 
				<< "El nom de l'agent '" << nom << "' ja existeix al sistema. " 
				<< "Renombrant-ho com a '" << ag->nom() << "'." << std::endl;
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
				<< "' sense haver especificat el seu tipus abans. " << std::endl;
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
					<< "'"<< std::endl;
			}
		}
	}
	if (entrada) {
		errors << "No s'esperava '" << prefetch << "'"<< std::endl;
	}

	// TODO: Comprovacions de que es tracta d'una estructura arborea correcta

	std::cout << "Configuracio dels agents externs [" << nomArxiu << "] carregada" << std::endl;

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