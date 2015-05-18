#include "FreeListAllocator.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <signal.h>

using namespace Allocators;

#define MAX_ARRAYSIZE 6000

char* array[MAX_ARRAYSIZE];

unsigned int arraySize = MAX_ARRAYSIZE;

std::string path = "files/";
int main()
{
	FreeListAllocator freeListAllocator;
	freeListAllocator.Initialize();


	std::fstream fs;
	for (int i = 0; i < arraySize; i+= 1)
	{
		array[i] = (char*)freeListAllocator.Malloc(sizeof(char));
	}

	for(int i =0; i < arraySize; i+=5)
	{
		std::string s;
		s = std::to_string(i - 4 * (i / 5));
		std::string concat = s + ".txt";
		std::sprintf(*(array + i * sizeof(char)), "%s", concat.c_str());

		fs.open(path + *(array + i * sizeof(char)),
		        std::fstream::in | std::fstream::out | std::fstream::app);
		fs << "Lorem ipsum";
		fs.close();

		pid_t pid = fork();
		if(pid == 0)
		{
			std::string file = path + *(array + i *sizeof(char));
			execl("/usr/bin/xdg-open", "xdg-open", file.c_str(), (char*)0);
			exit(1);
		}
	}
	for (int k = 0; k < arraySize; k++)
	{
		freeListAllocator.Free(array[k]);
	}


	sleep(5);
	for (int j = 0; j < arraySize; j+=5)
	{
		std::string s;
		s = std::to_string(j - 4 * (j / 5));
		std::string concat = path + s + ".txt";
		if(std::remove(concat.c_str()) != 0)
			std::cout << "Did not delete file" << std::endl;
	}
	kill(0, SIGKILL);

	return 0;
}
