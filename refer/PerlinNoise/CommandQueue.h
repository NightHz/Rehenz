// CommandQueue.h
// 
// By NightHz
// 
// 允许同时进行单读和单写操作
// 可用于主线程向其他线程传递命令
// 

#ifndef _COMMANDQUEUE_H_
#define _COMMANDQUEUE_H_


class CommandQueue
{
private:
	void * head;
	void * tail;

public:
	CommandQueue();
	~CommandQueue();

	void Enqueue(const void * _command, size_t _commandSize);
	bool Dequeue(void **_pCommand, size_t * _pCommandSize);

	bool IsEmpty();
};


#endif // !_COMMANDQUEUE_H_
