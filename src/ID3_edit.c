#include "ID3_edit.h"
#include "ID3_view.h"

/* Editing 1st 6 tags of mp3file
 * Input : structures containing source file and user data 
 * Output: Creates the duplicate mp3 file and loads all the data from user and src file
 * Return value : SUCCESS / FAILURE based on each function mentioned in the edit_tags()
 */
Status edit_tags(File *file, EditTag *editTag)
{
    //open files
    nibble.flag2 = open_files(file, editTag);
    if(!nibble.flag2)   return FAILURE;

    //copy header contents
    copy_header(file->fptr_mp3, editTag->fptr_dup);

    for(int index = 0; index < 6; index++)
    {
        //copy frame ID
        copy_4_bytes(file->fptr_mp3, editTag->fptr_dup);

        //read frame data size
        int framedata_size = read_frameData_size(file->fptr_mp3);
        if(framedata_size <= 0) return FAILURE;

        if(strcmp(frame_ids[index],editTag->usr_tag)) //different
        {
            copy_4_bytes(file->fptr_mp3, editTag->fptr_dup);
        }
        else //same
        {
            //write
            nibble.flag3 = 1;
            fseek(file->fptr_mp3, 4, SEEK_CUR);
            nibble.flag2 =  write_framedata_size((strlen(editTag->usr_data) + 1), editTag->fptr_dup);
            if(!nibble.flag2)   return FAILURE;
        }

        //Copy flag bytes
        copy_3_bytes(file->fptr_mp3, editTag->fptr_dup);

        //copy frame data
        if(!nibble.flag3)
        {
            copy_frameData(framedata_size, file->fptr_mp3, editTag->fptr_dup);
        }
        else//write frame data
        {
            fwrite(editTag->usr_data, 1, strlen(editTag->usr_data), file->fptr_mp3);
            //fputc('\0', file->fptr_mp3);

            fseek(file->fptr_mp3, framedata_size, SEEK_CUR);
            break;
        }
    }
    
    //Copy remaining data
    copy_remaining_data(file->fptr_mp3, editTag->fptr_dup);

    return SUCCESS;
}


/* 
 * Get File pointer for i/p file and output file
 * Inputs: structures containing source file and duplicate file
 * Output: Opens the file in read and write mode and stores the file pointer
 * Return Value: SUCCESS / FAILURE based on file opening process
 */
Status open_files(File *file, EditTag *editTag)
{
    // Src mp3 file
    file->fptr_mp3 = fopen(file->file_name, "r");
    // Do Error handling
    if (file->fptr_mp3 == NULL)
    {
    	perror("fopen"); 
    	fprintf(stderr, "ERROR: Unable to open file %s\n", file->file_name);
 
    	return FAILURE;
    }

    //duplicate mp3 file
    editTag->fptr_dup = fopen(editTag->file_name, "w");

    return SUCCESS;
}

/* 
 * Copys header data
 * Inputs: file pointers of src and duplicate file
 * Output: copys 10 bytes of data (header contents)
 * Return Value: SUCCESS
 */
void copy_header(FILE *fptr_src, FILE *fptr_dup)
{
    char temp[10];

    fread(temp, 10, 1, fptr_src);

    fwrite(temp, 10, 1, fptr_dup);
}

/* 
 * Copys 4 bytes
 * Inputs: file pointers of src and duplicate file
 * Output: copys 4 bytes of data 
 * Return Value: -----
 */
void copy_4_bytes(FILE *fptr_src, FILE *fptr_dup)
{
    char temp[4];

    fread(temp, 4, 1, fptr_src);

    fwrite(temp, 4, 1, fptr_dup);
}

/* 
 * Copys 3 bytes
 * Inputs: file pointers of src and duplicate file
 * Output: copys 4 bytes of data 
 * Return Value: -----
 */
void copy_3_bytes(FILE *fptr_src, FILE *fptr_dup)
{
    char temp[3];

    fread(temp, 3, 1, fptr_src);

    fwrite(temp, 3, 1, fptr_dup);
}

/* Copying remaining file data 
 * Input : file poniters of src and duplicate files
 * Output: Copys remaining data from src to duplicate file
 * Return value : -------
 */
void copy_remaining_data(FILE *fptr_src, FILE *fptr_dup)
{
    char buffer[1];
    // Read and write one character at a time
    while (fread(buffer, 1, 1, fptr_src) > 0)
    {
        fwrite(buffer, 1, 1, fptr_dup);
    }
}

Status write_framedata_size(int size, FILE *fptr_dup) 
{
    unsigned char buffer[4];

    // Convert size to big-endian format
    buffer[0] = (size >> 24) & 0xFF;
    buffer[1] = (size >> 16) & 0xFF;
    buffer[2] = (size >> 8) & 0xFF;
    buffer[3] = size & 0xFF;

    // Write the size to file
    if (fwrite(buffer, 1, 4, fptr_dup) != 4) {
        printf("Error: Failed to write frame data size\n");
        return FAILURE;
    }
    printf("Written frame data size: %d\n", size);
    return SUCCESS;
}

void copy_frameData(int size, FILE *fptr_src, FILE *fptr_dup)
{
    char buffer[size];

    fread(buffer, 1, size, fptr_src);

    fwrite(buffer, 1, size, fptr_dup);
}