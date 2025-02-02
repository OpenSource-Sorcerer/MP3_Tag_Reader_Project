#ifndef ID3_EDIT_H
#define ID3_EDIT_H

#include "main.h"

/* Opening the src and destination files */
Status open_files(File *file, EditTag *editTag);

/* Copying header data */
void copy_header(FILE *fptr_src, FILE *fptr_dup);

/* Copying 4 bytes of data */
void copy_4_bytes(FILE *fptr_src, FILE *fptr_dup);

/* Copying 3 bytes of data */
void copy_3_bytes(FILE *fptr_src, FILE *fptr_dup);

/* Copying remaining data */
void copy_remaining_data(FILE *fptr_src, FILE *fptr_dup);

/* writing frame data size */
Status write_framedata_size(int size, FILE *fptr_dup);

void copy_frameData(int size, FILE *fptr_src, FILE *fptr_dup);

/* 1st 6 Frame IDs */
const char *frame_ids[] = {"-t","-a","-A","-y","-g","-c"};

#endif