// Biotop.cpp: implementation of the CBiotop class.
//
//////////////////////////////////////////////////////////////////////

#include <fstream>
#include "Biotop.h"
#include "Substrat.h"
// Donat que es un template tota la implementacio ha de anar a la 
// capcelera (Es necessari per la majoria de compiladors

template <> void CBiotop<CSubstrat>::ProvaClasse(void) 
{
	uint32 i;
	uint32 nut=0;
	int escala[]={7,6,4,4,12,14,15,9};
	Topology topologia(400);
	CBiotop biotop(&topologia);
	t_position cuc[7]={130,130,130,130,130,130,130};
	while (cuc[0]!=8) {
		for (i=7;i--;) biotop[cuc[i]].ocupa(escala[6-i]);
		biotop[cuc[6]].desocupa();
		biotop[biotop.posicioAleatoria()].deposita(nut++);
		out << gotoxy(1,1) << hex;
		for (i=0; i<topologia.tamany(); i++) {
			CSubstrat &s = biotop[i];
			out << setw(1) << CColor(s.numeroMollecules()) << (s.ocupant()?'.':char('A'+s.ocupant()));
		}
		out << dec << endl;
		t_displacement direccio;
		rnd >> direccio;
		for (i=6;i--;) cuc[i+1]=cuc[i];
		cuc[0]=topologia.desplacament(cuc[0],direccio);
		if (cuc[0]==cuc[6]) cuc[0]=topologia.posicioAleatoria();
		out << blanc.fosc() << "Celles" << setfill(' ');
		for (i=7;i--;) out << setw(5) << cuc[i] << " - ";
		out<<setw(0)<<endl;
	}
	ofstream ofile("borrame",ios::out|ios::binary);
	biotop.store(ofile);
	ofile.close();
	CBiotop biotop2(&topologia);
	ifstream ifile("borrame",ios::in|ios::binary);
	biotop2.load(ifile);
	ifile.close();
	out << gotoxy(1,10) << hex;
	for (i=0; i<topologia.tamany(); i++) {
		CSubstrat &s = biotop2[i];
		out << setw(1) << CColor(s.numeroMollecules()) << (s.ocupant()?'.':char('A'+s.ocupant()));
	}
	out << endl;

}

