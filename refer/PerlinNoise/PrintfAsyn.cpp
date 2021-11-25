// PrintfAsyn.cpp
//
// By NightHz
//


#define WIN32_LEAN_AND_MEAN

#include "PrintfAsyn.h"
#include <stdio.h>
#include <string.h>
#include "CommandQueue.h"
#include <Windows.h>


static bool threadState = false;
static CommandQueue * queue = nullptr;


DWORD WINAPI PrintfAsynThread(LPVOID _p)
{
	UNREFERENCED_PARAMETER(_p);

	char * buffer;
	size_t size;
	while (true)
	{
		if(queue->Dequeue((void **)&buffer, &size))
		{
			printf(buffer);
			delete buffer;
		}
		else
			Sleep(500);
	}
	delete queue;
	queue = nullptr;
	return 0;
}


bool CreatePrintfAsynThread()
{
	if (threadState)
		return false;

	queue = new CommandQueue;

	if (!CreateThread(nullptr, 0, PrintfAsynThread, nullptr, 0, nullptr))
		return false;

	threadState = true;
	return true;
}


void PrintfAsyn(const char * _string)
{
	queue->Enqueue(_string, strlen(_string) + 1);
}


void PrintfAsynFlush()
{
	while (!queue->IsEmpty())
		Sleep(100);
}
