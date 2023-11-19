#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "stack_consol_io.h"
#include "stack.h"

// get first lower char, read whole user input till \n
char get_char(void)
{
    char command = getchar();
    if (command == '\n') 
        return 0;

    while ( getchar() != '\n' ) { } // read everything till \n
    return tolower(command);
}

// Show employee information
void show_employee(t_employee* ptr_item)
{
    putchar('\n');
    printf("Name: %s\n", ptr_item->name);
    printf("Status: ");
    (ptr_item->is_married) ? printf("married\n") : printf("single\n");
    printf("Salary: %.2lf$\n", ptr_item->salary);
    putchar('\n');
}


static char* get_dynamic_string(char* ret);
static char get_married_status(void);
// read employee information from consol input
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

// just show menu
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
