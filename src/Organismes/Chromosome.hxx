
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

	class Chromosome
	{
		// Inner Types
		public:
			typedef uint32 Codon;
			typedef std::vector<Codon> Codons;
		// Construction/Destruction
		public:
			/** Creates a chromosome without any codon */
			Chromosome() {}
			/**
			 * Returns the codon value at the specified position.
			 * @pre The position should be under the chromosome size.
			 */
			const Codon & operator[](uint32 position)
			{
				KKEP_ASSERT(position<size(),
					"Accessing to a non-existent codon.");
				return _codons.at(position);
			}
			/**
			 * Fills the chromosome with a sequential set of codons.
			 * This method is intended for testing.
			 */
			void initSequence(uint length, uint start=0)
			{
				_codons.resize(length);
				for (uint32 i = 0; i<length; i++)
					_codons[i]=start+i;
			}
			/**
			 * Returns the number of codons on the chromosome */
			uint32 size() const {return _codons.size();}

			/**
			 * Returns the chromosome string representation.
			 * The tipical representation of a chromosome is a colon separated
			 * bracketed list of hexadecimal 32 bits numbers like this one:
			 * @code
			 * [0000000:a4591ca0:fffffff]
			 * @endcode
			 */
			std::string asString()
			{
				std::ostringstream os;
				dumpOn(os);
				return os.str();
			}

			/**
			 * Dump the string representation on the ostream.
			 * It is faster than inserting the result of asString.
			 */
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

			/**
			 * Append the given chromosome content to the end
			 * of the receiver chromosome.
			 */
			void fuse(const Chromosome & toFuse)
			{
				const Codons & newCodons = toFuse._codons;
				_codons.insert(_codons.end(),
					newCodons.begin(),
					newCodons.end());
			}
			/**
			 * Extract part of the codons into the splitted chromosome.
			 * The splitted codons are removed from the original.
			 * The centromer is the first codon to be splitted.
			 * @return true when the split has been efective (useful centromer).
			 */
			bool split(unsigned int centromer, Chromosome & splitted)
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
