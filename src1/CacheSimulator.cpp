/*
	Cache Simulator Implementation by Justin Goins
	Oregon State University
	Fall Term 2018
*/

#include "CacheSimulator.h"
#include "CacheStuff.h"
#include "CacheController.h"

#include <iostream>
#include <fstream>
#include <thread>

using namespace std;

/*
	This function creates the cache and starts the simulator.
	Accepts core ID number, configuration info, and the name of the tracefile to read.
*/
void initializeCache(int id, ConfigInfo config, char * tracefile) {
	CacheController singlecore = CacheController(config, tracefile);
	singlecore.runTracefile();
}

/*
	This function accepts a configuration file and a trace file on the command line.
	The code then initializes a cache simulator and reads the requested trace file(s).
*/
int main(int argc, char* argv[]) {
	ConfigInfo config;
	if (argc < 3) {
		cerr << "You need at least two command line arguments. You should provide a configuration file and at least one trace file." << endl;
		return 1;
	}

	

	// read the configuration file
	cout << "Reading config file: " << argv[1] << endl;
	ifstream infile(argv[1]);
	unsigned int tmp;
	infile >> config.numberSets;
	infile >> config.blockSize;
	infile >> config.associativity;
	infile >> tmp;
	config.rp = static_cast<ReplacementPolicy>(tmp);
	infile >> tmp;
	config.wp = static_cast<WritePolicy>(tmp);
	infile >> config.cacheAccessCycles;
	infile >> config.memoryAccessCycles;
	infile >> tmp;
	config.cp = static_cast<CoherenceProtocol>(tmp);
	infile.close();
	
	// Examples of how you can access the configuration file information
	cout << config.numberSets << " sets with " << config.blockSize << " bytes in each block. N = " << config.associativity << endl;

	if (config.rp == ReplacementPolicy::Random)
		cout << "Using random replacement protocol" << endl;
	else
		cout << "Using LRU protocol" << endl;
	
	if (config.wp == WritePolicy::WriteThrough)
		cout << "Using write-through policy" << endl;
	else
		cout << "Using write-back policy" << endl;

	// For multithreaded operation you can do something like the following...
	// Note that this just shows you how to launch a thread and doesn't address
	// the cache coherence problem.
	//thread t = thread(initializeCache, 0, config, argv[2]);
	//t.detach();

	// For singlethreaded operation, you use this:
	initializeCache(0, config, argv[2]);

	return 0;
}
