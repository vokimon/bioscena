// Configuracio.cpp: implementation of the CConfiguracio class.
//
//////////////////////////////////////////////////////////////////////

#include <fstream>
#include "Configuracio.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CConfiguracio Config;

CConfiguracio::CConfiguracio()
{
/* 
// Configuracio per organismes no deterministic
	set("Organisme/Energia/Caducitat",1);
	set("Organisme/Fenotip/Longitud",32);
	set("Organisme/Energia/Inicial",10);
	set("Comunitat/NumeroOrganismes",0x00000200);
	set("Comunitat/TamanyRegeneracio",27);
	set("Comunitat/NumeroTaxons",0x00000200);
	set("Comunitat/NumeroMarquesPerTaxo",030);
	set("Biotop/CassellesAmplitud",80);
	set("Biotop/CassellesAltitud",20);
	set("Biotop/Substrat/MaximInicial",6);
	set("Biotop/Substrat/MolleculesInicials/Numero",4);
	set("Biotop/Substrat/MolleculesInicials/Element",0);
	set("Biotop/Substrat/MolleculesInicials/Tolerancia",3);
*/
// Configuracio per organismes deterministics no reactius
	set("Organisme/Energia/Inicial",10);
	set("Organisme/Energia/CaducitatCompartiments",20);
	set("Organisme/Energia/Compartiments",8);
	set("Organisme/Fenotip/Longitud",32);
	// Bits que no conten a l'hora de penalitzar per llargaria de cariotip
	set("Organisme/Cariotip/PenalitzacioLlarg/BitsTamanyMaxim",8);
	// Factor pel que es multiplica el cost adicional per al pare per cromosoma llarg
	set("Organisme/Cariotip/PenalitzacioLlarg/Factor",6);
	set("Organisme/Cariotip/LongitudMinima",3);
	set("Organisme/Cariotip/LongitudMaxima",10);
	set("Organisme/Cromosoma/LongitudMinima",1);
	set("Organisme/Cromosoma/LongitudMaxima",10);
	set("Organisme/Genotip/ZonaOperadora/Mascara",0x00010000);
	set("Organisme/Genotip/Promotor/Mascara",     0x00100000);
	set("Organisme/Genotip/Terminador/Mascara",   0x00100000);
	set("Organisme/Genotip/Intro/Mascara",        0x00000000);
	set("Organisme/Genotip/Traduibilitat/Intents",5);
	set("Organisme/Pap/Capacitat",0);
	set("Organisme/ProbabilitatMutacio/Encerts",1);
	set("Organisme/ProbabilitatMutacio/Mostra",15);
	set("Comunitat/NumeroOrganismes",0x00000200);
	set("Comunitat/TamanyRegeneracio",27);
	set("Comunitat/NumeroTaxons",0x00000200);
	set("Comunitat/NumeroMarquesPerTaxo",030);
	set("Comunitat/ProbabilitatGeneracioExpontanea/Mostra",50);
	set("Comunitat/ProbabilitatGeneracioExpontanea/Encerts",1);
	set("Biotop/CassellesAmplitud",30);
	set("Biotop/CassellesAltitud",30);
	set("Biotop/Substrat/MaximInicial",7);
	set("Biotop/Substrat/MolleculesInicials/Numero",2);
	set("Biotop/Substrat/MolleculesInicials/Element",0);
	set("Biotop/Substrat/MolleculesInicials/Tolerancia",3);
	set("Biosistema/Quantum/Utils",2);
	set("Biosistema/Quantum/Maxim",4);
	set("Biosistema/Energia/FixeInstruccio",1);
	set("Biosistema/Energia/AdicionalInutil",1);
	set("Biosistema/Energia/Mitosi",15);
	set("Biosistema/Energia/Engolir",6);
	set("Biosistema/Energia/Excretar",3);
	set("Biosistema/Energia/Moviment",0);
	set("Biosistema/Energia/Extraccio",6);
	set("Biosistema/Energia/FactorDescendencia",2);
	set("Biosistema/OpCodes/BitsOperacio",5);
	set("Sensor/Quimic/Intents",10);
	set("Sensor/Presencia/Intents",20);
	set("Sensor/Intern/Intents",10);
	set("Biosistema/Metabolisme/BitsSignificatius",0x00000005);
}

CConfiguracio::~CConfiguracio()
{

}

void CConfiguracio::parsejaArxiu(char * nomArxiu, CMissatger & errors)
{
	// Obrim l'arxiu
	ifstream entrada(nomArxiu);
	t_clau parametre;
	t_valor valor;
	string prefetch;

	out << "Carregant la configuracio dels parametres generals [" << nomArxiu << "]" << endl;

	if (!entrada) {
		errors << "No s'ha pogut obrir \"" << nomArxiu << "\"" << endl;
		return;
	}

	entrada >> prefetch;
	while (entrada && prefetch=="*") {	
		entrada >> parametre >> valor >> prefetch;
		// TODO: Esborrar aquesta traca (o no)
//		out << '\t' << parametre << ":\t" << valor << endl;
		if (!exist(parametre)) {
			errors 
				<< "'" << parametre << "' no es un parametre configurable. " 
				<< "Ignorant el parametre" << endl;
			continue;
		}
		set(parametre,valor);
	}
	if (entrada) {
		errors << "No s'esperava '" << prefetch << "'"<< endl;
	}

	out << "Configuracio dels parametres generals [" << nomArxiu << "] carregada" << endl;

}


void CConfiguracio::dump(CMissatger& msg)
{
	t_diccionari::iterator it = m_diccionari.begin();
	for (;it!=m_diccionari.end(); it++)
	{
		msg << "* " << it->first
			<< "\t" << it->second 
			<< endl;
	}
}

void CConfiguracio::ProvaClasse()
{
	Config.dump(out);
	Config.parsejaArxiu("Bioscena.ini", error);
	Config.dump(out);
	cin.get();
}
