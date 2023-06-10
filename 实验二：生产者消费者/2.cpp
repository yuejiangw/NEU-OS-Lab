#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <unistd.h>

#define BUFFER_SIZE 8 // 缓冲区大小
using namespace std;

vector<int> buffer(BUFFER_SIZE); // 缓冲区
queue<int> pQueue;				 // 生产者等待队列
queue<int> cQueue;				 // 消费者等待队列
int data;						 // 0-99
int writeptr, readptr;			 // 写指针　读指针
int consumer, producer;			 // 消费者等待数量　生产者等待数量

// 初始化
void init()
{
	for (int i = 0; i < BUFFER_SIZE; ++i)
	{
		buffer[i] = 0;
	}
	while (!pQueue.empty())
	{
		pQueue.pop();
	}
	while (!cQueue.empty())
	{
		cQueue.pop();
	}
	data = 0;
	writeptr = readptr = 0;
	consumer = producer = 0;
}

// 查找空位
int findEmpty(vector<int>& v, int len)
{
	for (int i = 0; i < len; ++i)
	{
		if (v[i] == 0)
		{
			return i;
		}
	}
	return -1;
}

// 查找脏位
int findDirty(vector<int>& v, int len)
{
	for (int i = 0; i < len; ++i)
	{
		if (v[i] > 0)
		{
			return i;
		}
	}
	return -1;
}

// 缓冲区是否满
bool isFull(vector<int>& v, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (v[i] == 0)
		{
			return false;
		}
	}
	return true;
}

// 缓冲区是否空
bool isEmpty(vector<int>& v, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (v[i] != 0)
		{
			return false;
		}
	}
	return true;
}

// 遍历展示队列中的内容
void showQueue(queue<int> q)
{
	int len = q.size();
	for (int i = 0; i < len; i++)
	{
		cout << q.front() << "	";
		q.pop();
	}
}

// 显示缓冲区内容
void show()
{
	for (int i = 0; i < BUFFER_SIZE; ++i)
	{
		if (buffer[i] > 0)
		{
			cout << "	" << buffer[i] << "   |";
		}
		else
		{
			cout << "      |";
		}
	}
	cout << "\nwriteptr = " << writeptr << "\treadptr = " << readptr << "\t";

	// 打印生产者
	if (producer)
	{
		cout << "\nPRODUCER wait : " << producer << "\n"
			<< "pQueue: ";
		showQueue(pQueue);
	}
	else
	{
		cout << "\nPRODUCER ready\t";
	}

	// 打印消费者
	if (consumer)
	{
		cout << "\nCONSUMER wait : " << consumer;
	}
	else
	{
		cout << "\nCONSUMER ready";
	}
	cout << endl;
}

// 生产
void produce()
{
	if (findEmpty(buffer, BUFFER_SIZE) == -1)
	{
		// 缓冲区满,阻塞
		++data;
		producer++;
		pQueue.push(data);
	}
	else if (isEmpty(buffer, BUFFER_SIZE))
	{
		// 缓冲区空
		if (cQueue.empty())
		{
			// 消费者等待队列为空,直接写入即可
			data++;
			buffer[writeptr] = data;
		}
		else
		{
			// 有阻塞的消费者，先写入再读出
			data++;
			buffer[writeptr] = data;
			show();
			sleep(2);
			buffer[readptr] = 0;
			readptr = (readptr + 1) % BUFFER_SIZE;
			consumer--;
			cQueue.pop();
		}
		writeptr = (writeptr + 1) % BUFFER_SIZE;
	}
	else
	{
		// 缓冲区不空也不满,直接写入即可
		data++;
		buffer[writeptr] = data;
		writeptr = (writeptr + 1) % BUFFER_SIZE;
	}
}

// 消费
void consume()
{
	if (findDirty(buffer, BUFFER_SIZE) == -1)
	{
		// 缓冲区空,则阻塞
		consumer++;
		cQueue.push(data);
	}
	else if (isFull(buffer, BUFFER_SIZE))
	{
		// 缓冲区满
		if (!pQueue.empty())
		{
			// 生产者等待队列不空，则先读出再写入
			buffer[readptr] = 0;
			readptr = (readptr + 1) % BUFFER_SIZE;
			show();
			sleep(2);
			buffer[writeptr] = pQueue.front();
			writeptr = (writeptr + 1) % BUFFER_SIZE;
			pQueue.pop();
			producer--;
		}
		else
		{
			// 生产者队列为空
			buffer[readptr] = 0;
			readptr = (readptr + 1) % BUFFER_SIZE;
		}
	}
	else
	{
		// 缓冲区不空也不满,直接读取即可
		buffer[readptr] = 0;
		readptr = (readptr + 1) % BUFFER_SIZE;
	}
}

// 启动程序
void start()
{
	char c;
	cin >> c;
	// e = exit    p = produce    c = consume
	while (c)
	{
		if (c == 'e')
		{
			break;
		}
		else if (c == 'p')
		{
			produce();
		}
		else if (c == 'c')
		{
			consume();
		}
		else
		{
			cout << "Please enter e(exit), p(produce), or c(consume)." << endl;
			continue;
		}
		show();
		cin >> c;
	}
}

int main()
{
	cout << "e-exit    p-produce	c-consume" << endl;
	init();
	start();
	return 0;
}
