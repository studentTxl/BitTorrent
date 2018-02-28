/*************************************************************************
    > File Name: bterror.c
    > Author: tongxianglong
    > Mail: studentTxl@163.com 
    > Created Time: Wed 28 Feb 2018 09:22:52 AM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include"bterror.h"

void btexit(int errno, char *file, int line)
{
	printf("exit at %s : %d with error number : %d\n", file, line, errno);
	exit(errno);
}
