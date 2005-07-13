
#include "BioIncludes.h"
#include "Chromosome.hxx"
#include <vector>
#include <iomanip>

// TODO:


namespace Bioscena
{

	class Karyotype
	{
		// Inner Types
		public:
			typedef std::vector<Chromosome *> Chromosomes;
		// Construction/Destruction
		public:
			/** Creates a karyotype without any chromosome */
			Karyotype() {}
			Karyotype(const Karyotype & karyotype)
			{
				copyChromosomesFrom(karyotype);
			}
			~Karyotype()
			{
				removeAllChromosomes();
			}
			/**
			 * Returns the chromosome value at the specified position.
			 * @pre The position should be under the karyotype size.
			 */
			const Chromosome & operator[](uint32 position) const
			{
				KKEP_ASSERT(position<size(),
					"Accessing to a non-existent chromosome.");
				return * _chromosomes.at(position);
			}
			/**
			 * Fills the karyotype with a sequential set of chromosomes.
			 * This method is intended for testing.
			 */
			void initSequence(uint nChromosomes, uint nCodons, uint start=0)
			{
				_chromosomes.resize(nChromosomes);
				for (uint32 i = 0; i<nChromosomes; i++)
				{
					_chromosomes[i]=new Chromosome;
					_chromosomes[i]->initSequence(nCodons, (i+start)<<4);
				}
			}
			/**
			 * Returns the number of chromosomes on the karyotype */
			uint32 size() const {return _chromosomes.size();}

			/**
			 * Returns the karyotype string representation.
			 * The tipical representation of a karyotype is a colon separated
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
				for (uint32 i = 0; i<size(); i++)
				{
					if (i) os << ':';
					_chromosomes[i]->dumpOn(os);
				}
				os << ']';
			}

			const Karyotype & operator = (const Karyotype & karyotype)
			{
				removeAllChromosomes();
				copyChromosomesFrom(karyotype);
			}
		private:
			void copyChromosomesFrom(const Karyotype & karyotype)
			{
				const unsigned nChromosomes = karyotype.size();
				_chromosomes.resize(nChromosomes);
				for (uint32 i = 0; i<nChromosomes; i++)
					_chromosomes[i]=new Chromosome(karyotype[i]);
			}
			void removeAllChromosomes()
			{
				for (uint32 i = 0; i<size(); i++)
					delete _chromosomes[i];
			}
			Chromosomes _chromosomes;
	};

}
