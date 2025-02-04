#include "ID3_edit.h"
#include "ID3_view.h"

/* Editing 1st 6 tags of mp3file
 * Input : structures containing source file and user data 
 * Output: Creates the duplicate mp3 file and loads all the data from user and src file
 * Return value : SUCCESS / FAILURE based on each function mentioned in the edit_tags()
 */
Status edit_tags(File *file, EditTag *editTag)
{
    int chkpos1 = 0, chkpos2 = 0;
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
            fseek(file->fptr_mp3, -4, SEEK_CUR);
            copy_4_bytes(file->fptr_mp3, editTag->fptr_dup);
        }
        else //same
        {
            nibble.flag3 = 1;
         
            nibble.flag2 =  write_framedata_size((strlen(editTag->usr_data) + 1), editTag->fptr_dup);
            if(!nibble.flag2)   return FAILURE;
        }

        //Copy flag bytes
        copy_3_bytes(file->fptr_mp3, editTag->fptr_dup);

        //copy frame data
        if(!nibble.flag3)
        {
            copy_frameData((framedata_size - 1), file->fptr_mp3, editTag->fptr_dup);
        }
        else//write frame data
        {
            fwrite(editTag->usr_data, 1, strlen(editTag->usr_data) , editTag->fptr_dup);

            fseek(file->fptr_mp3, framedata_size-1, SEEK_CUR);
            break;
        }
    }
    
    //Copy remaining data
    copy_remaining_data(file->fptr_mp3, editTag->fptr_dup);

    //removed old file and renamed the duplicate file
    remove(file->file_name);  
    rename(editTag->file_name, file->file_name); 

    fclose(editTag->fptr_dup);
     
    char *tag_name = print_tag(frame_ids, editTag->usr_tag);
    printf("\n\033[1mSelected Tag  :   \033[1;37;44m %s \033[0m\n",tag_name);
    printf("\n\033[1mTag Data      :   \033[1;37;44m %s \033[0m\n",editTag->usr_data);
    printf("------------------------------------------------------------");
    printf("\n                \033[1;37;43m DATA MODIFICATION COMPLETED \033[0m\n");
    printf("------------------------------------------------------------\n");
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
    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fptr_src)) > 0)
        fwrite(buffer, 1, bytes_read, fptr_dup);
}

/* Writing User data's size
 * Input : file poniters duplicate files, int value
 * Output: writing the integer value into duplicate fil
 * Return value : FAILURE / SUCCESS based on the fwrite function
 */
Status write_framedata_size(int size, FILE *fptr_dup) 
{
    unsigned char buffer[4];

    // Convert size to big-endian format
    buffer[0] = (size >> 24) & 0xFF;
    buffer[1] = (size >> 16) & 0xFF;
    buffer[2] = (size >> 8) & 0xFF;
    buffer[3] = size & 0xFF;

    // Write the size to file
    if (fwrite(buffer, 1, 4, fptr_dup) != 4) 
    {
        printf("Error: Failed to write frame data size\n");
        return FAILURE;
    }
    return SUCCESS;
}

/* Copying frame data
 * Input : file poniters of src and duplicate files, int value
 * Output: copying the data based on the integer value from src into duplicate file
 * Return value : -------
 */
void copy_frameData(int size, FILE *fptr_src, FILE *fptr_dup)
{
    char *buffer = malloc(size);  // Dynamically allocate memory for the buffer
    if (buffer == NULL) 
    {
        printf("Memory allocation failed\n");
        return;
    }

    fread(buffer, 1, size, fptr_src);
    fwrite(buffer, 1, size, fptr_dup);

    free(buffer);  
}

/* Printing the tag name
 * Input : frame id string, user tag
 * Output: compares the frame_ids elements with user tag
 * Return value : SUitable tag name
 */
char* print_tag(const char *frame_ids[], char *tag)
{
    if(strcmp(frame_ids[0], tag))   return  "title";

    else if(strcmp(frame_ids[1], tag))   return  "artist";

    else if(strcmp(frame_ids[2], tag))   return  "album";

    else if(strcmp(frame_ids[3], tag))   return  "year";

    else if(strcmp(frame_ids[4], tag))   return  "content_type";

    else if(strcmp(frame_ids[5], tag))   return  "comment";

    else return NULL;
}