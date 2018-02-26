/*************************************************************************
    > File Name: bitfield.h
    > Author: tongxianglong
    > Mail: studentTxl@163.com 
    > Created Time: Sun 25 Feb 2018 05:17:34 PM CST
 ************************************************************************/

#ifndef BITFIELD_H
#define BITFIELD_H

typedef struct Bitmap
{
	unsigned char *bitfield;
	int     bitfield_length;
	int     valid_length;
}Bitmap;

int create_bitfield();
int get_bit_value(Bitmap *bitmap, int index);
int set_bit_value(Bitmap *bitmap, int index, unsigned char value);
int all_zero(Bitmap *bitmap);
int all_set(Bitmap *bitmap);
void release_memory_in_bitfield();
int print_bitfield(Bitmap *bitmap);

int restore_bitmap();
int is_interested(Bitmap *dst, Bitmap *src);
int get_download_piece_num();

#endif


