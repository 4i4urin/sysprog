#include "stack.h"

#include <stdlib.h>
#include <string.h>


static void push(const t_employee* const ptr_item, const void* const ptrv_self);
static t_employee* pop(t_employee* const ret_item, const void* const ptrv_self);
static int size(const void* const ptrv_self);



t_module_stack init_stack(t_copy_item copy, t_delete_item delete)
{
    return (t_module_stack){
        .push   = &push,
        .pop    = &pop,
        .size   = &size,
        .copy_item = copy,
        .delete_item = delete,
        .ptr_head = NULL
    };  
}


static void push(const t_employee* const ptr_item, const void* const ptrv_self)
{
    t_module_stack* self = (t_module_stack*) ptrv_self;

    t_stack_item* new_item = (t_stack_item*) malloc(sizeof(t_stack_item));
    if (new_item == NULL)
        exit(100);

    self->copy_item(&new_item->data, ptr_item);

    new_item->last = self->ptr_head;
    self->ptr_head = new_item;
}


static t_employee* pop(t_employee* const ret_item, const void* const ptrv_self)
{
    t_module_stack* self = (t_module_stack*) ptrv_self;

    if (self->ptr_head == NULL)
        return NULL;

    self->copy_item(ret_item, &self->ptr_head->data);

    t_stack_item* del_item = self->ptr_head;
    self->ptr_head = del_item->last;


    self->delete_item(&del_item->data);
    free(del_item);

    return ret_item;
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
