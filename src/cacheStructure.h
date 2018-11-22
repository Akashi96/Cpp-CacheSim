/*
	CS-572 COMPUTER ARCHITECTURE
    Submitted By: Akash Agarwal
    OSU ID Number: 933471097
*/
#ifndef _CACHESTRUCTURE_H_
#define _CACHESTRUCTURE_H_

// PRE-DEFINED LIBRARIES
#include <vector>
#include <utility>
#include <list>

// nonDataBits.first represents Valid Bits
// nonDataBits.second represents Dirty Bits
typedef std::pair <bool, bool> nonDataBits;

// cacheEntry.second represent Tag Bits
typedef std::pair <nonDataBits, unsigned long int> cacheEntry;





#endif //CACHESTRUCTURE