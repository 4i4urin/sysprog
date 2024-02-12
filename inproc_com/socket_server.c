#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


#define SOCKET_ADDR "first_socket"


typedef char (*t_getchar_temp)(int sock_desc);

char* get_dynamic_string(char* ret, t_getchar_temp get_char, int sock_desc);

void read_from_socket(int sock_desc, char** socket_str);
void read_from_stdin(char** std_str);

char getchar_socket(int sock_desc);
char getchar_stdin();


int main(void)
{
    unlink(SOCKET_ADDR);

    int sock_desc = 0;
    if ( (sock_desc = socket(AF_UNIX, SOCK_STREAM, 0)) < 0 )
    {
        printf("ERROR: can't create socket\n");
        return -1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0 , sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_ADDR, sizeof(addr.sun_path) - 1);

    if ( bind(sock_desc, (struct sockaddr*)&addr, sizeof(struct sockaddr)) < 0 )
    {
        printf("ERROR: can't bind socket\n");
        return -1;
    }

    if ( listen(sock_desc, 5) < 0 )
    {
        printf("ERROR: can't listen\n");
        return -1;
    }


    int new_desc = 0;
    char* sock_str = NULL;
    char* stdin_str = NULL;

    if ( (new_desc = accept(sock_desc, NULL, NULL)) < 0 )
    {
        printf("ERROR: accept\n");
        return -1;
    }
    while (1)
    {
        read_from_socket(new_desc, &sock_str);
        printf("READ: %s", sock_str);
        printf("WRITE: ");
        read_from_stdin(&stdin_str);
        write(new_desc, stdin_str, strlen(stdin_str));
    }
    close(new_desc);

    close(sock_desc);
    
    return 0;
}


void read_from_socket(int sock_desc, char** socket_str)
{
    *socket_str = get_dynamic_string(*socket_str, getchar_socket, sock_desc);
    if (*socket_str == NULL)
    {
        printf("ERROR: allocate memory\n");
        exit(100);
    }
}


void read_from_stdin(char** std_str)
{
    *std_str = get_dynamic_string(*std_str, getchar_stdin, 0);
    if (*std_str == NULL)
    {
        printf("ERROR: allocate memory\n");
        exit(100);
    }
}


char* get_dynamic_string(char* ret, t_getchar_temp get_char, int pipe_desc)
{
    char ch = 0;
    int char_counter = 0;
    ret = (char*) malloc(sizeof(char));
    if (ret == NULL)
        return NULL;

    while ( (ch = get_char(pipe_desc)) != '\n' )
    {
        ret[ char_counter ] = ch;
        char_counter += 1;

        ret = (char*) realloc(ret, (char_counter  + 1) * sizeof(char));
        if (ret == NULL)
            return NULL;
    }
    
    if (ch == '\n' && char_counter == 0)
        ret = (char*) realloc(ret, 2 * sizeof(char));
    else
        ret = (char*) realloc(ret, (char_counter  + 1) * sizeof(char));

    ret[ char_counter ] = '\n';
    ret[ char_counter + 1] = '\0';

    return ret;
}


char getchar_socket(int sock_desc)
{
    char ch[1] = { 0 };
    if (read(sock_desc, ch, 1) < 0)
    {
        printf("ERROR: read from socket\n");
        exit(-1);
    }
    return (char)ch[0];
}


char getchar_stdin()
{
    return (char)getchar();
}
