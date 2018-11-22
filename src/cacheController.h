/*
	CS-572 COMPUTER ARCHITECTURE
    Submitted By: Akash Agarwal
    OSU ID Number: 933471097
*/
#ifndef _CACHECONTROLLER_H_
#define _CACHECONTROLLER_H_

// PRE-DEFINED LIBRARIES
#include <string>

// USER-DEFINED LIBRARIES
#include "cacheconf.h"
#include "cacheStructure.h"


// this structure is filled with information about each memory access
struct CacheResponse 
{
	bool hit; // did this memory operation encounter a hit?
	bool eviction; // did this memory operation involve an eviction?
	bool dirtyEviction; // was the evicted block marked as dirty?
	unsigned int cycles; // how many clock cycles did this operation take?
};

class CacheController
{
	private:
		struct AddressInfo 
        {
			unsigned long int tag;
			unsigned int setIndex;
		};
		unsigned int numByteOffsetBits;
		unsigned int numIndexBits;
		unsigned int globalCycles;
		unsigned int globalHits;
		unsigned int globalMisses;
		unsigned int globalEvictions;
		std::string inputFile, outputFile;
        // std::vector <cacheEntry>* cachePtr;
        std::vector <std::vector <cacheEntry> > cache;
		ConfigInfo config;

		// function to allow read or write access to the cache
		void cacheAccess(CacheResponse*, bool, unsigned long int);
		// function that can compute the index and tag matching a specific address
		AddressInfo getAddressInfo(unsigned long int);
		// compute the number of clock cycles used to complete a memory access
		void updateCycles(CacheResponse*, bool);

	public:
		CacheController(ConfigInfo, char *);
		void runTracefile();
};

#endif //CACHECONTROLLER