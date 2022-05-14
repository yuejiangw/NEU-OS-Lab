#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <error.h>

#define F_ULOCK 0 //解锁
#define F_LOCK 1  //互斥锁定区域

int main()
{
	int num, fd[2], result = -1;
	pid_t pid, reid, gtid;
	char outpipe[100], father_str[100];

	//创建管道, fd[0]读管道，fd[1]写管道
	result = pipe(fd);
	if (result == -1)
	{
		printf("fail to create pipe \n");
		return -1;
	}

	// 创建子进程
	printf("input the number of process: ");
	scanf("%d", &num);
	printf("\n****************************************************\n");

	while (num > 0)
	{
		pid = fork();
		printf("pid: %d\n", pid);

		if (pid == -1)
		{
			// 创建失败
			printf("creat subprocess failed!\n");
			exit(0);
		}
		else if (pid == 0)
		{
			// 创建子进程，从子进程返回ID
			printf("now I'm writing in subprocess %d\n", gtid = getpid());
			lockf(fd[1], F_LOCK, 0); // 加锁
			sprintf(outpipe, "child process %d is sending message!\n", gtid);
			write(fd[1], outpipe, sizeof(outpipe));
			sleep(1);
			lockf(fd[1], F_ULOCK, 0); // 解锁
			printf("send message successfully and exit subprocess %d\n", gtid);
			exit(0);
		}
		else if (pid > 0)
		{
			// 创建子进程，从父进程返回子进程的ID
			reid = wait(NULL);
			if (reid == -1)
			{
				printf("father process calls subprocess failed!\n");
			}
			else
			{
				read(fd[0], father_str, sizeof(father_str));
				printf("\nfather process reads from subprocess %d:\n", reid);
				printf("%s\n", father_str);
			}
		}
		printf("****************************************************\n");
		num--;
	}
	return 0;
}