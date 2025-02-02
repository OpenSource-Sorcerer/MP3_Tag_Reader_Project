#ifndef ID3_VIEW_H
#define ID3_VIEW_H

#include "main.h"

typedef struct TAGDATA_MP3
{
    char *title;
    char *artist;
    char *album;
    char *year;
    char *genre;
    char *comment;
}TagData;


/* Open source file */
Status open_file(File *file);

/* Check file signature */
Status chk_sign(FILE *fptr_src);

/* Reading frame ID */
char* read_frameID(FILE *fptr_src);

/* Reading frame data size */
int read_frameData_size(FILE *fptr_src);

/* Change the buffer into integer value */
int get_size(char *buffer);

/* reading frame data */
void read_framedata(char* frame_ID, int framedata_size, FILE *fptr_src);

/* Assiging tag data into structure */
void assign_tagData(char *frame_data, char *frame_ID);
#endif