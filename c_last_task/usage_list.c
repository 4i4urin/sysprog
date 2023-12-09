#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "linked_list.h"
#include "list_io_console.h"


char usage_list(t_module_list* ptr_list);
void copy_employee(void* dest, const void* const src);
void delete_emloyee(void* emp);

int main(void)
{
    t_module_list list = init_list(copy_employee, delete_emloyee);

    char quit_flag = 0;
    while ( !quit_flag )
    {
        quit_flag = usage_list(&list);
    }

    return 0;
}


void clear(const void* const ptrv_self);
void show_all(t_module_list* ptr_llist);

char usage_list(t_module_list* ptr_list)
{
    show_menu();

    int quit_flag = 0;
    char command = get_char();
    putchar('\n');

    t_employee tmp = {0, 0, (char*) malloc(1), 0, 0, 0};
    switch (command)
    {
    case 'a': // get user input and push
        if(get_user_employee(&tmp) == NULL)
            break;
        ptr_list->push_head(&tmp, ptr_list);
        printf("Push first item to the list\n");
        break;

    case 'A': // get user input and push
        if(get_user_employee(&tmp) == NULL)
            break;
        ptr_list->push_tail(&tmp, ptr_list);
        printf("Push last item to the list\n");
        break;

    case 'p': // pop one item
        if (ptr_list->pop_head(&tmp, ptr_list) == NULL)
        {
            printf("ERROR: the list is empty\n");
            break;
        }
        show_employee(&tmp);
        break;

    case 'P': // pop one item
        if (ptr_list->pop_tail(&tmp, ptr_list) == NULL)
        {
            printf("ERROR: the list is empty\n");
            break;
        }
        show_employee(&tmp);
        break;

    case 'g':
        if (ptr_list->take(&tmp, get_uint_number(), ptr_list) == NULL)
        {
            printf("ERROR: No such element\n");
            break;
        }
        show_employee(&tmp);
        break;

    case 'l': // show all item
        show_all(ptr_list);
        break;

    case 'n': // number of items in the list
        printf("Number of list item is %d\n", ptr_list->size(ptr_list));
        break;

    case 'c': // clear list
        clear(ptr_list);
        printf("The list is clear\n");
        break;

    case 'q': // clear list and quit from program
        clear(ptr_list);
        quit_flag = 1;
        printf("Bye\n");
        break;

    default:
        printf("Unknown command\n");
    }

    putchar('\n');
    return quit_flag;
}



void show_all(t_module_list* ptr_llist)
{
    t_employee tmp = { 0, 0, (char*) malloc(1), 0, 0, 0}; // hack to use realloc() in pop

    while (ptr_llist->pop_head(&tmp, ptr_llist) != NULL)
    {
        show_employee(&tmp);
    }
}


void clear(const void* const ptrv_self)
{
    t_module_list* self = (t_module_list*) ptrv_self;
    t_employee tmp = { 0, 0, (char*) malloc(1), 0, 0, 0};
    // hack to use realloc() in copy_employee()

    while ( self->pop_head(&tmp, self) != NULL) { }
}


void copy_employee(void* ptrv_dest, const void* const ptrv_src)
{
    t_employee* dest = (t_employee*)ptrv_dest;
    t_employee* src = (t_employee*)ptrv_src;
    dest->name = (char*) realloc(dest->name, sizeof(char) * (strlen(src->name) + 1));
    memcpy(dest->name, src->name, strlen(src->name) + 1);
    dest->is_married = src->is_married;
    dest->salary = src->salary;
    dest->age = src->age;
    dest->gender = src->gender;
    dest->id = src->id;
}


void delete_emloyee(void* ptrv_emp)
{
    t_employee* emp = (t_employee*) ptrv_emp;

    memset(emp->name, 0, strlen(emp->name) + 1);
    free(emp->name);

    emp->is_married = 0;
    emp->salary = 0;
    emp->age = 0;
    emp->gender = 0;
    emp->id = 0;
}
