#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <error.h>
#include <wait.h>
#include <time.h>
#include <unistd.h>

#define true 1
#define false 0
#define frame_num 4
#define frame_num_FIFO 6
#define total_instruction 20

typedef enum { USED, UNUSED }frame_flag;/* flag of frame */

typedef struct oneframe { 
	int page_no; 
    frame_flag flag; 
}one_frame;

int main()
{
	/***********initialize variable***********/
	int i,j,pid,gtid,reid;
	int access_index = 0;		       		 /* the index process access Acess_Series */
	int success_flag = false;	       		 /* value is true if page exists else false */
	float diseffect = 0.0;		       		 /* pages absence times */
	float absence_rate = 0.0;	      		 /* pages absence Index of access --absence_rate=diseffect/total_instruction */
	float success_rate = 0.0;	      		 /* pages success Index of access --success_rate=1-absence_rate */
    int Acess_Series[total_instruction]; 	 /* the pages series for access of memory */
    one_frame M_Frame[frame_num];  	     	 /* memory access series */
	one_frame M_Frame_FIFO[frame_num_FIFO];  /* memory access series FIFO*/
	/***********initialize memory access series***********/
	srand((int)time(0));		       		 /* seed for random number */
	printf("* access series numbers:");
	for(i = 0; i < total_instruction; i++)
	{	
		Acess_Series[i] = 1 + (int)(10.0*rand()/(RAND_MAX+1.0));//[1,20]
		printf("%d ",Acess_Series[i]);	
	}
	printf("\n");
	/***********initialize data structure M_Frame***********/
    for(i = 0; i < frame_num; i++)
	{	
		M_Frame[i].page_no = 0;
		M_Frame[i].flag = UNUSED;
	}

	for(i=0;i<frame_num_FIFO;i++)
	{	
		M_Frame[i].page_no = 0;
		M_Frame[i].flag = UNUSED;
	}
	/**********************subprocess1**********************/
	pid = fork();

	if(pid == -1)		/* create process error */
	{
		printf("* creat subprocess 1 failed!\n");
		exit(0);
	}
	else if(pid == 0)	/* subprocess */
	{
		printf("* subprocess1 %d for FIFO!\n", gtid = getpid());

		while(access_index < total_instruction)
		{
			for(i = 0; i < frame_num; i++)	/* check if page exists in memory or not */
			{	
				if (M_Frame[i].flag == USED)
				{
					if(M_Frame[i].page_no == Acess_Series[access_index])
					{
						success_flag = true;/* page exists in memory */
						break;
					}
				}
				else
					break;
			}

			if(!success_flag) /* page absent/page not exists in memory */
			{
				diseffect += 1.0;

				for(i = 0; i < frame_num; i++)/* check if frames are full or not */
				{	
					if(M_Frame[i].flag == UNUSED)/* not full */
					{
						M_Frame[i].flag = USED;
						M_Frame[i].page_no = Acess_Series[access_index];
						break;
					}
				}

				if(i == frame_num)/* full */
				{
					for(i = 0; i < frame_num - 1; i++)	
						M_Frame[i].page_no = M_Frame[i + 1].page_no;			// 栈中元素依次前移
					M_Frame[frame_num - 1].page_no = Acess_Series[access_index];// 新元素在栈顶
				}
			}
			success_flag = false;
			access_index++;
		}

		absence_rate = diseffect / total_instruction;	// 缺页率
		success_rate = 1 - absence_rate;
		printf("* subprocess1 %d successful for FIFO [4]:absence_rate=%f,sbIndex=%f\n\n",gtid,absence_rate,success_rate);

		access_index = 0;
		success_flag = false;
		diseffect = 0.0;
		exit(0);
	}

	reid = wait(NULL);
	if(reid == -1)
		printf("* call subprocess 1 failed!\n");
	/**********************subprocess3**********************/
	pid = fork();

	if(pid == -1)    /* create process error */
	{
		printf("* creat subprocess 3 failed!\n");
		exit(0);
	}
	else if(pid == 0)/* subprocess */
	{
		printf("* subprocess3 %d for FIFO!\n", gtid = getpid());

		while(access_index < total_instruction)
		{
			for(i = 0; i < frame_num_FIFO; i++)/* check if page exists in memory or not */
			{	
				if(M_Frame_FIFO[i].flag == USED)
				{
					if(M_Frame_FIFO[i].page_no == Acess_Series[access_index])
					{
						success_flag = true;/* page exists in memory */
						break;
					}
				}
				else
					break;
			}

			if(!success_flag)/* page absent/page not exists in memory */
			{
				diseffect+=1.0;

				for(i=0;i<frame_num_FIFO;i++)/* check if frames are full or not */
				{	
					if(M_Frame_FIFO[i].flag == UNUSED)/* not full */
					{
						M_Frame_FIFO[i].flag = USED;
						M_Frame_FIFO[i].page_no = Acess_Series[access_index];
						break;
					}
				}

				if(i==frame_num_FIFO)/* full */
				{
					for(i=0;i<frame_num_FIFO - 1;i++)	
						M_Frame_FIFO[i].page_no = M_Frame_FIFO[i + 1].page_no;
					M_Frame_FIFO[frame_num_FIFO - 1].page_no = Acess_Series[access_index];
				}
			}

			success_flag=false;
			access_index++;
		}

		absence_rate=diseffect/total_instruction;
		success_rate=1-absence_rate;
		printf("* subprocess3 %d successful for FIFO [6]:absence_rate=%f,sbIndex=%f\n\n",gtid,absence_rate,success_rate);

		access_index = 0;
		success_flag = false;
		diseffect = 0.0;
		exit(0);
	}

	reid = wait(NULL);
	if(reid == -1)
		printf("* call subprocess 3 failed!\n");
	/**********************subprocess2**********************/
	pid = fork();

	if(pid == -1)    /* create process error */
	{
		printf("* creat subprocess 2 failed!\n");
		exit(0);
	}
	else if(pid == 0)/* subprocess */
	{
		printf("* subprocess2 %d for LRU!\n",gtid = getpid());

		while(access_index < total_instruction)
		{
			for(i = 0; i < frame_num; i++)/* check if page exists in memory or not */
			{	
				if (M_Frame[i].flag == USED)
				{
					if (M_Frame[i].page_no == Acess_Series[access_index])
					{
						success_flag = true;	/* page exists in memory */

						for(j = i; j < frame_num - 1; j++)/* reanrange the sequence of page in frames */
						{
							if (M_Frame[j + 1].flag == UNUSED)
								break;
							else
							{
								M_Frame[j].page_no = M_Frame[j + 1].page_no;
								M_Frame[j + 1].page_no = Acess_Series[access_index];
							}
						}
						break;
					}
				}
				else
					break;	
			}

			if(!success_flag)	/* page absent/page not exists in memory */
			{
				diseffect += 1.0;

				for(i = 0; i < frame_num; i++)		/* check if frames are full or not */
				{	
					if(M_Frame[i].flag == UNUSED)	/* not full */
					{
						M_Frame[i].flag = USED;
						M_Frame[i].page_no = Acess_Series[access_index];
						break;
					}
				}

				if(i==frame_num)/* full */
				{
					for(i=0;i<frame_num - 1;i++)	
						M_Frame[i].page_no = M_Frame[i + 1].page_no;
					M_Frame[frame_num - 1].page_no = Acess_Series[access_index];
				}
			}

			for(i=frame_num-1;i>=0;i--)/* print the state of frames */
			{
				if(M_Frame[i].flag == UNUSED)
					printf("* M_Frame[%d]:  \n",i);
				else
					printf("* M_Frame[%d]:%d\n",i,M_Frame[i].page_no);
			}
			printf("--------------\n");
			success_flag=false;
			access_index++;
		}

		absence_rate=diseffect/total_instruction;
		success_rate=1-absence_rate;
		printf("* subprocess2 %d successful for LRU [4]:absence_rate=%f,sbIndex=%f\n",gtid,absence_rate,success_rate);
		exit(0);
	}

	reid = wait(NULL);
	if(reid == -1)
		printf("* call subprocess 2 failed!\n");

	return 0;
}