#include <iostream>

using std::cin;
using std::cout;
using std::endl;

typedef struct pcb
{
    int id;
    struct pcb *next;
} pcb;

pcb *ready, *run, *block;

// 创建带头结点的ready队列
pcb *create()
{
    pcb *head = new pcb;
    head->next = NULL;
    pcb *q = head;
    int n;
    cout << "输入进程数:\n";
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        pcb *p = new pcb;
        cin >> p->id;
        p->next = NULL;
        q->next = p;
        q = p;
    }
    return head;
}

// 插入结点
void insert(pcb *head, pcb *node)
{
    pcb *p = head;
    while (p->next)
    {
        p = p->next;
    }
    pcb *n = new pcb;
    n->id = node->id;
    p->next = n;
    n->next = NULL;
}

// 删除结点
void del(pcb *head)
{
    pcb *p = head->next;
    if (p)
    {
        head->next = head->next->next;
        delete p;
    }
}

// 展示各个状态的进程
void show()
{
    pcb *p;

    p = ready->next;
    cout << "ready:\t";
    while (p)
    {
        cout << p->id << " ";
        p = p->next;
    }
    cout << endl;

    p = run->next;
    cout << "run:\t";
    while (p)
    {
        cout << p->id << " ";
        p = p->next;
    }
    cout << endl;

    p = block->next;
    cout << "block:\t";
    while (p)
    {
        cout << p->id << " ";
        p = p->next;
    }
    cout << endl;
}

int main()
{
    ready = create();
    run = new pcb;
    run->next = NULL;
    block = new pcb;
    block->next = NULL;
    show();
    int i;
    cout << "chose:\t0:exit\t1:ready->run\t2:run->ready\t";
    cout << "3:run->block\t4:block->ready" << endl;
    while (cin >> i)
    {
        if (i == 0)
        {
            break;
        }
        else if (i == 1)
        {
            if (ready->next)
            {
                if (run->next)
                {
                    insert(block, run->next);
                    del(run);
                }
                insert(run, ready->next);
                del(ready);
            }
            else
            {
                if (block->next)
                {
                    insert(block, run->next);
                    del(run);
                    insert(run, block->next);
                    del(block);
                }
            }
        }
        else if (i == 2)
        {
            if (run->next)
            {
                if (ready->next)
                {
                    insert(ready, run->next);
                    del(run);
                    insert(run, ready->next);
                    del(ready);
                }
            }
        }
        else if (i == 3)
        {
            if (run->next)
            {
                if (ready->next)
                {
                    insert(block, run->next);
                    insert(run, ready->next);
                    del(run);
                    del(ready);
                }
                else
                {
                    insert(block, run->next);
                    insert(run, block->next);
                    del(block);
                    del(run);
                }
            }
        }
        else if (i == 4)
        {
            if (block->next)
            {
                insert(ready, block->next);
                del(block);
            }
        }
        else
        {
            cout << "You can only type in numbers in range(0~4)" << endl;
        }
        show();
        cout << "chose:\t1:ready->run\t2:run->ready\t";
        cout << "3:run->block\t4:block->ready" << endl;
    }
    return 0;
}