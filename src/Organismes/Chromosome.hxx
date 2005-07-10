
#include "BioIncludes.h"
#include <vector>
#include <iomanip>

// TODO:
// Random init
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
			typedef std::vector<Codon> Codons;
		// Construction/Destruction
		public:
			Cromosome() {}
			const Codon & operator[](uint32 position)
			{
				KKEP_ASSERT(position<size(),
					"Accessing to a non-existent codon.");
				return _codons.at(position);
			}
			void initSequence(uint length, uint start=0)
			{
				_codons.resize(length);
				for (uint32 i = 0; i<length; i++)
					_codons[i]=start+i;
			}
			uint32 size() const {return _codons.size();}

			std::string asString()
			{
				std::ostringstream os;
				dumpOn(os);
				return os.str();
			}
			void dumpOn(std::ostream & os)
			{
				os << '[';
				os << std::setfill('0');
				os << std::hex;
				for (uint32 i = 0; i<size(); i++)
				{
					if (i) os << ':';
					os 
						<< std::setw(8)
						<< _codons[i];
				}
				os << std::dec;
				os << ']';
			}
			void fuse(const Cromosome & toFuse)
			{
				const Codons & newCodons = toFuse._codons;
				_codons.insert(_codons.end(),
					newCodons.begin(),
					newCodons.end());
			}
			bool split(unsigned int centromer, Cromosome & splitted)
			{
				if (centromer>=_codons.size()) return false;
				if (centromer==0) return false;
				splitted._codons.assign(
					_codons.begin()+centromer,
					_codons.end());
				_codons.erase(
					_codons.begin()+centromer,
					_codons.end());
				return true;
			}
		private:
			Codons _codons;
	};

}
