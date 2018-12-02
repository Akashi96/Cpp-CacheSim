/*
	CS-572 COMPUTER ARCHITECTURE
    Submitted By: Akash Agarwal
    OSU ID Number: 933471097
*/
#ifndef _CACHEACCESS_H_
#define _CACHEACCESS_H_

// PRE-DEFINED LIBRARIES
#include <string>
#include <mutex>
#include <condition_variable>

// USER-DEFINED LIBRARIES
#include "cacheController.h"
#include "cacheStructure.h"
#include "cacheconf.h"
#include "busOperation.h"

// void writeOnCache(unsigned int, unsigned long int, std::vector <std::vector <cacheEntry> >, CacheResponse*, ConfigInfo);
// void readFromCache(unsigned int, unsigned long int, std::vector <std::vector <cacheEntry> >, CacheResponse*, ConfigInfo);
void writeOnCache(unsigned int, unsigned long int, std::vector <cacheEntry>&, CacheResponse*, ConfigInfo, std::mutex&, std::condition_variable&, bus&, int, std::function<void(unsigned int, unsigned long int, std::string)>);
void readFromCache(unsigned int, unsigned long int, std::vector <cacheEntry>&, CacheResponse*, ConfigInfo, std::mutex&, std::condition_variable&, bus&, int, std::function<void(unsigned int, unsigned long int, std::string)>);
void accessBus(unsigned int, unsigned long int, std::mutex&, std::condition_variable&, bus&, std::string, int, std::function<void(unsigned int, unsigned long int, std::string)>);

#endif //CACHEACCESS