/*
	CS-572 COMPUTER ARCHITECTURE
    Submitted By: Akash Agarwal
    OSU ID Number: 933471097
*/
// PRE-DEFINED LIBRARIES
#include <iostream>
#include <fstream>
#include <thread>

// USER-DEFINED LIBRARIES
#include "cacheconf.h"
#include "cacheController.h"

void intitializeCache(ConfigInfo config, char* traceFile)
{
    CacheController cache = CacheController(config, traceFile);
    cache.runTracefile();
}

int main(int argc, char *argv[])
{
    ConfigInfo config;
     if (argc < 3)
     {
        std::cerr << "you need at least two command line arguments. you should provide a configuration file and at least one trace file." << std::endl;
	 	return 1;
     }
    unsigned int tmp;
   
	// Read the configuration file
    std::ifstream configFile(argv[1]);
    configFile >> config.numberIndicies;
    configFile >> config.blockSize;
    configFile >> config.associativity;
    configFile >> tmp;
    config.replacementPolicy = static_cast<ReplacementPolicy>(tmp);
    configFile >> tmp;
	config.writePolicy = static_cast<WritePolicy>(tmp);
	configFile >> config.cacheAccessCycles;
	configFile >> config.memoryAccessCycles;
	configFile >> tmp;
	config.protocol = static_cast<CoherenceProtocol>(tmp);
	configFile.close();

    // Print out the cache cofigurations retrieved from the configFile
    std::cout << "The Cache configuration is as follow:\n"
              << "Number of indices = " << config.numberIndicies << std::endl
              << "The size of each block = " << config.blockSize << std::endl
              << "Set Associativity = " << config.associativity << std::endl
              << "Therefore, each index will have " << config.associativity << " blocks" << std::endl;
    if (config.replacementPolicy == ReplacementPolicy::Random)
        std::cout << "Replacement Policy = Random\n";
    else
        std::cout << "Replacement Policy = LRU\n";
    
    if (config.writePolicy == WritePolicy::WriteThrough)
		std::cout << "Write Policy = Write-through\n" << std::endl;
	else
		std::cout << "Write Policy = Write-back\n" << std::endl;
    
    intitializeCache(config, argv[2]);
    
    return 0;
}
