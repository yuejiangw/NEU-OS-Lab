#include <iostream>
#include <queue>
#include <string>
using namespace std;
class PCB;
int id = 0;
queue<PCB> qReady;
queue<PCB> qBlocked;
int CPU = -1;    //代表cpu，存储当前运行进程的id,空闲时为-1
enum state{NEW=0, READY, RUNNING, BLOCKED, EXIT};

class PCB
{
private:
    int pId;        // id，唯一标识一个pcb
    state pState;    //pcb状态
    bool isRunning; // 是否进入cpu运行
public:
    PCB(){setState(state(NEW));}
    PCB(int i);

    int getState();
    int getId();
    void setState(int state);
    void setRun(bool i);
    void setId(int i);

    void Admit();
    void Dispatch();
    void Timeout();
    void EventOccurs();
    void EventWait();
    void Release();
};

void PCB::setId(int i)
{
    pId = i;
}
void PCB::setState(int i)
{
    pState = state(i);
}
void PCB::setRun(bool i)
{
    isRunning = i;
}
//　new -> ready
void PCB::Admit()
{
    setState(READY);
    qReady.push(*this);
}
// ready -> running
void PCB::Dispatch()
{
    CPU = qReady.front().pId;
    qReady.pop();
    setState(RUNNING);
    setRun(true);
}
// running -> ready
void PCB::Timeout()
{
    CPU = -1;
    setState(READY);
    setRun(false);
    qReady.push(*this);
}
// blocked -> ready
void PCB::EventOccurs()
{
    setState(READY);
    PCB tempPCB = qBlocked.front();
    qBlocked.pop();
    qReady.push(tempPCB);
}
// running -> blocked
void PCB::EventWait()
{
    CPU = -1;
    setState(BLOCKED);
    setRun(false);
    qBlocked.push(*this);
}
// running -> exit
void PCB::Release()
{
    CPU = -1;
    setState(EXIT);
    setRun(false);
}

PCB::PCB(int i)
{
    pId = i;
    pState = NEW;
    isRunning = false;
}
int PCB::getState()
{
    return pState;
}
int PCB::getId()
{
    return pId;
}

void NewToReady()
{

}


int main()
{
    while(1)
    {
        cout << "0-new 1-dispatch 2-timeout 3-EventWait 4-EventOccurs 5-release" << endl;
        int x;
        cin >> x;
        switch(x)
        {
        case 0:
            cout << "create process p" + i << endl;
            PCB p(i++);


        }
    }
}
