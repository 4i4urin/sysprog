#ifndef LINKED_LIST_H
#define LINKED_LIST_H


typedef enum _te_list_direction
{
    E_HEAD,
    E_TAIL
} te_list_direction;


typedef struct _t_employee
{
    unsigned int id;
    unsigned int age;
    char* name;
    char is_married;
    double salary;
    char gender;
} t_employee;


typedef struct _t_list_item
{
    t_employee data;
    void* next;
    void* prev;
} t_list_item;


typedef void (*const t_push_func)(const t_employee* const ptr_item, te_list_direction e_dir, const void* const ptrv_self);
typedef t_employee* (*const t_pop_func)(t_employee* const ret_item, te_list_direction e_dir, const void* const ptrv_self);
typedef int (*const t_size_func)(const void* const ptrv_self);
typedef t_employee* (*const t_take_func)(t_employee* const ret_item, const unsigned int id, const void* const ptrv_self);

typedef void (*const t_copy_item)(void* dest, const void* const stc);
typedef void (*const t_delete_item)(void* item);


typedef struct _t_module_list
{
    t_push_func push;
    t_pop_func pop;
    t_size_func size;

    t_copy_item copy_item;
    t_delete_item delete_item;

    t_list_item* ptr_head;
    t_list_item* ptr_tail;

    t_take_func take;
    unsigned int id;
} t_module_list;


t_module_list init_list(t_copy_item copy, t_delete_item delete);


#endif /* LINKED_LIST_H */
