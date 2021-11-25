// CommandQueue.cpp
//
// By NightHz
//


#include "CommandQueue.h"
#include <memory.h>


struct CommandQueueUnit
{
	void * command;
	size_t commandSize;
	CommandQueueUnit * next;

	CommandQueueUnit()
		: command(nullptr), next(nullptr)
	{
	}
};


CommandQueue::CommandQueue()
{
	// keep least one unit in queue
	head = new CommandQueueUnit;
	tail = head;
}


CommandQueue::~CommandQueue()
{
	CommandQueueUnit * unit = (CommandQueueUnit *)head;
	CommandQueueUnit * unit2;
	while (unit)
	{
		unit2 = unit;
		unit = unit->next;
		if (unit2->command)
			delete unit2->command;
		delete unit2;
	}
}

void CommandQueue::Enqueue(const void * _command, size_t _commandSize)
{
	// make a new command unit
	CommandQueueUnit * unit = new CommandQueueUnit;
	unit->command = new char[_commandSize];
	memcpy(unit->command, _command, _commandSize);
	unit->commandSize = _commandSize;
	// link to queue
	((CommandQueueUnit *)tail)->next = unit;
	tail = unit;
}


bool CommandQueue::Dequeue(void **_pCommand, size_t * _pCommandSize)
{
	CommandQueueUnit * unit = ((CommandQueueUnit *)head)->next;
	if (unit)
	{
		if (_pCommand)
			(*_pCommand) = unit->command;
		unit->command = nullptr;
		if (_pCommandSize)
			(*_pCommandSize) = unit->commandSize;
		delete head;
		head = unit;
		return true;
	}
	else
		return false;
}

bool CommandQueue::IsEmpty()
{
	if (((CommandQueueUnit *)head)->next)
		return false;
	else
		return true;
}

