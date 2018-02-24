/*************************************************************************
    > File Name: parse_metafile.h
    > Author: tongxianglong
    > Mail: studentTxl@163.com 
    > Created Time: Fri 09 Feb 2018 11:43:03 AM CST
 ************************************************************************/

#ifndef PARSE_METAFILE
#define PARSE_METAFILE

//serve the URL of tracker
typedef struct _Announce_list
{
	char announce[128];
	struct _Announce_list *next;
}Announce_list;

//server the length and rounter
typedef struct Files
{
	char path[256];
	long length;
	struct Files *next;
}Files;

int read_metafile(char *metafile_name);
int find_keyword(char *keyword, long *position);
int read_announce_list();
int add_an_announce(char* url);

int get_piece_length();
int get_pieces();

int is_multi_files();
int get_file_name();
int get_file_length();
int get_files_length_path();
int get_info_hash();
int get_peer_id();

void release_memory_in_parse_metafile();
int parse_metafile(char *metafile);

#endif

