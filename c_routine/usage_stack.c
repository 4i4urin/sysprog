#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "stack.h"


char usage_stack(t_module_stack* ptr_stack);

int main(void)
{
    t_module_stack stack = init_stack();

    char quit_flag = 0;
    while ( !quit_flag )
    {
        quit_flag = usage_stack(&stack);
    }

}


char get_char(void);
t_employee* get_user_employee(t_employee* emp);
void show_employee(t_employee* ptr_item);
void show_all(t_module_stack* ptr_stack);
void show_menu(void);

char usage_stack(t_module_stack* ptr_stack)
{
    show_menu();

    int quit_flag = 0;
    char command = get_char();
    putchar('\n');

    t_employee tmp = {0, 0, 0};
    switch (command)
    {
    case 'a': // get user input and push
        if(get_user_employee(&tmp) == NULL)
            break;
        ptr_stack->push(&tmp, ptr_stack);
        printf("Push item to the stack\n");
        break;

    case 'p': // pop one item
        if (ptr_stack->pop(&tmp, ptr_stack) == NULL)
        {
            printf("ERROR: the stack is empty\n");
            break;
        }
        show_employee(&tmp);
        break;

    case 'l': // show all item
        show_all(ptr_stack);
        break;

    case 'n': // number of items in the stack
        printf("Number of stack item is %d\n", ptr_stack->size(ptr_stack));
        break;

    case 'c': // clear stack
        ptr_stack->clear(ptr_stack);
        printf("The Stack is clear\n");
        break;

    case 'q': // clear stack and quit from program
        ptr_stack->clear(ptr_stack);
        quit_flag = 1;
        printf("Bye\n");
        break;

    default:
        printf("Unknown command\n");
    }

    putchar('\n');
    return quit_flag;
}


void show_all(t_module_stack* ptr_stack)
{
    t_employee tmp = {(char*) malloc(1), 0, 0}; // hack to use realloc() in pop

    while (ptr_stack->pop(&tmp, ptr_stack) != NULL)
    {
        show_employee(&tmp);
    }
}


char get_char(void)
{
    char command = getchar();
    if (command == '\n') 
        return 0;

    while ( getchar() != '\n' ) { } // read everything till \n
    return tolower(command);
}


void show_employee(t_employee* ptr_item)
{
    putchar('\n');
    printf("Name: %s\n", ptr_item->name);
    printf("Status: ");
    (ptr_item->is_married) ? printf("married\n") : printf("single\n");
    printf("Salary: %.2lf$\n", ptr_item->salary);
    putchar('\n');
}


char* get_dynamic_string(char* ret);
char get_married_status(void);

t_employee* get_user_employee(t_employee* emp)
{
    printf("Enter employee name: ");
    emp->name = get_dynamic_string(emp->name);
    if (emp->name == NULL)
    {
        printf("Error input\n");
        return NULL;
    }

    printf("Is the employee married?(y/n) ");
    emp->is_married = get_married_status();

    printf("Enter employee salary: ");
    if (scanf("%lf", &emp->salary) != 1)
    {
        printf("Invalid number\n");
        return 0;
    }

    getchar();
    return emp;
}


char get_married_status(void)
{
    char status = get_char();
    if (tolower(status) == 'y')
        return 1;
    else 
        return 0;
}


char* get_dynamic_string(char* ret)
{
    // I don't know. Do i need to free memory if it maybe allocate before
    free(ret); // clear memory

    char ch = 0;
    int char_counter = 0;
    ret = (char*) malloc(sizeof(char));

    while ( (ch = getchar()) != '\n' && ch != EOF)
    {
        ret[ char_counter ] = ch;
        char_counter += 1;

        ret = (char*) realloc(ret, (char_counter  + 1) * sizeof(char));
    }

    if (char_counter == 0)
        return NULL;

    ret[ char_counter ] = '\0';

    return ret;
}


void show_menu(void)
{
    printf("Menu of stack options:\n");
    printf("a - add item to the stack\n");
    printf("p - pop item from the stack\n");
    printf("l - list all item of the stack (stack will be empty)\n");
    printf("n - show number of items in the stack (size of the stack will not change\n");
    printf("c - clear stack\n");
    printf("q - quit from program\n");
    printf("Print your command: ");
}
