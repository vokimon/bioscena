/// @file Probabilitat.cpp: implementation of the CProbabilitatSaxona class.
//
//////////////////////////////////////////////////////////////////////

#include "Probabilitat.h"
#include <time.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProbabilitat::~CProbabilitat()
{

}

//////////////////////////////////////////////////////////////////////
// Proves
//////////////////////////////////////////////////////////////////////

void CProbabilitat::TestClass() {
	out << "-- Testing CProbabilitat" << std::endl;
	uint32 v[][4] = {
		{3,10,4,10},
		{3,10,2,5},
		{1,6,3,10}
	};
	uint32 e[][4] = {
		{3,10,3,10},
		{3,10,6,20},
		{6,6,30,30}
	};
	out << "Testing comparations for < operands" << std::endl;
	for (int i=sizeof(v)/sizeof(uint32[4]); i--;) {
		out << "Comparing "
				<< v[i][0] << "/" << v[i][1] << " and " 
				<< v[i][2] << "/" << v[i][3] << std::endl;
		if (CProbabilitat(v[i][0],v[i][1]) == CProbabilitat(v[i][2],v[i][3])) 
			error << "Unexpected result for ==" << std::endl;
		if (!(CProbabilitat(v[i][0],v[i][1]) != CProbabilitat(v[i][2],v[i][3]))) 
			error << "Unexpected result for !=" << std::endl;
		if (!(CProbabilitat(v[i][0],v[i][1]) < CProbabilitat(v[i][2],v[i][3]))) 
			error << "Unexpected result for <" << std::endl;
		if (CProbabilitat(v[i][0],v[i][1]) > CProbabilitat(v[i][2],v[i][3])) 
			error << "Unexpected result for >" << std::endl;
		if (CProbabilitat(v[i][0],v[i][1]) >= CProbabilitat(v[i][2],v[i][3])) 
			error << "Unexpected result for >=" << std::endl;
		if (!(CProbabilitat(v[i][0],v[i][1]) <= CProbabilitat(v[i][2],v[i][3]))) 
			error << "Unexpected result for <=" << std::endl;
		}
	out << "Testing comparations for > operands" << std::endl;
	for (int i=sizeof(v)/sizeof(uint32[4]); i--;) {
		out << "Comparing "
			<< v[i][0] << "/" << v[i][3] << " and " 
			<< v[i][2] << "/" << v[i][1] << std::endl;
		if (CProbabilitat(v[i][2],v[i][3]) == CProbabilitat(v[i][0],v[i][1]))
			error << "Unexpected result for ==" << std::endl;
		if (!(CProbabilitat(v[i][2],v[i][3]) != CProbabilitat(v[i][0],v[i][1]))) 
			error << "Unexpected result for !=" << std::endl;
		if (CProbabilitat(v[i][2],v[i][3]) < CProbabilitat(v[i][0],v[i][1])) 
			error << "Unexpected result for <" << std::endl;
		if (!(CProbabilitat(v[i][2],v[i][3]) > CProbabilitat(v[i][0],v[i][1]))) 
			error << "Unexpected result for >" << std::endl;
		if (!(CProbabilitat(v[i][2],v[i][3]) >= CProbabilitat(v[i][0],v[i][1]))) 
			error << "Unexpected result for >=" << std::endl;
		if (CProbabilitat(v[i][2],v[i][3]) <= CProbabilitat(v[i][0],v[i][1])) 
			error << "Unexpected result for <=" << std::endl;

	}
	out << "Testing comparations for == operands" << std::endl;
	for (int i=sizeof(e)/sizeof(uint32[4]); i--;) {
		out << "Comparing "
			<< v[i][0] << "/" << v[i][3] << " and " 
			<< v[i][2] << "/" << v[i][1] << std::endl;
		if (!(CProbabilitat(e[i][0],e[i][1]) == CProbabilitat(e[i][2],e[i][3]))) 
			error << "Unexpected result for ==" << std::endl;
		if (CProbabilitat(e[i][0],e[i][1]) != CProbabilitat(e[i][2],e[i][3])) 
			error << "Unexpected result for !=" << std::endl;
		if (CProbabilitat(e[i][0],e[i][1]) < CProbabilitat(e[i][2],e[i][3])) 
			error << "Unexpected result for <" << std::endl;
		if (CProbabilitat(e[i][0],e[i][1]) > CProbabilitat(e[i][2],e[i][3])) 
			error << "Unexpected result for >" << std::endl;
		if (!(CProbabilitat(e[i][0],e[i][1]) >= CProbabilitat(e[i][2],e[i][3]))) 
			error << "Unexpected result for >=" << std::endl;
		if (!(CProbabilitat(e[i][0],e[i][1]) <= CProbabilitat(e[i][2],e[i][3]))) 
			error << "Unexpected result for <=" << std::endl;
		}
	for (int i=sizeof(e)/sizeof(uint32[4]); i--;) {
		out << "Comparing "
			<< v[i][2] << "/" << v[i][1] << " and " 
			<< v[i][0] << "/" << v[i][3] << std::endl;
		if (!(CProbabilitat(e[i][2],e[i][3]) == CProbabilitat(e[i][0],e[i][1]))) 
			error << "Unexpected result for ==" << std::endl;
		if (CProbabilitat(e[i][2],e[i][3]) != CProbabilitat(e[i][0],e[i][1])) 
			error << "Unexpected result for !=" << std::endl;
		if (CProbabilitat(e[i][2],e[i][3]) < CProbabilitat(e[i][0],e[i][1])) 
			error << "Unexpected result for <" << std::endl;
		if (CProbabilitat(e[i][2],e[i][3]) > CProbabilitat(e[i][0],e[i][1])) 
			error << "Unexpected result for >" << std::endl;
		if (!(CProbabilitat(e[i][2],e[i][3]) >= CProbabilitat(e[i][0],e[i][1]))) 
			error << "Unexpected result for >=" << std::endl;
		if (!(CProbabilitat(e[i][2],e[i][3]) <= CProbabilitat(e[i][0],e[i][1]))) 
			error << "Unexpected result for <=" << std::endl;

	}
	const uint32 N = 0xffffff;
	const uint32 success=3;
	const uint32 sample=10;
	out << "Performance and success ratio tendency for a " << success << "/" <<sample << " probability..." << std::endl;
	{
		uint32 encerts=0;
		time_t t = time(NULL);
		for (uint32 i = N; i--;) {
			if (CProbabilitat(success,sample)) encerts++;
		}
		out << "Temporary object -"
			<< " Time:" << time(NULL)-t 
			<< " Encerts: " << encerts 
			<< " Mostra: " << N 
			<< " Ratio: " << ((double)encerts*100/N) 
			<< std::endl;
	}
	{
		uint32 encerts=0;
		time_t t = time(NULL);
		for (uint32 i = N; i--;) {
			if (CProbabilitat::EsDona(success,sample)) encerts++;
		}
		out << "Static function -" 
			<< " Time:" << time(NULL)-t 
			<< " Encerts: " << encerts 
			<< " Mostra: " << N 
			<< " Ratio: " << ((double)encerts*100/N) 
			<< std::endl;
	}
	{
		uint32 encerts=0;
		time_t t = time(NULL);
		for (uint32 i = N; i--;) {
			if (CProbabilitat::EsDona(3,10)) encerts++;
		}
		out << "Static function inmediate -" 
			<< " Time:" << time(NULL)-t 
			<< " Encerts: " << encerts 
			<< " Mostra: " << N 
			<< " Ratio: " << ((double)encerts*100/N) 
			<< std::endl;
	}
	{
		uint32 encerts=0;
		CProbabilitat p(success,sample);
		time_t t = time(NULL);
		for (uint32 i = N; i--;) {
			if (p.esDona()) encerts++;
		}
		out << "Stack object -"
			<< " Time:" << time(NULL)-t 
			<< " Encerts: " << encerts 
			<< " Mostra: " << N 
			<< " Ratio: " << ((double)encerts*100/N) 
			<< std::endl;
	}
}


