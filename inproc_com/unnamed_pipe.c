#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define READ  0
#define WRITE 1

#define BUF_LEN 200

void parent_proc(int write_desc, int read_desc);
void child_proc(int write_desc, int read_desc);

void write_to_pipe(int pipe_desc, const char* str);
void read_from_pipe(int pipe_desc, char* read_buf, int read_buf_len);

char* get_dynamic_string(char* ret);
/*
The machines rose from the ashes of the nuclear fire. 
Their war to exterminate mankind has raged for decades, but the final battle would not be fought in the future. 
It would be fought here, in our present. 
Tonight.
*/


int main(void)
{
    int pipe_parent_child[2] = { 0 }, pipe_child_parent[2] = { 0 };

    if (pipe(pipe_parent_child))
    {
        printf("ERROR: can't create pipe parent to child\n");
        return -1;
    }
    if (pipe(pipe_child_parent))
    {
        printf("ERROR: can't create pipe child to parent\n");
        return -1;
    }

    pid_t pid = fork();
    if (pid < (pid_t) 0)
    {
        printf("ERROR: fork error\n");
        return -1;
    } else if (pid == (pid_t) 0) // child
    {// pipe_parent_child --> read; pipe_child_parent --> write
        close(pipe_parent_child[WRITE]);
        close(pipe_child_parent[READ]);
        child_proc(pipe_child_parent[WRITE], pipe_parent_child[READ]);
    } else // parent
    {// pipe_parent_child --> write; pipe_child_parent -->read
        close(pipe_parent_child[READ]);
        close(pipe_child_parent[WRITE]);
        parent_proc(pipe_parent_child[WRITE], pipe_child_parent[READ]);
    }
    return 0;
}

void parent_proc(int write_desc, int read_desc)
{
    char* user_input = NULL;
    char read_buf[BUF_LEN] = { 0 };
    while (1)
    {
        printf("PARENT WRITE: ");
        user_input = get_dynamic_string(user_input);
        if (user_input != NULL)
            write_to_pipe(write_desc, user_input);
        else 
            write_to_pipe(write_desc, " ");
        free(user_input);

        read_from_pipe(read_desc, read_buf, BUF_LEN);
        printf("PARENT READ: %s\n\n", read_buf);
    }
}


void child_proc(int write_desc, int read_desc)
{
    char* user_input = NULL;
    char read_buf[BUF_LEN] = { 0 };
    while (1)
    {
        read_from_pipe(read_desc, read_buf, BUF_LEN);
        printf("CHILD READ: %s\n\n", read_buf);
        
        printf("CHILD WRITE: ");
        user_input = get_dynamic_string(user_input);
        if (user_input != NULL)
            write_to_pipe(write_desc, user_input);
        else 
            write_to_pipe(write_desc, " ");
        free(user_input);
    }
}


void write_to_pipe(int pipe_desc, const char* str)
{
    write(pipe_desc, str, strlen(str));
}


void read_from_pipe(int pipe_desc, char* read_buf, int read_buf_len)
{
    int count = read(pipe_desc, read_buf, read_buf_len);
    read_buf[count] = '\0';
}


char* get_dynamic_string(char* ret)
{
    char ch = 0;
    int char_counter = 0;
    ret = (char*) malloc(sizeof(char));
    if (ret == NULL)
        return NULL;

    while ( (ch = getchar()) != '\n' )
    {
        if (ch == EOF)
        {
            clearerr(stdin);
            return NULL;
        }

        ret[ char_counter ] = ch;
        char_counter += 1;

        ret = (char*) realloc(ret, (char_counter  + 1) * sizeof(char));
        if (ret == NULL)
            return NULL;
    }

    if (char_counter == 0)
        return NULL;

    ret[ char_counter ] = '\0';

    return ret;
}
