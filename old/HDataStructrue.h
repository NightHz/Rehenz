// HDataStructrue.h
//
// By NightHz
//
//
//

#ifndef _HDATASTRUCTRUE_H_
#define _HDATASTRUCTRUE_H_

namespace Rehenz
{
	// ����Ԫ
	template <class T>
	struct LinkedListUnit
	{
	public:
		LinkedListUnit *next; // ��һ����Ԫ
		T data;   // ����

		LinkedListUnit(T& _data) : data(_data)
		{
			next = nullptr;
		}
	};

	// ����
	template <class T>
	class LinkedList
	{
	protected:
		LinkedListUnit<T> *head; // ����ͷ
		unsigned int size;   // �洢������
	public:
		LinkedList()
		{
			head = nullptr;
			size = 0;
		}
		~LinkedList()
		{
			LinkedListUnit<T> *next;
			while (true)
			{
				if (head == nullptr)
					break;
				next = head->next;
				delete head;
				head = next;
			}
		}

		T& operator[](unsigned int index)
		{
			LinkedListUnit<T> *p = head;
			for (int i = 0;i < index;i++)
				p = p->next;
			return p->data;
		}

		void Insert(T data, unsigned int index = 0)
		{
			LinkedListUnit<T> *unit = new LinkedListUnit<T>(data);
			if (index == 0) // ��������ͷ
			{
				unit->next = head;
				head = unit;
			}
			else
			{
				LinkedListUnit<T> *p = head;
				for (int i = 0;i < index-1;i++)
					p = p->next;
				unit->next = p->next;
				p->next = unit;
			}
			size++;
		}

		void Delete(unsigned int index)
		{
			LinkedListUnit<T> *unit;
			if (index == 0) // ɾ������ͷ
			{
				unit = head;
				head = unit->next;
			}
			else
			{
				LinkedListUnit<T> *p = head;
				for (int i = 0;i < index - 1;i++)
					p = p->next;
				unit = p->next;
				p->next = unit->next;
			}
			delete unit;
			size--;
		}

		unsigned int GetSize()
		{
			return size;
		}
	};
}

#endif // !_HDATASTRUCTRUE_H_
