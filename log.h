/*************************************************************************
    > File Name: log.h
    > Author: tongxianglong
    > Mail: studentTxl@163.com 
    > Created Time: Wed 28 Feb 2018 09:32:18 AM CST
 ************************************************************************/

#ifndef LOG_H
#define LOG_H

#include<stdarg.h>

void logcmd(char *fmt, ...);

int init_logfile(char *filename);

int logfile(char *file, int line, char *msg);

#endif
