#include "stack.h"

#include <stdlib.h>
#include <string.h>


static void push(const t_employee* const ptr_item, const void* const ptrv_self);
static t_employee* pop(t_employee* const ret_item, const void* const ptrv_self);
static void clear(const void* const ptrv_self);
static int size(const void* const ptrv_self);
static void delete_item(t_stack_item* const del_item);


t_module_stack init_stack(void)
{
    return (t_module_stack){
        .push   = &push,
        .pop    = &pop,
        .clear  = &clear,
        .size   = &size,
        .ptr_head = NULL
    };  
}


static void push(const t_employee* const ptr_item, const void* const ptrv_self)
{
    t_module_stack* self = (t_module_stack*) ptrv_self;

    t_stack_item* new_item = (t_stack_item*) malloc(sizeof(t_stack_item));
    if (new_item == NULL)
        exit(100);

    new_item->data.name = (char*) malloc(sizeof(char) * strlen(ptr_item->name));
    memcpy(new_item->data.name, ptr_item->name, strlen(ptr_item->name));
    new_item->data.is_married = ptr_item->is_married;
    new_item->data.salary = ptr_item->salary;

    new_item->last = self->ptr_head;
    self->ptr_head = new_item;
}


static t_employee* pop(t_employee* const ret_item, const void* const ptrv_self)
{
    t_module_stack* self = (t_module_stack*) ptrv_self;

    if (self->ptr_head == NULL)
        return NULL;

    // free(ret_item->name);
    ret_item->name = (char*) realloc(ret_item->name, sizeof(char) * strlen(self->ptr_head->data.name));
    memcpy(ret_item->name, self->ptr_head->data.name, strlen(self->ptr_head->data.name));
    ret_item->is_married = self->ptr_head->data.is_married;
    ret_item->salary = self->ptr_head->data.salary;

    t_stack_item* del_item = self->ptr_head;
    self->ptr_head = del_item->last;

    delete_item(del_item);

    return ret_item;
}


static void clear(const void* const ptrv_self)
{
    t_module_stack* self = (t_module_stack*) ptrv_self;
    t_employee tmp = { (char*) malloc(1), 0, 0};
    // hack to use realloc() in pop

    while ( self->pop(&tmp, self) != NULL) { }
}


static int size(const void* const ptrv_self)
{
    t_module_stack* self = (t_module_stack*) ptrv_self;
    t_stack_item* ptr_item = self->ptr_head;

    int size = 0;
    while (ptr_item != NULL)
    {
        size += 1;
        ptr_item = (t_stack_item*)ptr_item->last;
    }
    return size;
}


static void delete_item(t_stack_item* const del_item)
{
    // delete name
    memset(del_item->data.name, 0, strlen(del_item->data.name));
    free(del_item->data.name);

    del_item->data.is_married = 0;
    del_item->data.salary = 0;
    free(del_item);
}
