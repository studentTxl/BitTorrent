/*************************************************************************
    > File Name: bterror.h
    > Author: tongxianglong
    > Mail: studentTxl@163.com 
    > Created Time: Wed 28 Feb 2018 09:04:14 AM CST
 ************************************************************************/

#ifndef BTERROR_H
#define BTERROE_H

#define FILE_FD_ERR               -1
#define FILE_READ_ERR             -2
#define FILE_WRITE_ERR            -3
#define INVALID_METAFILE_ERR      -4
#define INVALID_SOCKET_ERR        -5
#define INVALID_TRACKER_URL_ERR   -6
#define INVALID_TRACKER_REPLY_ERR -7
#define INVALID_HASH_ERR          -8
#define INVALID_MESSAGE_ERR       -9
#define INVALID_PARAMETER_ERR     -10
#define FAILED_ALLOCATE_MEM_ERR   -11
#define NO_BUFFER_ERR             -12
#define READ_SOCKET_ERR           -13
#define WRITE_SOCKET_ERR          -14
#define RECEIVE_EXIT_SIGNAL_ERR   -15

void btexit(int errno, char *file, int line);

#endif
