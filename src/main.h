#ifndef MAIN_H
#define MAIN_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/* User defined types */
typedef unsigned int uint;

typedef unsigned char uch;

/* Status will be used in fn. return type */
typedef enum
{
    FAILURE,
    SUCCESS
} Status;

/* Operation type will be used to validate cmd args */
typedef enum
{
    VIEW,
    EDIT,
    HELP,
    VERSION,
    UNSUPPORTED
} OperationType;

/* Using Bit Fields for flags */
struct NIBBLE
{
    uch flag1 : 2;
    uch flag2 : 1;
    uch flag3 : 1;
};

/* File structure used to store src file name and fptr */
typedef struct {
    FILE *fptr_mp3;    // Source MP3 file pointer
    char file_name[20];   // Source MP3 file name
} File;

/* Tag structure used to store user data*/
typedef struct {
    char usr_tag[5];   // User-specified tag
    char usr_data[100];// User-specified data
    char *file_name;    // Destination MP3 file name
    FILE *fptr_dup;    // Destination MP3 file pointer
} EditTag;

/* Initializing structure for flags */
struct NIBBLE nibble;

/* Initializing structure for View Tag - Source file */
File file;

/* Initializing structure for Edit tag - Duplicate file */
EditTag editTag;

/* Editing tags in mp3 file */
Status edit_tags(File *file, EditTag *editTag);

/* Viewing tags in mp3 file */
Status view_tags(File *file);
               
/* Prints Horizontal line */
void print_line();
#endif