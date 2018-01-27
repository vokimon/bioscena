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
	_size=size;
}

Topology::~Topology()
{
}

//////////////////////////////////////////////////////////////////////
// Redefinibles
//////////////////////////////////////////////////////////////////////

uint32 Topology::size() const
{
	return _size;
}

bool Topology::isValidPosition(Position pos) const
{
	return (pos<_size)&&(pos>=0);
}

Topology::Position Topology::displace (Position origin, Displacement movimentRelatiu) const
{
	return movimentRelatiu ? randomPosition() : origin;
}

Topology::Position Topology::displaceRandomly (Position origin, uint32 radi) const
{
	while (radi--) origin = displace (origin, rnd.get());
	return origin;
}

Topology::Displacement Topology::opositeDisplacement (Displacement desp) const
{
	Displacement inverse_displacement=rnd.get(0,0xffffffff);
	return inverse_displacement;
}

Topology::Displacement Topology::nilDisplacement () const
{
	return 0;
}

bool Topology::wayTo (Position posOrigen, Position posDesti, Displacement & desp) const
{
	desp = rnd.get();
	return desp==posDesti;
}

Topology::Position Topology::randomPosition() const
{
	return rnd.get(0,_size-1);
}

uint32 Topology::distance(Position posOrigen, Position posDesti) const
{
	return posOrigen==posDesti?0:_size-1;
}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////
#include "Color.h"
using namespace AnsiCodes;

void Topology::ClassTest(void) {
	// TODO: Proves Topologia::wayTo
	uint32 i;
	int escala[]={0,6,3,4,12,14,15,7};
	Topology topologia(400);
	uint32 * celles = new uint32[topologia.size()];
	for (i=topologia.size();i--;) celles[i]=0;
	Position cuc[7]={130,130,130,130,130,130,130};
	while (cuc[0]!=8) {
		for (i=7;i--;) celles[cuc[i]]=escala[6-i];
		celles[topologia.randomPosition()]=8;
		out << gotoxy(1,1) << hex;
		for (i=0; i<topologia.size(); i++)
			out << setw(1) << CColor(celles[i]) << "#";
		out << dec << endl;
		CRandomStream rnd;
		Displacement direccio;
		rnd>>direccio;
		for (i=6;i--;) cuc[i+1]=cuc[i];
		cuc[0]=topologia.displace(cuc[0],direccio);
		if (cuc[0]==cuc[6]) cuc[0]=topologia.randomPosition();
		out<<blanc.fosc()<<"Celles"<<setw(5)<<setfill(' ');
		for (i=7;i--;) out << cuc[i] << " - ";
		out<<setw(0)<<endl;
	}
}

