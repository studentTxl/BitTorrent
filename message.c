/*************************************************************************
    > File Name: message.c
    > Author: tongxianglong
    > Mail: studentTxl@163.com 
    > Created Time: Fri 02 Mar 2018 02:49:54 PM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>
#include<time.h>
#include<sys/socket.h>
#include"parse_metafile.h"
#include"bitfield.h"
#include"peer.h"
#include"policy.h"
#include"data.h"
#include"message.h"

#define HANDSHAKE     -2
#define KEEP_ALIVE    -1
#define CHOKE          0
#define UNCHOKE        1
#define INTERESTED     2
#define UNINTERESTED   3
#define HAVE           4
#define BITFIELD       5
#define REQUEST        6
#define PIECE          7
#define CANCEL         8
#define PORT           9

#define KEEP_ALIVE_TIME  45

extern Bitmap *bitmap;
extern char info_hash[20];
extern char peer_id[20];
extern int have_piece_index[64];
extern Peer *peer_head;

int int_to_char(int i, unsigned char c[4])
{
	c[3] = i%256;
	c[2] = (i-c[3])/256%256;
	c[1] = (i -c[3] -c[2]*256)/256/256%256;
	c[0] = (i -c[3] -c[2]*256 -c[1]*256*256) / 256 / 256 / 256 % 256;

	return 0;
}

int char_to_int(unsigned char c[4])
{
	int i;
	i = c[0]*256*256*256 + c[1]*256*256 + c[2]*256 + c[3];

	return i;
}

int create_handshake_msg(char *info_hash, char *peer_id, Peer *peer){
	int i;
	unsigned char keyword[20] = "BitTorrent protocol", c = 0x00;
	unsigned char *buffer = peer->out_msg + peer->msg_len;
	int len = MSG_SIZE - peer->msg_len;
	if(len < 68) return -1;

	buffer[0] = 19;
	for(i=0; i<19; ++i) buffer[i+1] = keyword[i];
	for(i=0; i<8; ++i)  buffer[i+20] = c;
	for(i=0; i<20;++i)  buffer[i+48] = info_hash[i];
	for(i=0; i<20; ++i) buffer[i+48] = peer_id[i];

	peer->msg_len += 68;
	return 0;
}

int create_keep_alive_msg(Peer *peer)
{
	unsigned char *buffer = peer->out_msg + peer->msg_len;
	int len = MSG_SIZE - peer->msg_len;

	if(len < 4) return -1;
	memset(buffer,0,4);
	peer->msg_len += 4;
	return 0;
}

int create_chock_interested_msg(int type, Peer *peer)
{
	unsigned char *buffer = peer->out_msg + peer->msg_len;
	int len = MSG_SIZE - peer->msg_len;

	if(len < 5) return -1;
	memset(buffer, 0, 5);
	buffer[3] = 1;
	buffer[4] = type;

	peer->msg_len +=5;
	return 0;
}

int create_have_msg(int index, Peer *peer)
{
	unsigned char *buffer = peer->out_msg + peer->msg_len;
	int len = MSG_SIZE - peer->msg_len;
	unsigned char c[4];

	if(len < 9) return -1;
	memset(buffer,0, 9);
	buffer[3] = 5;
	buffer[4] = 4;
	int_to_char(index, c);
	buffer[5] = c[0];
	buffer[6] = c[1];
	buffer[7] = c[2];
	buffer[8] = c[3];

	peer->msg_len += 9;
	return 0;
}


int create_bitfield_msg(char *bitfield, int bitfield_len, Peer *peer)
{
	int i;
	unsigned char c[4];
	unsigned char *buffer = peer->out_msg + peer->msg_len;
	int len = MSG_SIZE - peer->msg_len;

	if(len < bitfield_len+5)
	{
		printf("%s:%d buffer too small\n", __FILE__,__LINE__);
		return -1;
	}
	int_to_char(bitfield_len+1, c);
	for(i=0; i<4; ++i)
		buffer[i] = c[i];
	buffer[4] = 5;
	for(i=0; i<bitfield_len; ++i) buffer[i+5] = bitfield[i];

	peer->msg_len += bitfield_len+5;
	return 0;
}

int create_request_msg(int index,int begin,int length,Peer *peer)
{
	int i;
	unsigned char c[4];
	unsigned char *buffer = peer->out_msg + peer->msg_len;
	int len = MSG_SIZE -peer->msg_len;

	if(len < 17) return -1;
	memset(buffer, 0, 17);
	buffer[3] = 13;
	buffer[4] = 6;
	int_to_char(index, c);
	for(i=0; i<4; ++i) buffer[i+5] = c[i];
	int_to_char(begin, c);
	for(i=0; i<4; ++i) buffer[i+9] = c[i];
	int_to_char(length, c);
	for(i=0; i<4; ++i) buffer[i+13] = c[i];

	peer->msg_len += 17;
	return 0;
}

int create_piece_msg(int index, int begin, char *block, int b_len,Peer *peer)
{
	int i;
	unsigned char c[4];
	unsigned char *buffer = peer->out_msg + peer->msg_len;
	int len = MSG_SIZE - peer->msg_len;

	if(len< b)_len+13{
		printf("%s:%d buffer too small\n", __FILE__, __LINE__);
		return -1;
	}

	int_to_char(b_len+9, c);
	for(i=0; i<4; i++) buffer[i] = c[i];
	buffer[4] = 7;
	int_to_char(index, c);
	for(i=0; i<4; ++i) buffer[i+5] = c[i];
	int_to_char(begin,c);
	for(i=0; i<4; ++i) buffer[i+9] = c[i];
	for(i=0; i<b_len; ++i) buffer[i+13] = block[i];

	peer->msg_len += b_len+13;
	return 0;
}

int reate_cancel_msg(int index, int begin, int length, Peer *peer)
{
	int i;
	unsigned char c[4];
	unsigned char *buffer = peer->out_msg + peer->msg_len;
	int len = MSG_SIZE - peer->msg_len;

	if(len <17) return -1;
	memset(buffr , 0, 17);
	buffer[3] = 13;
	buffer[4] = 8;
	int_to_char(index, c);
	for(i=0; i<4; i++) buffer[i+5] = c[i];
	int_to_char(begin,c);
	for(i=0; i<4; ++i) buffer[i+9] = c[i];
	int_to_char(length,c);
	for(i=0; i<4; ++i) buffer[i+13] = c[i];

	peer->msg_len += 17;
	return 0;
}
