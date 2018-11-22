/*
	CS-572 COMPUTER ARCHITECTURE
    Submitted By: Akash Agarwal
    OSU ID Number: 933471097
*/
// PRE-DEFINED LIBRARIES
#include <iostream>
#include <utility>
#include <list>
#include <thread>
#include <cmath>

// USER-DEFINED LIBRARIES
#include "cacheController.h"
#include "cacheStructure.h"
#include "cacheAccess.h"

void writeOnCache(unsigned int indexNo, unsigned long int tag, std::vector <cacheEntry>& cacheSet,
                     CacheResponse* response, ConfigInfo config)
{
    std::cout << "The instruction requires write on cache\n";
    int flag = -1; // This will keep check whether the value has been written in the cache or not
    unsigned int numberOfCycles = 0;
    
    // The cache will always be accessed for the first time. Add the cache access time to the total cycles
    numberOfCycles += config.cacheAccessCycles;
    // auto cacheSet = cache.at(indexNo);
    
    for(auto itr = cacheSet.begin(); itr != cacheSet.end(); itr++)
    {// We are at the index where we have to perform write operation, now check every set to find good position to write the data
       
        // CHECK FOR VALID BIT
        if(itr -> first.first == 0)
        {// If valid bit is 0, simply write on the cache block
            // std::cout << "Inside if tag == 0\n"; 
            response -> hit = 0;
            response -> eviction = 0;
            response -> dirtyEviction = 0;
            std::cout << "Hit: " << response -> hit << "\n"
                      << "Eviction: " << response -> eviction << "\n"
                      << "Dirty Eviction: " << response -> dirtyEviction << "\n";
            itr -> first.first = 1;
            flag = 0;
            // check the write-policy
            if(config.writePolicy == WritePolicy::WriteThrough)
            {   // If the write-policy is write-through, we don't care about the dirty bit and the data written to cache 
                // will also be written to the memory, so the memoryAccessCycles will be added to total clock cycles. 
                itr -> second = tag;
                numberOfCycles += config.memoryAccessCycles; 
            }
            
            else
            {   // If the write-policy is write-back, we set the dirty bit, since the data is not written back to 
                // the memory, no clock cycles will be added.
                itr -> second = tag;
                itr -> first.second = 1;
            }
            if (config.replacementPolicy == ReplacementPolicy::LRU) 
            {// move the recently written block to the first position of the array
                cacheEntry block = *itr;
                cacheSet.erase(itr);
                cacheSet.emplace(cacheSet.begin(), block);
            }
            break;
        }// end of if(itr -> first.first == 0)
        
        // CHECK FOR TAG BITS
        if(itr -> second == tag && itr -> first.first == 1)
        {// If valid bit is 1 and we find the matching tag
            response -> hit = 1;
            response -> eviction = 0;
            response -> dirtyEviction = 0;
            std::cout << "Hit: " << response -> hit << "\n"
                      << "Eviction: " << response -> eviction << "\n"
                      << "Dirty Eviction: " << response -> dirtyEviction << "\n";
            flag = 0;
            // check the write-policy
            if(config.writePolicy == WritePolicy::WriteThrough)
            {   // If the write-policy is write-through, we don't care about the dirty bit and the data written to cache 
                // will also be written to the memory, so the memoryAccessCycles will be added to total clock cycles. 
                numberOfCycles += config.memoryAccessCycles;
            }
            
            else
            {   // If the write-policy is write-back, we set the dirty bit, since the data is not written back to 
                // the memory, no clock cycles will be added.
                itr -> first.second = 1;
            }

            if (config.replacementPolicy == ReplacementPolicy::LRU) 
            {// move the recently written block to the first position of the array
                cacheEntry block = *itr;
                cacheSet.erase(itr);
                cacheSet.emplace(cacheSet.begin(), block);
            }
            break;
        }// end of if(itr -> second == tag && itr -> first.first == 1)
    }
    if(flag == -1)
    {// Nothing matched, we need to perform eviction
        response -> hit = 0;
        response -> eviction = 1;

        if(config.replacementPolicy == ReplacementPolicy::Random)
        {   
            auto itr = cacheSet.begin();
            std::advance(itr, (rand() % (config.associativity - 1) + 0));
            
            if(itr -> first.second == 1)
            {
                response -> dirtyEviction = 1;
                numberOfCycles += config.memoryAccessCycles;
            }
            else
                response -> dirtyEviction = 0;

            cacheSet.erase(itr);
            cacheEntry block;
            block.first.first = 1;
            block.second = tag;
            if(config.writePolicy == WritePolicy::WriteThrough)
            {
                numberOfCycles += config.memoryAccessCycles;
                block.first.second = 0;
            }
            else
                block.first.second = 1;
            cacheSet.emplace(itr, block);
        }
        
        else // If replacement policy is LRU
        {   
            auto itr = std::prev(cacheSet.end());
            
            if(itr -> first.second == 1)
            {
                response -> dirtyEviction = 1;
                numberOfCycles += config.memoryAccessCycles;
            }
            else
                response -> dirtyEviction = 0;

            cacheSet.erase(itr);
            cacheEntry block;
            block.first.first = 1;    
            block.second = tag;
            if(config.writePolicy == WritePolicy::WriteThrough)
            {
                numberOfCycles += config.memoryAccessCycles;
                block.first.second = 0;
            }
            else
                block.first.second = 1;
            cacheSet.emplace(cacheSet.begin(), block);
            std::cout << "Hit: " << response -> hit << "\n"
                      << "Eviction: " << response -> eviction << "\n"
                      << "Dirty Eviction: " << response -> dirtyEviction << "\n";
        }
    }
    response -> cycles = numberOfCycles;
    std::cout << "Number of Cycles: " << response -> cycles << "\n\n";


}

