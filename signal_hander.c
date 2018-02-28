/*************************************************************************
    > File Name: signal_hander.c
    > Author: tongxianglong
    > Mail: studentTxl@163.com 
    > Created Time: Wed 28 Feb 2018 02:40:41 PM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include"bitfield.h"
#include"peer.h"
#include"data.h"
#include"tracker.h"
#include"torrent.h"
#include"signal_hander.h"

extern int download_piece_num;
extern int *fds;
extern int fds_len;
extern Peer *peer_head;

void do_clear_work()
{
	Peer *p = peer_head;
	while(p != NULL)
	{
		if(p->state != CLOSING)
			close(p->socket);
		p = p->next;
	}

	if(download_piece_num > 0)
	{
		restore_bitmap;
	}
	int i;
	for(i = 0; i < fds_len; ++i)
	{
		close(fds[i]);
	}

	release_memory_in_parse_meafile();
	release_memory_in_bitfield();
	release_memory_in_peer();
	release_memory_in_torrent();

	exit(0);
}

void process_signal(int signo)
{
	printf("please wait for clear operations\n");
	do_clear_work();
}

int set_signal_hander()
{
	if(signal(SIGPIPE, SIG_IGN)==SIG_ERR)
	{
		perror("can not catch signal:sigpipe\n");
		return -1;
	}

	if(signal(SIGINT, process_signal)==SIG_ERR)
	{
		perror("can not catch signal:sigpipe\n");
		return -1;
	}

	if(signal(SIGTERM, process_signal)==SIG_ERR)
	{
		perror("can not catch signal:sigpipe\n");
		return -1;
	}
	return 0;
}



