#include <iostream>
#include <string>
#include <queue>
#define BUFFERSIZE 8

enum State{RUN=0, WAIT=1, READY=2};

int buffer[BUFFERSIZE]; //缓冲区

class PCB   //进程控制块
{
private:
    std::string pName;
    State pState = READY;
    int counts = 0;
public:
    PCB(){counts++;}
    PCB(std::string name);
    void setState(int x);
};

class Semaphore // 信号量
{
private:
    int sCount;             // 资源数
    std::queue<PCB> sQueue; // 等待队列
public:
    Semaphore(){}
    Semaphore(int x);
    void P(PCB p);
    void V();
};

class BufferInstruction // 缓冲区说明块
{
private:
    int readPtr;
    int writePtr;
    int readWaitPtr;
    int writeWaitPtr;
public:

};

