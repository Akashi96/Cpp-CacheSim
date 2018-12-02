/*
	CS-572 COMPUTER ARCHITECTURE
    Submitted By: Akash Agarwal
    OSU ID Number: 933471097
*/
// PRE-DEFINED LIBRARIES
#include <iostream>
#include <fstream>
#include <thread>
#include <regex>
#include <cmath>
#include <functional>

// USER-DEFINED LIBRARIES
#include "cacheController.h"
#include "cacheStructure.h"
#include "cacheAccess.h"
#include "busOperation.h"



CacheController::CacheController(ConfigInfo config, char* traceFile, int threadId)
{
    // store the configuration info in the config object of this class
    this -> config = config;
    // store the name of the input file
    this -> inputFile = std::string(traceFile);
    // store the name of output file by appending .out in inputFile
    this -> outputFile = this -> inputFile + ".out";
    // compute no. of offset bits
    this -> numByteOffsetBits = log2(config.blockSize);
    // compute no. of Index bits
    this -> numIndexBits = log2(config.numberIndicies);

    // Initialize the counters
    this -> globalCycles = 0;
    this -> globalHits = 0;
    this -> globalMisses = 0;
    this -> globalEvictions = 0;
	this -> threadId = threadId;

	// form a function pointer to onBusresponse()
	funcPointer fp = std::bind(&CacheController::onBusresponse, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	this -> fp = fp;

    // Create Cache
    std::vector <std::vector <cacheEntry> > cache;
    
    for(size_t i = 0; i < config.numberIndicies; i++)
    {
        // std::vector <cacheEntry> cacheSet(config.associativity, std::make_pair(std::make_pair(1, 1), i));
        std::vector <cacheEntry> cacheSet(config.associativity);
        cache.push_back(cacheSet);
    }
    this -> cache = cache;
    
    // Print Cache
    std::cout << "Cache Created\n";
    std::cout << "Cache consists of " << cache.size() << " indicies\n";
	int i = 0;
    for(auto itr = cache.begin(); itr != cache.end(); itr++)
    {   
        std::cout << "Index:\tV  D  S  Tag\tV  D  S  Tag\tV  D  S  Tag\t" << std::endl;
        int set = 1;
        // std::cout << "Set\t" << "V " << "D " << "Tag \n";
		std::cout << i << "\t";
        for(auto i = itr -> begin(); i != itr -> end(); i++)
        {
            std::cout << i -> first.first.first << "  " 
                      << i -> first.first.second << "  " << i -> first.second << "  " << i -> second << "\t";
            set++;
        }
        i++;
		std::cout << "\n\n";
    }
}

void CacheController::onBusresponse(unsigned int index, unsigned long int tag, std::string message)
{
	std::cout << "In onBusresponse()\n";
	
	auto cacheSet = cache.at(index);
	if(message == "read")
	{
		for(auto itr = cacheSet.begin(); itr != cacheSet.end(); itr++)
		{
        	if(itr -> second == tag && itr -> first.first.first == 1)
			{
				itr -> first.second = 1; // set shared bit to one
				break;
			}

		}
		return;	
	}

	if(message == "write")
	{
		for(auto itr = cacheSet.begin(); itr != cacheSet.end(); itr++)
		{
			if(itr -> second == tag && itr -> first.first.first == 1)
			{
				itr -> first.first.first = 0; // Put Valid bit to 0
				cacheEntry block = *itr;
                cacheSet.erase(itr);
                cacheSet.emplace(cacheSet.end(), block); // Put the invalid entry at the end of set(For implementation reasons)
				break;
			}
		}
	}
	
}

CacheController::AddressInfo CacheController::getAddressInfo(unsigned long int address)
{	
	AddressInfo ai;
	
	std::cout << "address = " << address << std::endl;

	ai.tag = address >> (numByteOffsetBits + numIndexBits);
	std::cout << "tag = " << ai.tag << std::endl;
	
	unsigned long int offset = static_cast<unsigned long int>(std::pow(2, static_cast<double>(numByteOffsetBits)));
	std::cout << "offset = " << address % offset << std::endl;

	ai.setIndex = (address / offset) % (static_cast<unsigned long int>(std::pow(2, static_cast<double>(numIndexBits))));
	std::cout << "index = " << ai.setIndex << std::endl;
	return ai;
}

void CacheController::cacheAccess(CacheResponse* response, bool isWrite, unsigned long int address, 
											std::mutex& mutex, std::condition_variable& convar, bus& Bus)
{
	AddressInfo ai = getAddressInfo(address);

	// Check if the instruction is a read or write instruction
	if(isWrite)
		writeOnCache(ai.setIndex, ai.tag, cache.at(ai.setIndex), response, config, mutex, convar, Bus, threadId, fp);
	else
		readFromCache(ai.setIndex, ai.tag, cache.at(ai.setIndex), response, config, mutex, convar, Bus, threadId, fp);
	
	globalCycles += response -> cycles;
	
	if(response -> hit == 0)
		globalMisses++;
	else
		globalHits++;
	
	if(response -> eviction != 0)
		globalEvictions++;
}

/*
	Starts reading the tracefile and processing memory operations.
*/
void CacheController::runTracefile(std::mutex& mutex, std::condition_variable& convar, bus& Bus) 
{
	std::cout << "Input tracefile: " << inputFile << std::endl;
	std::cout << "Output file name: " << outputFile << std::endl;
	
	// process each input line
	std::string line;
	// define regular expressions that are used to locate commands
	std::regex commentPattern("==.*");
	std::regex instructionPattern("I .*");
	std::regex loadPattern(" (L )(.*)(,[[:digit:]]+)$");
	std::regex storePattern(" (S )(.*)(,[[:digit:]]+)$");
	std::regex modifyPattern(" (M )(.*)(,[[:digit:]]+)$");

	// open the output file
	std::ofstream outfile(outputFile);
	// open the output file
	std::ifstream infile(inputFile);
	// parse each line of the file and look for commands
	while (getline(infile, line)) {
		// these strings will be used in the file output
		std::string opString, activityString;
		std::smatch match; // will eventually hold the hexadecimal address string
		unsigned long int address;
		// create a struct to track cache responses
		CacheResponse response;

		// ignore comments
		if (std::regex_match(line, commentPattern) || std::regex_match(line, instructionPattern)) {
			// skip over comments and CPU instructions
			continue;
		} else if (std::regex_match(line, match, loadPattern)) {
			std::cout << "Found a load op!" << std::endl;
			std::istringstream hexStream(match.str(2));
			hexStream >> std::hex >> address;
			outfile << match.str(1) << match.str(2) << match.str(3);
			cacheAccess(&response, false, address, mutex, convar, Bus);
			outfile << " " << response.cycles << (response.hit ? " hit" : " miss") << (response.eviction ? " eviction" : "");
		} else if (std::regex_match(line, match, storePattern)) {
			std::cout << "Found a store op!" << std::endl;
			std::istringstream hexStream(match.str(2));
			hexStream >> std::hex >> address;
			outfile << match.str(1) << match.str(2) << match.str(3);
			cacheAccess(&response, true, address, mutex, convar, Bus);
			outfile << " " << response.cycles << (response.hit ? " hit" : " miss") << (response.eviction ? " eviction" : "");
		} else if (std::regex_match(line, match, modifyPattern)) {
			std::cout << "Found a modify op!" << std::endl;
			std::istringstream hexStream(match.str(2));
			hexStream >> std::hex >> address;
			outfile << match.str(1) << match.str(2) << match.str(3);
			// first process the read operation
			cacheAccess(&response, false, address, mutex, convar, Bus);
			std::string tmpString; // will be used during the file output
			tmpString.append(response.hit ? " hit" : " miss");
			tmpString.append(response.eviction ? " eviction" : "");
			unsigned long int totalCycles = response.cycles; // track the number of cycles used for both stages of the modify operation
			// now process the write operation
			cacheAccess(&response, true, address, mutex, convar, Bus);
			tmpString.append(response.hit ? " hit" : " miss");
			tmpString.append(response.eviction ? " eviction" : "");
			totalCycles += response.cycles;
			outfile << " " << totalCycles << tmpString;
		} else {
			throw std::runtime_error("Encountered unknown line format in tracefile.");
		}
		outfile << std::endl;
	}

	// Print Cache to see changes in the cache.
	std::cout << "Cache consists of " << cache.size() << " indicies\n";
	int i = 0;
    for(auto itr = cache.begin(); itr != cache.end(); itr++)
    {   
        std::cout << "Index:\tV  D  S  Tag\tV  D  S  Tag\tV  D  S  Tag\t" << std::endl;
        int set = 1;
		std::cout << i << "\t";
       
	    for(auto i = itr -> begin(); i != itr -> end(); i++)
        {
            std::cout << i -> first.first.first << "  " 
                      << i -> first.first.second << "  " << i -> first.second << "  " << i -> second << "\t";
            set++;
        }
        i++;
		std::cout << "\n\n";
    }
	// add the final cache statistics
	outfile << "Hits: " << globalHits << " Misses: " << globalMisses << " Evictions: " << globalEvictions << std::endl;
	outfile << "Cycles: " << globalCycles << std::endl;

	infile.close();
	outfile.close();
}