void readFromCache(unsigned int indexNo, unsigned long int tag, std::vector <cacheEntry>& cacheSet,
                     CacheResponse* response, ConfigInfo config)
{
    std::cout << "The instruction requires read from cache\n";
    unsigned int numberOfCycles = 0;
    int flag = -1;
    
    // The cache will always be accessed for the first time. Add the cache access time to the total cycles
    numberOfCycles += config.cacheAccessCycles;
    // auto cacheSet = cache.at(indexNo);
    
    for(auto itr = cacheSet.begin(); itr != cacheSet.end(); itr++)
    {// We are at the index where we have to perform write operation, now check every set to find good position to write the data
       
        // CHECK FOR VALID BIT
        if(itr -> first.first == 0)
        {// If valid bit is 0, simply write on the cache block
            response -> hit = 0;
            response -> eviction = 0;
            response -> dirtyEviction = 0;
            flag = 0;
            std::cout << "Hit: " << response -> hit << "\n"
                      << "Eviction: " << response -> eviction << "\n"
                      << "Dirty Eviction: " << response -> dirtyEviction << "\n";
            itr -> first.first = 1;
            itr -> second = tag;
            numberOfCycles += config.memoryAccessCycles;
            
            if (config.replacementPolicy == ReplacementPolicy::LRU) 
            {// move the recently written block to the first position of the array
                cacheEntry block = *itr;
                cacheSet.erase(itr);
                cacheSet.emplace(cacheSet.begin(), block);
            }
            break;
        }// end of if(itr -> first.first == 0)
        
        // CHECK FOR TAG BITS
        if(itr -> second == tag && itr -> first.first == 1)
        {// If valid bit is 1 and we find the matching tag
            response -> hit = 1;
            response -> eviction = 0;
            response -> dirtyEviction = 0;
            std::cout << "Hit: " << response -> hit << "\n"
                      << "Eviction: " << response -> eviction << "\n"
                      << "Dirty Eviction: " << response -> dirtyEviction << "\n";
            flag = 0;
            
            if (config.replacementPolicy == ReplacementPolicy::LRU) 
            {// move the recently written block to the first position of the array
                cacheEntry block = *itr;
                cacheSet.erase(itr);
                cacheSet.emplace(cacheSet.begin(), block);
            }
            break;
        }
    }

    if(flag == -1)
        {// Nothing matched, we need to perform eviction
            response -> hit = 0;
            response -> eviction = 1;

            if(config.replacementPolicy == ReplacementPolicy::Random)
            {   
                auto itr = cacheSet.begin();
                std::advance(itr, (rand() % (config.associativity - 1) + 0));
                numberOfCycles += config.memoryAccessCycles;
                cacheSet.erase(itr);
                cacheEntry block;
                block.first.first = 1;
                block.first.second = 0;
                block.second = tag;
                cacheSet.emplace(itr, block);
            }

            else // If replacement policy is LRU
            {   
                auto itr = std::prev(cacheSet.end());
                numberOfCycles += config.memoryAccessCycles;
                cacheSet.erase(itr);
                cacheEntry block;
                block.first.first = 1;
                block.first.second = 0;
                block.second = tag;
                cacheSet.emplace(cacheSet.begin(), block);
            }
            std::cout << "Hit: " << response -> hit << "\n"
                      << "Eviction: " << response -> eviction << "\n"
                      << "Dirty Eviction: " << response -> dirtyEviction << "\n";
        }
    response -> cycles = numberOfCycles;
    std::cout << "Number of Cycles: " << response -> cycles << "\n\n";
}