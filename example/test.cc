#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <thread>

int main(int argc, char* argv[])
{
	if(argc != 2)
		return 0;
	int n = atoi(argv[1]);
	std::thread* workers[10000];
	
	for(int i=0; i<n; ++i)
	{
		workers[i] = new std::thread([](){system("./RPC_client");});
	}
	for(int i=0; i<n; ++i)
	{
		workers[i]->join();
		delete workers[i];
		workers[i] = nullptr;
	}
	return 0;
}
