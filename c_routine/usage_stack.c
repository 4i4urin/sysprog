#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "stack_consol_io.h"


char usage_stack(t_module_stack* ptr_stack);
void copy_employee(void* dest, const void* const src);
void delete_emloyee(void* emp);

int main(void)
{
    t_module_stack stack = init_stack(copy_employee, delete_emloyee);

    char quit_flag = 0;
    while ( !quit_flag )
    {
        quit_flag = usage_stack(&stack);
    }
    return 0;
}


void clear(const void* const ptrv_self);

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
        clear(ptr_stack);
        printf("The Stack is clear\n");
        break;

    case 'q': // clear stack and quit from program
        clear(ptr_stack);
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


void clear(const void* const ptrv_self)
{
    t_module_stack* self = (t_module_stack*) ptrv_self;
    t_employee tmp = { (char*) malloc(1), 0, 0};
    // hack to use realloc() in copy_employee()

    while ( self->pop(&tmp, self) != NULL) { }
}


void copy_employee(void* ptrv_dest, const void* const ptrv_src)
{
    t_employee* dest = (t_employee*)ptrv_dest;
    t_employee* src = (t_employee*)ptrv_src;

    dest->name = (char*) realloc(dest->name, sizeof(char) * strlen(src->name));
    memcpy(dest->name, src->name, strlen(src->name));
    dest->is_married = src->is_married;
    dest->salary = src->salary;
}


void delete_emloyee(void* ptrv_emp)
{
    t_employee* emp = (t_employee*) ptrv_emp;

    memset(emp->name, 0, strlen(emp->name));
    free(emp->name);

    emp->is_married = 0;
    emp->salary = 0;
}