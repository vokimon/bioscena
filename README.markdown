Bioscena, simulation of an evolutive biological system
======================================================

This project is a computer model that simulates a complex evolutionary world by emulating many traits of actual life.

![bioscenalogo](bioscenalogo.png)

[![CI](https://github.com/vokimon/bioscena/actions/workflows/main.yml/badge.svg)](https://github.com/vokimon/bioscena/actions/workflows/main.yml)
[![Coverage Status](https://coveralls.io/repos/github/vokimon/bioscena/badge.svg?branch=master)](https://coveralls.io/github/vokimon/bioscena?branch=master)

Bioscena beings are controlled by a mechanism which is analogous to the actual gene expression in real life beings.
Simulations show up many interesting emerging behaviours that you can monitor and trace.

The gene expression system works like that:

- Chromosomes are bit streams
	- Each 2 bits form a base (4 different bases)
	- Chromosomes can be multiple in a single being
	- Their lenght is variable
	- They may mutate when copied to the offspring
	- Mutations are as diverse as in nature: Base switch, inversions, translocations...
- Genes are little programs coded into the chromosomes
	- On transcription bases are grouped into codons, three bases
	- Special codons mark begining and end of each gene to be transcripted
	- A gene is expressed or not depending on its leading part, the promoter
	- The promoter codes a condition depending on the state of the phenotype
	- The rest of the gen are byte codes coding operations: Split, sense, move, feed, metabolize, excrete, modify the phenotype...
- Phenotype is an array of numbers
	- They control which genes are expressed
	- Actions of the being modify it
	- Other beings may observe it

Many behaviors that you might thing should be hard coded as world rules emerge by its own as result of evolution.

- Complexity: regardles the simplicity/easiness of the biotope, beings create the complexity with their interactions
- Mortality: Inmortals appear at first, but they are no longer relevant as reproducing beings appear
- Reproduction: Beings end choosing the proper time to do it, without dying
- Offspring care: Some beings develop strategies to benefit offspring, often harming themselves
- Aging: Genes causing degeneration are introduced and favored by evolution
- Trophic chains: While some beings specialize in taking nutrients from the biotope, others specialize on taking them from other beings
- Baiting: Some predators started dumping nutrients to attract their victims


