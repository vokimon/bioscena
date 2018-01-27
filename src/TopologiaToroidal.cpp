// TopologiaToroidal.cpp: implementation of the Torus class.
//
//////////////////////////////////////////////////////////////////////

#include <iomanip>
#include "Toroid.hxx"
#include "Color.h"
using namespace AnsiCodes;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Bioscena::Torus::Torus(uint32 xMax=5, uint32 yMax=5)
	// Precondicions:
	//   nD<xMax<0x1FFFFFFF on nD es el nombre maxim de displacements basics a un displacement complert
	//   2<yMax<(0xFFFFFFFF>>digitsSignificatius(xMax))
: Topology(xMax*yMax)
{
//	cout<<"Inicializando un Substrato Toroidal de "<<xMax <<" X "<<yMax<<endl;
	m_direccions[UP        ]=  0 -xMax;
	m_direccions[UP_RIGHT  ]= +1 -xMax;
	m_direccions[RIGHT     ]= +1 +0   ;
	m_direccions[DOWN_RIGHT]= +1 +xMax;
	m_direccions[DOWN      ]=  0 +xMax;
	m_direccions[DOWN_LEFT ]= -1 +xMax;
	m_direccions[LEFT      ]= -1 +0   ;
	m_direccions[UP_LEFT   ]= -1 -xMax;
	m_xMax=xMax;
	m_yMax=yMax;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

/*
void debugPresenta(CMissatger & stream) {
	stream << gotoxy(1,2);
	uint32 nCella=0;
	for (uint32 j=0;j<m_yMax;j++) {
		for (uint32 i=0;i<m_xMax;i++)
//			stream << m_casselles[nCella++] <<"#";
			stream << m_casselles[nCella++];
		stream<<endl;
		}
	stream << blanc.fosc();
}
*/

void Bioscena::Torus::ProvaClasse(void) {
	// TODO: Proves TopologiaToroidal::unio
	// TODO: Prova maximitzant els limits calculats als 'Pre' del constructor
	uint32 i;
	int escala[]={7,6,4,4,12,14,15,9};
	Torus topo(25,21);
	uint32 * celles = new uint32[topo.size()];
	uint32 cuc[7]={130,130,130,130,130,130,130};
	for (i=topo.size();i--;) celles[i]=3;
	while (cuc[0]!=8) {
		for (i=7;i--;) celles[cuc[i]]=escala[6-i];
		celles[cuc[6]]=0;
		celles[topo.randomPosition()]=4;
//		topo.debugPresenta(out);
		out << gotoxy(1,2);
		uint32 nCella=0;
		for (uint32 j=0;j<topo.height();j++) {
			for (uint32 i=0;i<topo.width();i++)
				out << CColor(celles[nCella++]) << "#";
			out << endl;
			}
		out << blanc.fosc();
		CRandomStream rnd;
		uint32 direccio;
		rnd>>direccio;
		for (i=6;i--;) cuc[i+1]=cuc[i];
		cuc[0]=topo.displace(cuc[0],direccio);
		if (cuc[0]==cuc[6]) cuc[0]=topo.randomPosition();
		out<<"Celles"<<setw(5)<<setfill(' ');
		for (i=7;i--;) out << cuc[i] << " - ";
		out<<setw(0)<<endl;
	}
}

