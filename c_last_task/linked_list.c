#include "linked_list.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>


#define CREATE_TAIL_LINKS(new, tail) new->prev = tail; tail = new; new->next = NULL;
#define CREATE_HEAD_LINKS(new, head) new->next = head; head = new; new->prev = NULL;


static void push(const t_employee* const ptr_item, te_list_direction e_dir, const void* const ptrv_self);
static t_employee* pop(t_employee* const ret_item, te_list_direction e_dir, const void* const ptrv_self);
static int size(const void* const ptrv_self);
static t_employee* take(t_employee* const ret_item, const unsigned int id, const void* const ptrv_self);


t_module_list init_list(t_copy_item copy, t_delete_item delete)
{
    return (t_module_list){
        .push  = &push,
        .pop   = &pop,
        .size   = &size,

        .copy_item = copy,
        .delete_item = delete,

        .ptr_head = NULL,
        .ptr_tail = NULL,

        .take = take,
        .id = 0
    };  
}


static void push(const t_employee* const ptr_item, te_list_direction e_dir, const void* const ptrv_self)
{
    t_module_list* self = (t_module_list*) ptrv_self;

    t_list_item* new_item = (t_list_item*) malloc(sizeof(t_list_item));
    if (new_item == NULL)
        exit(100);

    self->copy_item(&new_item->data, ptr_item);
    new_item->data.id = (++(self->id));

    // when add the first elemenr
    if (self->ptr_tail == NULL && self->ptr_head == NULL)
    {
        self->ptr_tail = self->ptr_head = new_item;
        new_item->prev = new_item->next = NULL;
        return;
    }

    if (e_dir == E_HEAD)
    {
        self->ptr_head->prev = new_item;
        CREATE_HEAD_LINKS(new_item, self->ptr_head)
    } else if (e_dir == E_TAIL)
    {
        self->ptr_tail->next = new_item;
        CREATE_TAIL_LINKS(new_item, self->ptr_tail)
    }
}


static t_employee* pop(t_employee* const ret_item, te_list_direction e_dir, const void* const ptrv_self)
{
    t_module_list* self = (t_module_list*) ptrv_self;

    t_list_item* pop_item = (e_dir == E_TAIL) ? self->ptr_tail : self->ptr_head;
    if (pop_item == NULL)
        return NULL;

    self->copy_item(ret_item, &pop_item->data);

    // pop last elemnt
    if (self->ptr_tail == self->ptr_head)
    {
        self->ptr_head = NULL;
        self->ptr_tail = NULL;
    }

    if (e_dir == E_TAIL)
    {
        self->ptr_tail = (t_list_item*)pop_item->prev;
        if (self->ptr_tail != NULL)
            self->ptr_tail->next = NULL;
    } else if (e_dir == E_HEAD)
    {
        self->ptr_head = (t_list_item*)pop_item->next;
        if (self->ptr_head != NULL)
            self->ptr_head->prev = NULL;
    }

    self->delete_item(&pop_item->data);
    free(pop_item);
    
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
        free(del_item);

        return ret_item;
    }
    return NULL;
}
