/*************************************************************************
    > File Name: btMain.c
    > Author: tongxianglong
    > Mail: studentTxl@163.com 
    > Created Time: Fri 09 Feb 2018 11:39:57 AM CST
 ************************************************************************/

#include<stdio.h>
#include"parse_metafile.h"
#include"bitfield.h"

extern Bitmap * bitmap;

int main()
{
/*	printf("hello\n");
	read_metafile("2018.torrent");
	read_announce_list();
	is_multi_files();
	get_piece_length();
	get_file_name();
	get_info_hash();*/
	int ret = parse_metafile("2018.torrent");
	printf("%d\n", ret);

	create_bitfield();
	print_bitfield(bitmap);
	return 0;
}
