#include "linked_list.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>


static void push_tail(const t_employee* const ptr_item, const void* const ptrv_self);
static void push_head(const t_employee* const ptr_item, const void* const ptrv_self);
static t_employee* pop_tail(t_employee* const ret_item, const void* const ptrv_self);
static t_employee* pop_head(t_employee* const ret_item, const void* const ptrv_self);
static int size(const void* const ptrv_self);
static t_employee* take(t_employee* const ret_item, const unsigned int id, const void* const ptrv_self);


t_module_list init_list(t_copy_item copy, t_delete_item delete)
{
    return (t_module_list){
        .push_tail  = &push_tail,
        .push_head  = &push_head,
        .pop_tail   = &pop_tail,
        .pop_head   = &pop_head,

        .size   = &size,
        .copy_item = copy,
        .delete_item = delete,

        .ptr_head = NULL,
        .ptr_tail = NULL,

        .take = take,
        .id = 0
    };  
}


static void push_tail(const t_employee* const ptr_item, const void* const ptrv_self)
{
    t_module_list* self = (t_module_list*) ptrv_self;

    t_list_item* new_item = (t_list_item*) malloc(sizeof(t_list_item));
    if (new_item == NULL)
        exit(100);

    self->copy_item(&new_item->data, ptr_item);
    new_item->data.id = (++(self->id));

    // when add first elemenr
    if (self->ptr_tail == NULL && self->ptr_head == NULL)
        self->ptr_head = new_item;

    // when add NOT the first element
    if (self->ptr_tail != NULL)
        self->ptr_tail->next = new_item;

    new_item->prev = self->ptr_tail;
    self->ptr_tail = new_item;
    new_item->next = NULL;
}


static void push_head(const t_employee* const ptr_item, const void* const ptrv_self)
{
    t_module_list* self = (t_module_list*) ptrv_self;

    t_list_item* new_item = (t_list_item*) malloc(sizeof(t_list_item));
    if (new_item == NULL)
        exit(100);

    self->copy_item(&new_item->data, ptr_item);
    new_item->data.id = (++(self->id));

    // when add the first element
    if (self->ptr_tail == NULL && self->ptr_head == NULL)
        self->ptr_tail = new_item;

    // when add NOT the first element
    if (self->ptr_head != NULL)
        self->ptr_head->prev = new_item;

    new_item->next = self->ptr_head;
    self->ptr_head = new_item;
    new_item->prev = NULL;
}


static t_employee* pop_tail(t_employee* const ret_item, const void* const ptrv_self)
{
    t_module_list* self = (t_module_list*) ptrv_self;

    if (self->ptr_tail == NULL)
        return NULL;

    self->copy_item(ret_item, &self->ptr_tail->data);

    t_list_item* del_item = self->ptr_tail;

    // pop last elemnt
    if (self->ptr_tail == self->ptr_head)
        self->ptr_head = NULL;

    self->ptr_tail = (t_list_item*)del_item->prev;
    if (self->ptr_tail != NULL)
        self->ptr_tail->next = NULL;
    
    self->delete_item(&del_item->data);
    del_item->next = NULL;
    del_item->prev = NULL;
    free(del_item);
    
    return ret_item;
}


static t_employee* pop_head(t_employee* const ret_item, const void* const ptrv_self)
{
    t_module_list* self = (t_module_list*) ptrv_self;

    if (self->ptr_head == NULL)
        return NULL;

    self->copy_item(ret_item, &self->ptr_head->data);

    t_list_item* del_item = self->ptr_head;

    // pop last elemnt
    if (self->ptr_tail == self->ptr_head)
        self->ptr_tail = NULL;

    self->ptr_head = (t_list_item*)del_item->next;
    if (self->ptr_head != NULL)
        self->ptr_head->prev = NULL;

    self->delete_item(&del_item->data);
    del_item->next = NULL;
    del_item->prev = NULL;
    free(del_item);

    return ret_item;
}


static int size(const void* const ptrv_self)
{
    t_module_list* self = (t_module_list*) ptrv_self;
    t_list_item* ptr_item = self->ptr_head;

    int size = 0;
    while (ptr_item != NULL)
    {
        size += 1;
        ptr_item = (t_list_item*)ptr_item->next;
    }
    return size;
}

static t_employee* take(t_employee* const ret_item, const unsigned int id, const void* const ptrv_self)
{
    t_module_list* self = (t_module_list*) ptrv_self;
    t_list_item* ptr_item = self->ptr_head;

    while (ptr_item != NULL)
    {
        if (ptr_item->data.id != id)
        {
            ptr_item = (t_list_item*)ptr_item->next;
            continue;
        }

        t_list_item* del_item = ptr_item;
        self->copy_item(ret_item, &del_item->data);

        if (del_item->prev != NULL)
            ((t_list_item*)del_item->prev)->next = del_item->next;

        if (del_item->next != NULL)
            ((t_list_item*)del_item->next)->prev = del_item->prev;

        if (del_item == self->ptr_head)
            self->ptr_head = del_item->next;
        if (del_item == self->ptr_tail)
            self->ptr_tail = del_item->prev;

        self->delete_item(&del_item->data);
        del_item->next = NULL;
        del_item->prev = NULL;
        free(del_item);

        return ret_item;
    }
    return NULL;
}
