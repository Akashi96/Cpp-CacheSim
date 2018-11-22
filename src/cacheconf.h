/*
	CS-572 COMPUTER ARCHITECTURE
    Submitted By: Akash Agarwal
    OSU ID Number: 933471097
*/

#ifndef _CACHECONF_H_
#define _CACHECONF_H_

enum class ReplacementPolicy 
{
	Random,
	LRU
};

enum class WritePolicy 
{
	WriteThrough,
	WriteBack
};

enum class CoherenceProtocol 
{
	Simple,
	MESI
};

// structure to hold information from the configuration file
struct ConfigInfo 
{
	unsigned int numberIndicies; // how many sets are in the cache
	unsigned int blockSize; // size of each block in bytes
	unsigned int associativity; // the level of associativity (N)
	ReplacementPolicy replacementPolicy;
	WritePolicy writePolicy;
	unsigned int cacheAccessCycles;
	unsigned int memoryAccessCycles;
	CoherenceProtocol protocol;
};

#endif //CACHECONF
