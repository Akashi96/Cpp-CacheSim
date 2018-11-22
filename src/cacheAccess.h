/*
	CS-572 COMPUTER ARCHITECTURE
    Submitted By: Akash Agarwal
    OSU ID Number: 933471097
*/
#ifndef _CACHEACCESS_H_
#define _CACHEACCESS_H_

// PRE-DEFINED LIBRARIES
#include <string>

// USER-DEFINED LIBRARIES
#include "cacheController.h"
#include "cacheStructure.h"
#include "cacheconf.h"

// void writeOnCache(unsigned int, unsigned long int, std::vector <std::vector <cacheEntry> >, CacheResponse*, ConfigInfo);
// void readFromCache(unsigned int, unsigned long int, std::vector <std::vector <cacheEntry> >, CacheResponse*, ConfigInfo);
void writeOnCache(unsigned int, unsigned long int, std::vector <cacheEntry>&, CacheResponse*, ConfigInfo);
void readFromCache(unsigned int, unsigned long int, std::vector <cacheEntry>&, CacheResponse*, ConfigInfo);


#endif //CACHEACCESS