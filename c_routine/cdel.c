#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>


#define STRING_MAX 2000
#define ARG_NUM_MIN 3
#define CREATE_ARGC_CMD "-c"
#define DELETE_ARGC_CMD "-d"


typedef enum _t_action
{
    E_ERROR = -1,
    E_DELETE = 0x0,
    E_CREATE,
} t_action;


t_action get_action(const char* const flag);
void exit_with_inf(char str[]);
char* get_absolut_path(char* path_str, char* current_dir, const char* user_input);


void create_file(char* path_file);
void remove_file(char* path_file);
// input:   cdel -c /path/to/filename   (create)
//          cdel -d /path/to/filename   (delete)
// Условно бескончное количество путей
// tupe of paths
//    / - absolut
//    ~ - home dir
//    . or nothing - current dir
//    .. - one dir high

int main(int argc, char const *argv[])
{
    if (argc < ARG_NUM_MIN)
        exit_with_inf("ERROR ARGUMENT");

    t_action action = get_action(argv[1]);
    if (action == E_ERROR)
        exit_with_inf("UNKNOWN FLAG");

    char current_dir[STRING_MAX];
    if (getcwd(current_dir, sizeof(current_dir)) == NULL)
        exit_with_inf("ERROR: getcwd function");

    for (int i = 2; i < argc; i++)
    {
        char* path_str = (char*) malloc(1);
        char* path_file = get_absolut_path(path_str, current_dir, argv[i]);
        if (path_file == NULL)
        {
            printf("UNEXPECTED ERROR: dyring processing %s\n", argv[i]);
            free(path_str);
            continue;
        }

        if (action == E_CREATE)
            create_file(path_file);
        else
            remove_file(path_file);

        free((void*)path_file);
    }

    return 0;
}


void remove_file(char* path_file)
{
    if (remove(path_file) != -1)
        printf("Delete file %s\n", path_file);
    else
        printf("ERROR cannot delete: %s\n", path_file);
}


void create_file(char* path_file)
{
    FILE* file = fopen(path_file, "a");
    if (file != NULL)
        printf("Create file: %s\n", path_file);
    else
    {
        printf("ERROR cannot create: %s\n", path_file);
        return;
    }

    fclose(file);
}


int home_dir_len(char* current_dir);

char* get_absolut_path(char* path_str, char* current_dir, const char* user_input)
{
    if (user_input[0] == '/')
    { // absolut path
        path_str = (char*)realloc(path_str, strlen(user_input));
        memcpy(path_str, user_input, strlen(user_input));
    } else if (user_input[0] == '~' && user_input[1] == '/')
    { // from home
        path_str = (char*) realloc(path_str, strlen(user_input) - 2 + home_dir_len(current_dir));
        memcpy(path_str, current_dir, home_dir_len(current_dir)); // /home/shisel/
        memcpy(path_str + home_dir_len(current_dir), user_input + 2, strlen(user_input) - 2); // /home/shishel/some/
    } else if (user_input[0] == '.' && user_input[1] == '/')
    {// from curretn dir
        path_str = (char*)realloc(path_str, strlen(current_dir) + strlen(user_input) - 2);
        memcpy(path_str, current_dir, strlen(current_dir)); // /home/shishel/ without "\0"
        memcpy(path_str + strlen(current_dir), user_input + 1, strlen(user_input));
    } else if ( isalpha(user_input[0]) || (user_input[0] == '.' && user_input[1] == '.') )
    {// from curretn dir
        path_str = (char*)realloc(path_str, strlen(current_dir) + strlen(user_input) + 1);
        memcpy(path_str, current_dir, strlen(current_dir)); // /home/shishel/ without "\0"
        path_str[strlen(current_dir)] = '/';
        memcpy(path_str + strlen(current_dir) + 1, user_input, strlen(user_input));
        path_str[strlen(current_dir) + strlen(user_input) + 1] = '\0';
    } else 
        return NULL;
    return path_str;
}

// count current dir till the third '/'
int home_dir_len(char* current_dir)
{
    int indx = 0;
    int slash_count = 0;
    while (slash_count != 3)
    {
        if (current_dir[indx] == '/')
            slash_count += 1;
        indx += 1;
    }
    return indx;
}


t_action get_action(const char* const flag)
{
    if (!strcmp(flag, CREATE_ARGC_CMD))
        return E_CREATE;
    else if (!strcmp(flag, DELETE_ARGC_CMD))
        return E_DELETE;
    else 
        return E_ERROR;
}


void exit_with_inf(char str[])
{
    printf("%s\n", str);
    exit(100);
}
