#include "ID3_view.h"

TagData *tags = NULL;  // Declare it globally, but initialize it later

/* tags structure initialization
 * Input : ---------
 * Output: initialize the tags struct
 * Return value : ------
 */
void init_tags() {
    tags = (TagData *)malloc(sizeof(TagData));
    if (!tags) {
        printf("Memory allocation failed for tags\n");
        exit(1); // Exit the program if memory allocation fails
    }
}

/* Prints 1st 6 tags of mp3file
 * Input : ---------
 * Output: print data to the screen
 * Return value : ------
 */
void view(void)
{
    print_line();
    printf("\n\033[1mMP3 Tag Reader and Editor for ID3v3\033[0m\n");
    print_line();
    printf("\n\033[1mTITLE          :           \033[1;37;42m %s \033[0m\n",tags->title);
    printf("\n\033[1mARTIST         :           \033[1;37;42m %s \033[0m\n",tags->artist);
    printf("\n\033[1mALBUM          :           \033[1;37;42m %s \033[0m\n",tags->album);
    printf("\n\033[1mYEAR           :           \033[1;37;42m %s \033[0m\n",tags->year);
    printf("\n\033[1mCONTENT TYPE   :           \033[1;37;42m %s \033[0m\n",tags->genre);
    printf("\n\033[1mCOMMENT        :           \033[1;37;42m %s \033[0m\n",tags->comment);
    print_line();
    printf("\n");

}

/* Viewing 1st 6 tags of mp3file
 * Input : structure containing source file
 * Output: Read the tags and print them to the screen
 * Return value : SUCCESS / FAILURE based on each function mentioned in the view_tags()
 */
Status view_tags(File *file)
{
    init_tags();
    //open file
    nibble.flag2 = open_file(file);
    if(!nibble.flag2)   return FAILURE;

    //check signature
    nibble.flag2 = chk_sign(file->fptr_mp3);
    if(!nibble.flag2)   return FAILURE;

    //skip header bytes
    fseek(file->fptr_mp3, 7, SEEK_CUR);

    for(int index = 0; index < 6; index++)
    {
        //Read Frame ID
        char *frame_ID = read_frameID(file->fptr_mp3);
        if(frame_ID == NULL) return FAILURE;

        //Read Frame Data size
        int framedata_size = read_frameData_size(file->fptr_mp3);
        if(framedata_size <= 0) return FAILURE;

        //skip flag bytes
        fseek(file->fptr_mp3, 3, SEEK_CUR);

        //read frame data
        read_framedata(frame_ID, framedata_size, file->fptr_mp3);
    }
    //viewing the tags
    view();

    fclose(file->fptr_mp3);
    return SUCCESS;
}

/* 
 * Get File pointer for i/p file
 * Inputs: structure containing source file
 * Output: Opens the file in read mode and stores the file pointer
 * Return Value: SUCCESS / FAILURE based on file opening process
 */
Status open_file(File *file)
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
    return SUCCESS;
}

/* 
 * Checks signature of the source file
 * Inputs: file pointer
 * Output: reads the 1st 3 bytes of data and checks the data
 * Return Value: SUCCESS / FAILURE based on the file data
 */
Status chk_sign(FILE *fptr_src)
{
    char temp[3];

    fread(temp, 3, 1, fptr_src);

    return (strcmp(temp, "ID3")) ? FAILURE : SUCCESS;
}

/* 
 * Read frame ID
 * Inputs: file pointer
 * Output: reads the 4 bytes of file data 
 * Return Value: string contains frame ID
 */
char* read_frameID(FILE *fptr_src)
{
    char *temp = malloc(4);

    fread(temp, 4, 1, fptr_src);

    return temp;
}

/* 
 * Read frame data size
 * Inputs: file pointer
 * Output: reads the 4 bytes of file data 
 * Return Value: integer value (frame data size)
 */
int read_frameData_size(FILE *fptr_src)
{
    char temp[4];

    fread(temp, 4, 1, fptr_src);

    return get_size(temp);
}

/* 
 * Change 4 byte data into integer
 * Inputs: string
 * Output: Size of the frame data
 * Return Value: integer value (frame data size)
 */
int get_size(char *buffer)
{
    return (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
}

/* 
 * Read frame data 
 * Inputs: frame_ID, size, file pointer
 * Output: reads the size bytes of file data and send them to another function
 * Return Value: ------
 */
void read_framedata(char* frame_ID, int framedata_size, FILE *fptr_src)
{
    char *frame_data = (char *)malloc(framedata_size);
    if (!frame_data) {
        printf("Memory allocation failed for frame data\n");
        return;
    }

    fread(frame_data, 1, framedata_size - 1, fptr_src);
    frame_data[framedata_size - 1] = '\0'; 

    assign_tagData(frame_data, frame_ID);
    
    free(frame_data); // Avoid memory leaks
}

/* 
 * Assign frame data to tag structure
 * Inputs: string(frame data), frameID
 * Output: Assigning the data into struct members for viewing
 * Return Value: -----"TPE1","TALB","TYER","TCON","COMM"
*/  
void assign_tagData(char *frame_data, char *frame_ID)
{
    if (!tags) return; // Ensure tags is initialized

    if (!(strcmp(frame_ID, "TIT2")))
        tags->title = strdup(frame_data);
    else if (!(strcmp(frame_ID, "TPE1")))
        tags->artist = strdup(frame_data);
    else if (!(strcmp(frame_ID, "TALB")))
        tags->album = strdup(frame_data);
    else if (!(strcmp(frame_ID, "TYER")))
        tags->year = strdup(frame_data);
    else if (!(strcmp(frame_ID, "TCON")))
        tags->genre = strdup(frame_data);
    else if (!(strcmp(frame_ID, "COMM")))
        tags->comment = strdup(frame_data);
}
