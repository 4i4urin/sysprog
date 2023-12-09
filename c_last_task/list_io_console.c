#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

#include "linked_list.h"
#include "list_io_console.h"

void clear_input(void);

// get first lower char, read whole user input till \n
char get_char(void)
{
    char command = getchar();
    if (command == '\n') 
        return 0;

    clear_input();
    return command;
}

// Show employee information
void show_employee(t_employee* ptr_item)
{
    putchar('\n');
    printf("ID: %u\n", ptr_item->id);
    printf("Name: %s\n", ptr_item->name);
    printf("Age: %d\n", ptr_item->age);

    printf("Sex: ");
    if (ptr_item->gender == 'm')
        printf("male\n");
    else if (ptr_item->gender == 'f')
        printf("female\n");
    else
        printf("unknown\n");

    printf("Status: ");
    (ptr_item->is_married) ? printf("married\n") : printf("single\n");
    printf("Salary: %.2lf$\n", ptr_item->salary);

    putchar('\n');
}


static char* get_dynamic_string(char* ret);
static char get_married_status(void);
static char get_user_gender(void);
// read employee information from consol input
t_employee* get_user_employee(t_employee* emp)
{
    printf("Enter employee name: ");
    emp->name = get_dynamic_string(emp->name);
    if (emp->name == NULL)
    {
        printf("Error input\n");
        clear_input();
        return NULL;
    }
    printf("Enter employee's age: ");
    if (scanf("%u", &emp->age) != 1)
    {
        printf("Invalid number\n");
        clear_input();
        return NULL;
    }
    clear_input();
    printf("Enter employee gender (Male/Female): ");
    emp->gender = get_user_gender();

    printf("Is the employee married?(y/n) ");
    emp->is_married = get_married_status();

    printf("Enter employee salary: ");
    if (scanf("%lf", &emp->salary) != 1)
    {
        printf("Invalid number\n");
        clear_input();
        return NULL;
    }
    clear_input();
    return emp;
}

// read maried status of employee from consol
static char get_married_status(void)
{
    char status = get_char();
    if (tolower(status) == 'y')
        return 1;
    else 
        return 0;
}

// read employee name from consol
static char* get_dynamic_string(char* ret)
{
    // I don't know. Do i need to free memory if it maybe allocate before
    free(ret); // clear memory

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


static char get_user_gender(void)
{
    char gender = tolower(get_char());

    if (gender != 'm' || gender != 'f')
        return 0;

    return gender;
}

// just show menu
void show_menu(void)
{
    printf("Menu of list options:\n");
    printf("a - add FIRST item to the list\n");
    printf("A - add LAST item to the list\n");
    printf("p - pop FIRST item from the list\n");
    printf("P - pop LAST item from the list\n");
    printf("g - pop item by ID\n");
    printf("l - list all item from the list (list will be empty)\n");
    printf("n - show number of items in the list (size of the list will not change)\n");
    printf("c - clear list\n");
    printf("q - quit from program\n");
    printf("Print your command: ");
}


void clear_input(void)
{
    char ch = 0;
    while ((ch = getchar()) != '\n') 
    {
        if (ch == EOF)
        {
            clearerr(stdin);
            break;
        }
    }
}


unsigned int get_uint_number(void)
{
    printf("Enter ID of user: ");
    unsigned int ret = 0;
    if (scanf("%u", &ret) != 1)
    {
        printf("Invalid number\n");
        clear_input();
        return UINT_MAX;
    }
    clear_input();
    return ret;
}
