/*
	CS-572 COMPUTER ARCHITECTURE
    Submitted By: Akash Agarwal
    OSU ID Number: 933471097
*/
// PRE-DEFINED LIBRARIES
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <vector>
#include <algorithm>

// USER-DEFINED LIBRARIES
#include "cacheconf.h"
#include "cacheController.h"
#include "busOperation.h"

void bus::putMsg_onBus(std::string address, std::string message, funcPointer sendResponse, int threadId)
{
    std::cout << "Hello, In putMsg_onBus\n";
    
    if(this -> bus.find(address) != this -> bus.end())
    {// If the address is already present in  the bus, send the message to the function pointers of cache controllers present in the value vector
        auto element = this -> bus.find(address);
        // element contains address to the vector<pair<funcPointer, threadId> >
        if(message == "write")
        {   
            // if no other entry for same thread is present push_back the new entry
            auto newValue = std::make_pair(sendResponse, threadId);
            int foundThreadid = -1;
            // int foundPos = std::find(element -> second.begin(), element -> second.end(), newValue);
            for(auto itr = element -> second.begin(); itr != element -> second.end(); itr++)
            {
                if(itr -> second == threadId)
                {
                    foundThreadid = 0;
                    break;
                }
            }
            if(foundThreadid == -1)
            {
                element -> second.push_back(newValue);   // Add current thread's functionPointer in the vector
            }
            
            for(auto itr = element -> second.begin(); itr != element -> second.end(); )
            {   // Iterate the vector to send response to the other threads
                
                // std::cout << "Inside bus, iterating the function pointer vector\n";
                if(itr -> second != threadId)   // This will make sure that the thread doesnot invalidate its own cache entry
                {   
                    // convert the string key: "index|tag" to unsigned int index and unsigned long int tag
                    int pos = address.find("|");
                    unsigned int index = std::stoi(address.substr(0, pos - 0));
                    unsigned long int tag = std::stoi(address.substr(pos + 1, address.length() - pos - 1));
                    // std::cout << "index: " << index << "tag: " << tag << std::endl;
                    funcPointer fp = itr -> first;
                    fp(index, tag, message);
                    itr = element -> second.erase(itr); // Delete the functionPointer from the vector, because the corresponding cache doesnot 
                                                        // contain the entry for the address anymore.
                                                        // erase() shifts the next elements to the left, and hence erase() will return the value to next element
                } 
                
                else
                {
                    itr++;
                }
                
            }
            return;
        }
        
        if(message == "read")
        {
            // if no other entry for same thread is present push_back the new entry
            auto newValue = std::make_pair(sendResponse, threadId);
            int foundThreadid = -1;
            // int foundPos = std::find(element -> second.begin(), element -> second.end(), newValue);
            for(auto itr = element -> second.begin(); itr != element -> second.end(); itr++)
            {
                if(itr -> second == threadId)
                {
                    foundThreadid = 0;
                    break;
                }
            }
            if(foundThreadid == -1)
            {
                element -> second.push_back(newValue);   // Add current thread's functionPointer in the vector
            }
            
            if(element -> second.size() > 1)
            { // if size = 1, this means that only the current thread's function pointer is present in vector and no other
              // threads(caches) share the same data. Thus, set the shared bit only when there is sharing of data
                for(auto itr = element -> second.begin(); itr != element -> second.end(); itr++)
                {   // Iterate the vector to send response to the other threads

                    // convert the string key: "index|tag" to unsigned int index and unsigned long int tag
                    int pos = address.find("|");
                    unsigned int index = std::stoi(address.substr(0, pos - 0));
                    unsigned long int tag = std::stoi(address.substr(pos + 1, address.length() - pos - 1));
                    // std::cout << "index: " << index << "tag: " << tag << std::endl;
                    funcPointer fp = itr -> first;
                    fp(index, tag, message);
                }
            }
            return;

        }
         
    }
    
    else
    {   
        std::cout << "Adding fp to map\n";
        std::vector <std::pair <funcPointer, int> > vec;
        vec.push_back(std::make_pair(sendResponse, threadId));
        this -> bus[address] = vec;
        std::cout << "Added\n";
    }
    
}