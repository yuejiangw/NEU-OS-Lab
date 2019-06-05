#include<iostream>
using namespace std;
 
typedef struct pcb{
    int id;
    struct pcb *next;
}pcb;
 
pcb *ready,*run,*block;
 
pcb* cre()//创建带头结点的ready队列
{
    pcb *head=new pcb;
    head->next=NULL;
    pcb *q=head;
    int n;
    cout<<"输入进程数:\n";
    cin>>n;
    for(int i=0;i<n;i++)
    {
        pcb *p=new pcb;
        cin>>p->id;
        p->next=NULL;
        q->next=p;
        q=p;
    }
    return head;
}
 
void ins(pcb *head,pcb *node)//插入节点
{
    pcb *p=head;
    while(p->next)
    {
        p=p->next;
    }
    pcb *n=new pcb;
    n->id=node->id;
    p->next=n;
    n->next=NULL;
}
 
void del(pcb *head)
{
    pcb *p=head->next;
    if(p)
    {
    head->next=head->next->next;
    delete p;
    }
}
 
void dis()
{
    pcb *p;
    p=ready->next;
    cout<<"ready:\t";
    while(p)
    {
        cout<<p->id<<" ";
        p=p->next;
    }
    cout<<endl;
    p=run->next;
    cout<<"run  :\t";
    while(p)
    {
        cout<<p->id<<" ";
        p=p->next;
    }
    cout<<endl;
    p=block->next;
    cout<<"block:\t";
    while(p)
    {
        cout<<p->id<<" ";
        p=p->next;
    }
    cout<<endl;
}
 
int main()
{
    ready=cre();
    run=new pcb;
    run->next=NULL;
    block=new pcb;
    block->next=NULL;
    dis();
    int i;
    cout<<"chose:\t1:ready->run\t2:run->ready\t3:run->block\t4:block->ready\n";
    while(cin>>i)
    {
        if(i==1)
        {
            if(ready->next)
            {
                if(run->next)
                {
                    ins(block,run->next);
                    del(run);
                }
                ins(run,ready->next);
                del(ready);
            }
            else
            {
                if(block->next)
                {
                    ins(block,run->next);
                    del(run);
                    ins(run,block->next);
                    del(block);
                }
            }
        }
        else if(i==2)
        {
            if(run->next)
            {
                if(ready->next)
                {
                    ins(ready,run->next);
                    del(run);
                    ins(run,ready->next);
                    del(ready);
                }
            }
        }
        else if(i==3)
        {
            if(run->next)
            {
                if(ready->next)
                {
                    ins(block,run->next);
                    ins(run,ready->next);
                    del(run);
                    del(ready);
                }
                else
                {
                    ins(block,run->next);
                    ins(run,block->next);
                    del(block);
                    del(run);
                }
 
            }
        }
        else if(i==4)
        {
            if(block->next)
            {
                ins(ready,block->next);
                del(block);
            }
        }
        else
            break;
        dis();
        cout<<"chose:\t1:ready->run\t2:run->ready\t3:run->block\t4:block->ready\n";
    }
    return 0;
}