
#include "BioIncludes.h"
#include <vector>

// TODO:
// Random init
// Sequence init (to do testing)
// fusion with another cromosome
// partition by a centromer
// remove codons
// add codons


namespace Bioscena
{

	class Cromosome
	{
		// Inner Types
		public:
			typedef uint32 Codon;
		// Construction/Destruction
		public:
			Cromosome() {}
			const Codon & operator[](uint32 position)
			{
				KKEP_ASSERT(position<size(),
					"Accessing to a non-existent codon.");
				return _codons[position];
			}
			uint32 size() const {return _codons.size();}
		private:
			std::vector<Codon> _codons;
	};

}
