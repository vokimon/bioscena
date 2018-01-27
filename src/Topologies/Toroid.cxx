// Toroid.cxx: implementation of the Toroid class.
//
//////////////////////////////////////////////////////////////////////

#include <iomanip>
#include "Toroid.hxx"
#include "Color.h"
using namespace AnsiCodes;

namespace Bioscena
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Toroid::Toroid(uint32 xMax, uint32 yMax)
	// Precondicions:
	//   nD<xMax<0x1FFFFFFF on nD es el nombre maxim de displacements basics a un displacement complert
	//   2<yMax<(0xFFFFFFFF>>digitsSignificatius(xMax))
: Topology(xMax*yMax)
{
//	KKEPAssert(xMax<0x1FFFFFFF, "Toroid width too big, it may give problems"); // PORT
//	KKEPAssert(xMax>8, "Toroid width too small, it may give problems due to the maximun displacement."); // PORT
//	KKEPAssert(yMax>2, "Toroid height too small. Displacement algoritm does not suport this height."); // PORT
//	KKEPAssert(double(yMax)*double(yMax)<=double(0xFFFFFFFF), "Toroid size too big."); // PORT
//	cout<<"Inicializando un Substrato Toroidal de "<<xMax <<" X "<<yMax<<std::endl;
	m_direccions[N ]=  0 -xMax;
	m_direccions[NE]= +1 -xMax;
	m_direccions[E ]= +1 +0   ;
	m_direccions[SE]= +1 +xMax;
	m_direccions[S ]=  0 +xMax;
	m_direccions[SW]= -1 +xMax;
	m_direccions[W ]= -1 +0   ;
	m_direccions[NW]= -1 -xMax;
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
		stream<<std::endl;
		}
	stream << blanc.fosc();
}
*/

static void dumpTestToroid(Toroid&topo, uint32 * celles, uint32 x, uint32 y) {
//	std::cout << gotoxy(x,y); // PORT
	uint32 nCella=0;
	for (uint32 j=0;j<topo.height();j++) {
		for (uint32 i=0;i<topo.width();i++)
			std::cout << CColor(celles[nCella++]) << "#"; // PORT
		std::cout << std::endl;
		}
	std::cout << blanc.fosc(); // Port
}

void Toroid::ClassTest(void) {
	int escala[]={7,6,5,4,12,14,15,9};
	// TODO: Prova maximitzant els limits calculats als 'Pre' del constructor
	{
		std::cout << "\033[s";
		Toroid topo(100,10);
		uint32 * celles = new uint32[topo.size()];
		uint32 cuc[7]={130,130,130,130,130,130,130};
		for (uint32 i=topo.size();i--;) celles[i]=3;
		for (uint32 radi=0; radi<14; radi++) {
			cuc[0]=topo.randomPosition();
			for (unsigned times=400; --times; ) {
				for (uint32 i=7;i--;)
					celles[cuc[i]]=escala[6-i];
				celles[cuc[6]]=0;
				celles[topo.randomPosition()]=4;
				for (uint32 i=6;i--;) cuc[i+1]=cuc[i];
				cuc[0]=topo.displaceRandomly(cuc[0],radi);
				std::cout << "\033[1;1H";
				dumpTestToroid(topo,celles,1,2);
				std::cout << "\033[0m";
				std::cout
					<< "Radi: " << radi
					<< " Celles: "
					;
				for (uint32 i=7;i--;)
					std::cout <<std::setw(5)<<std::setfill(' ')<< cuc[i] << " - ";
				std::cout<<std::setw(0) << std::endl;
				// std::cin.get(); // Debugging aid
			}
		}
	}
	{
		Toroid topo(31,13);
		uint32 * celles = new uint32[topo.size()];
		uint32 cuc[7]={130,130,130,130,130,130,130};
		for (uint32 direction=0x88888800; direction<0x888888ff;direction++) {
			for (uint32 i=topo.size();i--;) celles[i]=3;
			for (uint32 j=topo.size()+7;j--;) {
				std::cout << "\033[1;1H";
				for (uint32 i=7;i--;) celles[cuc[i]]=escala[6-i];
				celles[cuc[6]]--;
				dumpTestToroid(topo,celles,1,2);
				for (uint32 i=6;i--;) cuc[i+1]=cuc[i];
				cuc[0]=topo.displace(cuc[0],direction);
//				KKEPAssert(topo.isValidPosition(cuc[0]),"Invalid position reached"); // PORT
				std::cout << "\033[0m";
				std::cout << "Dir:" << std::hex << direction <<" Celles: ";
				for (uint32 i=7;i--;) std::cout <<std::setw(5)<<std::setfill(' ')<< cuc[i] << " - ";
				std::cout<<std::setw(0)<<std::endl;
			}
		}
		std::cout << "\033[u";
	}
	{
		uint32 i;
		int escala[]={7,6,4,4,12,14,15,9};
		Toroid topo(25,21);
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
}

} // namespace Bioscena

