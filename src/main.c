#include "main.h"

/* 
 * Prints line on screen
 * Inputs: ----
 * Output: Prints line on output screen
 * Return Value: ----
 */
void print_line()
{
    for(int i = 100; i--;)   printf("-");
}

/* 
 * Prints Error Message
 * Inputs: ----
 * Output: Prints Error Message on output screen
 * Return Value: ----
 */
void print_err()
{
    print_line();
    printf("\n\033[1mERROR  : ./a.out :   \033[0m\033[1;37;41m INVALID ARGUMENTS \033[0m\n");
    printf("\n\033[1mUSAGE  :\033[0m\n");
    printf("\n\033[1mTo view tags please pass like          :   \033[0m\033[1;37;43m ./a.out -v <mp3file> \033[0m\n");
    printf("\n\033[1mTo edit tags please pass like          :   \033[0m\033[1;37;43m ./a.out -e -t/-a/-A/-y/-g/-c <mp3file> \033[0m\n");
    printf("\n\033[1mTo view help options please pass like  :   \033[0m\033[1;37;43m ./a.out --help \033[0m\n");
    printf("\n\033[1mTo view version please pass like       :   \033[0m\033[1;37;43m ./a.out -v \033[0m\n");
    print_line();printf("\n");
}

/* 
 * Prints Options 
 * Inputs: ----
 * Output: Prints options to see or edit file data
 * Return Value: ----
 */
void print_help()
{
    print_line();
    printf("\n\033[1mUSAGE  :   ./a.out -[taAygc] ""value"" <mp3file>\n          ./a.out -v\033[0m\n");
    printf("\n\033[1m -t      Modifies a Title tag\n -a     Modifies a Artist tag\n -A    Modifies a Album tag\033[0m\n");
    printf("\n\033[1m -y      Modifies a Year tag\n -g     Modifies a Genre tag\n -c    Modifies a Comment tag\033[0m\n");
    printf("\n\033[1m -v      Prints version info\n --help Modifies a Artist tag\033[0m\n");
    print_line();
}

/* 
 * Validate CMD arguments count
 * Inputs: count
 * Output: Checks that required number of args are present or not
 * Return Value: FAILURE / SUCCESS, based on cmd args count
 */
Status validate_argc(int count)
{
    if(count < 2 || count > 5)   return FAILURE;

    else return SUCCESS;
}

/* 
 * Checks CMD args
 * Inputs: cmd args
 * Output: Checks the operation type for the further process
 * Return Value: operation type based on argv[1]
 */
OperationType chk_operation_type(char *argv[])
{
    if(!(strcmp(argv[1],"-v"))) //view or version
    {
        if(argv[2] == NULL) return VERSION;

        else if(strstr(argv[2],".mp3"))     return VIEW;
    }
    else if(!(strcmp(argv[1],"--help")))    return HELP;    //help
    
    else if(!(strcmp(argv[1],"-e")))    //edit
    {
        if((!(strcmp(argv[2],"-t"))) || (!(strcmp(argv[2],"-a"))) || (!(strcmp(argv[2],"-A"))) || (!(strcmp(argv[2],"-y"))) || (!(strcmp(argv[2],"-g"))) || (!(strcmp(argv[2],"-c"))))
        {
            if(argv[4] != NULL)
                if(strstr(argv[4],".mp3"))  return EDIT;
        }
    } 
    return UNSUPPORTED;
}

/* 
 * Main function
 * Inputs: Command Line Arguments
 * Output: Execution starts here
 * Return Value: 0
 */
int main(int argc, char *argv[])
{
    nibble.flag1 = validate_argc(argc);
    if(!nibble.flag1)   goto print_error;

    switch(chk_operation_type(argv))
    {
        case 0: //view

            strcpy((file.file_name),argv[2]);
            nibble.flag1 = view_tags(&file);
            if(!nibble.flag1)   goto print_error;
            break;

        case 1: //edit
            strcpy(file.file_name, argv[4]);
            editTag.file_name = "ToBeRemoved.mp3";
            strcpy(editTag.usr_data,argv[3]);
            strcpy(editTag.usr_tag,argv[2]);
            nibble.flag1 = edit_tags(&file, &editTag);
            if(!nibble.flag1)   goto print_error;
            break;

        case 2: //help
            print_help();
            break;

        case 3: //version
            print_line();
            printf("\n\033[1m                               VERSION          :           \033[1;37;42m ID3v3 \033[0m\n");
            print_line();
            printf("\n");
            break;

        default:    goto print_error;   //unsupported
    }
    
    return 0;

    print_error:
        print_err();
        return 0;
}