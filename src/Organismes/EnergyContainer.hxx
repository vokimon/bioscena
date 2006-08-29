#ifndef EnergyContainer_hxx
#define EnergyContainer_hxx

#include "BioIncludes.h"
#include <vector>

namespace Bioscena
{


/**
 * A container of energy which disipates old energy.
 * 
 * This class implements an energy container which disipates
 * the older chunk of energy obtained.
 * The number of slots is the number of disipations for
 * a previously fed energy in order to be lost.
 * Energy consumption takes it from older slots.
 */
class EnergyContainer
{
public:
	EnergyContainer(uint32 slots)
		: _energy(0)
		, _energySlots(slots)
		, _currentlyFed(0)
	{
	}
	uint32 feed(uint32 amount)
	{
		_energy += amount;
		_energySlots[_currentlyFed] += amount;
	}
	uint32 consume(uint32 amount)
	{
		uint32 notServed = amount;
		uint32 i = _currentlyFed;
		while (_energy && notServed)
		{
			if (not i) i = _energySlots.size();
			i--;
			uint32 & available = _energySlots[i];
			uint32 consumed = available<notServed? available : notServed;;
			_energy -= consumed;
			notServed -= consumed;
			available -= consumed;
		}
		return amount - notServed;
	}
	void disipate()
	{
		if (_currentlyFed==0)
			_currentlyFed=_energySlots.size();
		_currentlyFed--;
		_energy -= _energySlots[_currentlyFed];
		_energySlots[_currentlyFed] = 0;
	}
	uint32 total() const
	{
		return _energy;
	}
private:
	uint32 _energy;
	std::vector<uint32> _energySlots;
	uint32 _currentlyFed;
};


}


#endif//EnergyContainer_hxx
