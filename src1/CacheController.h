/*
	Cache Simulator Implementation by Justin Goins
	Oregon State University
	Fall Term 2018
*/

#ifndef _CACHECONTROLLER_H_
#define _CACHECONTROLLER_H_

#include "CacheStuff.h"
#include <string>

class CacheController {
	private:
		struct AddressInfo {
			unsigned long int tag;
			unsigned int setIndex;
		};
		unsigned int numByteOffsetBits;
		unsigned int numSetIndexBits;
		unsigned int globalCycles;
		unsigned int globalHits;
		unsigned int globalMisses;
		unsigned int globalEvictions;
		std::string inputFile, outputFile;

		ConfigInfo ci;

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
