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

typedef enum { USED, UNUSED }frame_flag;    // 标签，判断页面是否被使用

typedef struct oneframe { 
	int page_no; 
    frame_flag flag; 
}one_frame;