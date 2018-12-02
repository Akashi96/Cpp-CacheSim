/*
	CS-572 COMPUTER ARCHITECTURE
    Submitted By: Akash Agarwal
    OSU ID Number: 933471097
*/
#ifndef _BUSOPERATION_H
#define _BUSOPERATION_H

// PRE-DEFINED LIBRARIES
#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <functional>

// USER-DEFINED LIBRARIES
#include "cacheconf.h"
#include "cacheStructure.h"

typedef std::function <void(unsigned int, unsigned long int, std::string)> funcPointer;
typedef std::vector <std::pair <funcPointer, int> > value;
class bus
{
    public:
        std::map <std::string, value> bus;
        void putMsg_onBus(std::string, std::string, std::function<void(unsigned int, unsigned long int, std::string)>, int);
        
};

#endif //BUSOPERATION