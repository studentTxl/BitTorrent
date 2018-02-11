/*************************************************************************
    > File Name: parse_metafile.c
    > Author: tongxianglong
    > Mail: studentTxl@163.com 
    > Created Time: Fri 09 Feb 2018 12:10:37 PM CST
 ************************************************************************/

#include<stdio.h>
#include<ctype.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"parse_metafile.h"
//#include"shal.h"

char *metafile_content = NULL;
long filesize;

int piece_length = 0;
char *pieces = NULL;
int pieces_length = 0;

int multi_file = 0;
char *file_name = NULL;
long long file_length = 0;
Files *files_head = NULL;

unsigned char info_hash[20];
unsigned char peer_id[20];

Announce_list *announce_list_head = NULL;

int read_metafile(char *metafile_name)
{
	long i;

	FILE *fp = fopen(metafile_name, "rb");
	if(fp == NULL)
	{
		printf("%s:%d can not open file!\n", __FILE__ , __LINE__);
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	if(filesize == -1)
	{
		printf("%s:%d fseek failed\n", __FILE__, __LINE__);
		return -1;
	}
	metafile_content = (char *)malloc(filesize + 1);
	if(metafile_content == NULL)
	{
		printf("%s:%d malloc failed\n", __FILE__, __LINE__);
		return -1;
	}
	fseek(fp, 0, SEEK_SET);
	for(i=0; i<filesize; ++i)
		metafile_content[i] = fgetc(fp);
	metafile_content[i] = 0;
	fclose(fp);
#ifdef DEBUG
	printf("metafile size is: %ld\n", filesize);
#endif
	return 0;
}

int find_keyword(char *keyword, long *position)
{
	long i;

	*position = -1;
	if(keyword == NULL)
		return 0;

	for(i=0; i<filesize-strlen(keyword); ++i)
	{
		if(memcmp(&metafile_content[i], keyword, strlen(keyword))==0)
		{
			*position = -1;
			return 1;
		}
	}
	return 0;
}

int read_announce_list()
{
	Announce_list *node = NULL;
	Announce_list *p = NULL;
	int len = 0;
	long i;

	if( find_keyword("13:announce-list", &i)==0)
	{
		if(find_keyword("8:announce", &i) == 1)
		{
			i = i + strlen("8:announce");
			while(isdigit(metafile_content[i]))
			{
				len = len*10 + (metafile_content[i] - '0');
				i++;
			}
			i++;

			node = (Announce_list *)malloc(sizeof(Announce_list));
			strncpy(node->announce, &metafile_content[i], len);
			node->announce[len] = '\0';
			node->next = NULL;
			announce_list_head = node;
		}
	}
	else
	{
		i = i + strlen("13:announce-list");
		i++;
		while(metafile_content[i] != 'e')
		{
			i++;
			while(isdigit(metafile_content[i]))
			{
				len = len*10 + (metafile_content[i] - '0');
				i++;
			}
			if(metafile_content[i] == ':')
				i++;
			else
				return -1;

			if(memcmp(&metafile_content[i], "http", 4)==0)
			{
				node = (Announce_list *)malloc(sizeof(Announce_list));
				strncpy(node->announce, &metafile_content[i], len);
				node->announce[len] = '\0';
				node->next = NULL;
				if(announce_list_head == NULL)
					announce_list_head = node;
				else
				{
					p = announce_list_head;
					while(p->next != NULL)
						p = p->next;
					p->next = node;
				}
			}
			i = i + len;
			len = 0;
			i++;
			if(i >= filesize)
				return -1;
		}
	}
#ifdef DEBUG
	p = announce_list_head;
	while(p != NULL)
	{
		printf("%s\n", p->announce);
		p = p->next;
	}
#endif
	return 0;
}






