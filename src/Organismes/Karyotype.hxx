
#include "BioIncludes.h"
#include "Chromosome.hxx"
#include "RandomStream.hxx"
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
			 * [
			 * [0000000:a4591ca0:fffffff]
			 * [00033e0:479678b3:1f3f331]
			 * ]
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
			/**
			 * Complete duplication of a single chromosome.
			 */
			void mutationByPositiveAneploidy(unsigned int toBeDuplicated, unsigned int targetPosition)
			{
				KKEP_ASSERT(toBeDuplicated<_chromosomes.size(),
					"Positive Anaeploidy: Selecting a chromosome beyond range");
				KKEP_ASSERT(targetPosition<=_chromosomes.size(),
					"Positive Anaeploidy: Placing duplicated chromosome beyond range");
				Chromosome * duplicate = new Chromosome(*_chromosomes[toBeDuplicated]);
				_chromosomes.insert(_chromosomes.begin()+targetPosition, duplicate);
			}
			void mutationByPositiveAneploidy()
			{
				unsigned int N = _chromosomes.size();
				mutationByPositiveAneploidy(random(0,N-1),random(0,N));;
			}
			/**
			 * Insertion of a new random chromosome.
			 */
			void mutationByRandomChromosomeInsertion(unsigned int targetPosition, unsigned int size)
			{
				KKEP_ASSERT(targetPosition<=_chromosomes.size(),
					"Positive Anaeploidy: Placing duplicated chromosome beyond range");
				Chromosome * chromosome = new Chromosome;
				chromosome->initRandom(size);
				_chromosomes.insert(_chromosomes.begin()+targetPosition, chromosome);
			}
			void mutationByRandomChromosomeInsertion()
			{
				unsigned int N = _chromosomes.size();
				unsigned int targetPosition = random(0,N);
				unsigned int size = random(4,10);
				mutationByRandomChromosomeInsertion(targetPosition, size);;
			}
			/**
			 * Deletion of a single chromosome.
			 */
			void mutationByNegativeAneploidy(unsigned int toBeRemoved)
			{
				if (size()<2) return;
				KKEP_ASSERT(toBeRemoved<_chromosomes.size(),
					"Negative Anaeploidy: Selecting a chromosome beyond range");

				delete _chromosomes[toBeRemoved];
				_chromosomes.erase(_chromosomes.begin()+toBeRemoved);
			}
			void mutationByNegativeAneploidy()
			{
				unsigned int N = _chromosomes.size();
				unsigned int toBeRemoved = random(0,N-1);
				mutationByNegativeAneploidy(toBeRemoved);
			}
			/**
			 * Complete duplication of the karyotype
			 */
			void mutationByEuploidy()
			{
				const unsigned nChromosomes = _chromosomes.size();
				_chromosomes.resize(nChromosomes*2);
				for (uint32 i = 0; i<nChromosomes; i++)
					_chromosomes[nChromosomes+i]=new Chromosome(*_chromosomes[i]);

			}
			void mutationByFusion(unsigned int receiver, unsigned int fused)
			{
				KKEP_ASSERT(receiver<_chromosomes.size(),
					"MutationByFusion: Selecting a chromosome beyond range");
				KKEP_ASSERT(fused<_chromosomes.size(),
					"MutationByFusion: Fusing a chromosome beyond range");

				_chromosomes[receiver]->fuse(*_chromosomes[fused]);
				if (receiver==fused) return;
				delete _chromosomes[fused];
				_chromosomes.erase(_chromosomes.begin()+fused);
			}
			void mutationByFusion()
			{
				unsigned int N = _chromosomes.size();
				unsigned int receiver = random(0,N-1);
				unsigned int fused = random(0, N-1);
				mutationByFusion(receiver,fused);;
			}
			void mutationBySplit(unsigned int toSplit, unsigned int splitPoint, unsigned int target)
			{
				KKEP_ASSERT(toSplit<size(),
					"MutationBySplit: Splitting a chromosome beyond range");
				KKEP_ASSERT(splitPoint<_chromosomes[toSplit]->size()-1,
					"MutationBySplit: Split point beyond chromosome lenght");
				KKEP_ASSERT(splitPoint>0,
					"MutationBySplit: Split point can not be 0");
				KKEP_ASSERT(target<=size(),
					"MutationBySplit: Target chromosome position beyond range");

				Chromosome * chromosome = new Chromosome;
				bool hasBeenSplitted = _chromosomes[toSplit]->split(splitPoint,*chromosome);
				_chromosomes.insert(_chromosomes.begin()+target, chromosome);
			}
			void mutationBySplit()
			{
				unsigned int N = _chromosomes.size();
				unsigned int toSplit = random(0,N-1);
				unsigned int n = _chromosomes[toSplit]->size();
				unsigned int splitPoint = random(1,n-2);
				unsigned int target = random(0, N);
				mutationBySplit(toSplit, splitPoint, target);
			}
			void mutationByTranslocation(unsigned source, unsigned splitPoint, unsigned length, unsigned target, unsigned insertPoint )
			{
				KKEP_ASSERT(source<size(),
						"MutationByTranslocation: Source chromosome position beyond range");
				KKEP_ASSERT(target<size(),
						"MutationByTranslocation: Target chromosome position beyond range");

				Chromosome & c1 = *_chromosomes[source];
				Chromosome & c2 = *_chromosomes[target];

				KKEP_ASSERT(splitPoint<c1.size(),
						"MutationByTranslocation: Segment start point beyond range");
				KKEP_ASSERT(insertPoint<c2.size(),
						"MutationByTranslocation: Insertion point beyond target chromosome size");
				KKEP_ASSERT(length!=0,
						"MutationByTranslocation: Zero length translocated segment");
				KKEP_ASSERT(length<c1.size(),
						"MutationByTranslocation: Total migration of codons from source");
				// TODO: Same cromosome
				c2.insertSegment(c1,splitPoint,length,insertPoint);
				c1.removeSegment(splitPoint, length);
			}
			void mutationByTranslocation()
			{
				unsigned int N = _chromosomes.size();
				unsigned int source = random(0,N-1);
				unsigned int n1 = _chromosomes[source]->size();
				unsigned int target = random(0, N-1);
				unsigned int n2 = _chromosomes[target]->size();
				unsigned int splitPoint = random(0,n1-1);
				unsigned int length = random(1, N-1);
				unsigned int insertPoint = random(0,n2-1);
				mutationByTranslocation(source, splitPoint, length, target, insertPoint);
			}
		private:
			unsigned int random(unsigned int lower, unsigned int higher)
			{
				return Random::Get(lower, higher);
			}
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
