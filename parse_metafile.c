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
			*position = i;
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

	if( find_keyword("13:announce-list", &i)==0 )
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

int add_an_announce(char *url)
{
	Announce_list *p = announce_list_head, *q;

	while( p != NULL )
	{
		if(strcmp(p->announce, url) == 0)
			break;
		p = p->next;
	}
	if(p != NULL)
		return 0;

	q = (Announce_list *)malloc(sizeof(Announce_list));
	strcpy(q->announce, url);
	q->next = NULL;

	p = announce_list_head;
	if(p == NULL)
	{
		announce_list_head = q;
		return 1;
	}
	while(p->next != NULL)
		p = p->next;
	p->next = q;
	return 1;
}

int is_multi_files()
{
	long i;
	if( find_keyword("5:files", &i)==1)
	{
		multi_file = 1;
		return 1;
	}

#ifdef DEBUG
	printf("is_multi_files:%d\n", multi_file);
#endif
	return 0;
}

int get_piece_length()
{
	long i;
	if( find_keyword("12:piece length", &i)==1 )
	{
		i = i + strlen("12:piece length");
		i++;
		while(metafile_content[i] != 'e')
		{
			piece_length = piece_length * 10 + (metafile_content[i] - '0');
			i++;
		}
	}
	else
	{
		return -1;
	}
#ifdef DEBUG
	printf("piece length: %d\n", piece_length);
#endif
	return 0;
}

int get_pieces()
{
	long i;
	if(find_keyword("6:pieces", &i) == 1)
	{
		i = i+8;
		while(metafile_content[i] != ':')
		{
			pieces_length = pieces_length * 10 + (metafile_content[i]-'0');
			i++;
		}
			i++;
			pieces = (char *)malloc(pieces_length+1);
			memcpy(pieces, &metafile_content[i], pieces_length);
			pieces[pieces_length] = '\0';
	}
	else
	{
		return -1;
	}
#ifdef DEBUG
	printf("get pieces ok\n");
#endif
	return 0;
}

int get_file_name()
{
	long i;
	int count = 0;
	if(find_keyword("4:name", &i) == 1)
	{
		i = i + 6;
		while(metafile_content[i] != ':')
		{
			count = count * 10 + (metafile_content[i] - '0');
			i++;
		}
		i++;
		file_name = (char *)malloc(count+1);
		memcpy(file_name, &metafile_content[i], count);
		file_name[count] = '\0';
	}
	else
	{
		return -1;
	}
#ifdef DEBUG
	printf("file_name:%s", file_name);
#endif
	return 0;
}

int get_file_length()
{
	long i;
	if(is_multi_files() == 1)
	{
		if(files_head == NULL)
			get_files_length_path();
		Files *p = files_head;
		while(p != NULL)
		{
			file_length += p->length;
			p = p->next;
		}
	}
	else
	{
		if(find_keyword("6:length", &i) == 1)
		{
			i = i + 8;
			i++;
			while(metafile_content[i] != 'e')
			{
				file_length = file_length * 10 + (metafile_content[i]-'0');
				i++;
			}
		}
	}
#ifdef DEBUG
	printf("file lenngth :%lld\n", file_length);
#endif
	return 0;
}

int get_files_length_path()
{
	long i;
	int length;
	int count;
	Files *node = NULL;
	Files *p = NULL;

	if(is_multi_files() != 1)
	{
		return 0;
	}

	for(i=0; i<filesize-8; i++)
	{
		if(memcmp(&metafile_content[i], "6:length", 8)==0)
		{
			i = i + 8;
			i++;
			length = 0;
			while(metafile_content[i] != 'e')
			{
				length = length * 10 + (metafile_content[i]-'0');
				i++;
			}
			node = (Files *)malloc(sizeof(Files));
			node->length = length;
			node->next = NULL;
			if(files_head == NULL)
				files_head = node;
			else
			{
 				p = files_head;
				while(p->next != NULL)
					p = p->next;
				p->next = node;
			}
		}
		if(memcmp(&metafile_content[i], "4:path", 6)==0)
		{
			i = i + 6;
			i++;
			count = 0;
			while(metafile_content[i] != ':')
			{
				count = count * 10 + (metafile_content[i] - '0');
				i++;
			}
			i++;
			p = files_head;
			while(p->next != NULL)
				p = p->next;
			memcpy(p->path, &metafile_content[i], count);
			*(p->path + count) = '\0';
		}
	}
	return 0;
}
