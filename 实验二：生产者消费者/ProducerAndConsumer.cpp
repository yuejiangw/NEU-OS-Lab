#include "ProducerAndConsumer.h"

PCB::PCB(std::string name)
{
    pName = name;
    counts++;
}
void PCB::setState(int x)
{
    pState = State(x);
}

Semaphore::Semaphore(int x)
{
    this.sCount = x;
}
void Semaphore::P(PCB p)
{
    sCount--;
    if(sCount < 0)
    {
        p.setState(1);
        sQueue.push(p);
    }
}
void Semaphore::V()
{
    sCount++;
    if(sCount <= 0)
    {
        PCB p = sQueue.front();
        p.setState(2);
        // 插入就绪队列
    }
}
