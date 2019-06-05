#include <iostream>
#include <vector>
#include <queue>
using namespace std;
#define BUFFERSIZE 8			// 缓冲区大小
vector<int> buffer(BUFFERSIZE);	// 缓冲区
queue<int> waitQueue;			// 等待队列
int data;						// 0-99
int writeptr, readptr;			// 写指针　读指针
int consumer, producer;			// 消费者等待数量　生产者等待数量

// 初始化
void init()
{
	for (int i = 0; i < BUFFERSIZE; ++i)
	{
		buffer[i] = 0;
	}
	while(!waitQueue.empty())
	{
		waitQueue.pop();
	}
	data = 0;
	writeptr = readptr = 0;
	consumer = producer = 0;  
}
// 查找空位
int findEmpty(vector<int>& v, int len)
{
	for(int i = 0; i < len; ++i)
	{
		if(v[i] == 0)
			return i;
	}
	return -1;
}
// 查找脏位
int findDirty(vector<int>& v, int len)
{
	for(int i = 0; i < len; ++i)
	{
		if(v[i] > 0)
			return i;
	}
	return -1;
}
// 显示缓冲区内容
void show()
{
    for(int i = 0; i < BUFFERSIZE; ++i)
    {
        if(buffer[i] > 0)
            cout << "  " << buffer[i] << "  |";
        else
            cout << "      |";
    }
    cout << "\nwriteptr = \t" << writeptr << "\treadptr = \t" << readptr << "\t";
    if(producer)
        cout << "PRODUCER wait : " << producer << "\t";
    else
        cout << "PRODUCER ready\t\t";
    if(consumer)
        cout << "CONSUMER wait : " << consumer;
    else
        cout << "CONSUMER ready";
    cout << "\n";
}
// 生产
void produce()
{
	if(findEmpty(buffer, BUFFERSIZE) == -1) // buffer full
	{
		++data;
		producer++;
		waitQueue.push(data);
	}
	else
	{
		if(waitQueue.empty())
		{
			data++;
			buffer[writeptr] = data;
		}
		else
		{
			producer--;
			buffer[writeptr] = waitQueue.front();
			waitQueue.pop();
		}
		writeptr = (writeptr + 1) % BUFFERSIZE;
	}
}
// 消费
void consume()
{
	if(findDirty(buffer, BUFFERSIZE) == -1) // buffer empty
	{
		if(waitQueue.empty())
		{
			consumer++;
		}
		else
		{
			buffer[writeptr] = waitQueue.front();
			waitQueue.pop();
			writeptr = (writeptr + 1) % BUFFERSIZE;
		}
	}
	else
	{
		buffer[readptr] = 0;
		readptr = (readptr + 1) % BUFFERSIZE;
	}
}
// 启动程序
void work()
{
	char c;
	cin >> c;
	// e = exit    p = produce    c = consume
	while(c != 'e' && (c == 'p' || c == 'c'))
	{
		if(c == 'p')
		{
			produce();
		}
		else if(c == 'c')
		{
			consume();
		}
		show();
		cin >> c;
	}
}

int main()
{
	cout << "e-exit    p-produce	c-consume" << endl;
	init();
	work();
	return 0;
}
