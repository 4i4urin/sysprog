#ifndef STACK_H
#define STACK_H


typedef struct _t_employee
{
	char* name;
	char is_married;
	double salary;
} t_employee;


typedef struct _t_stack_item
{
	void* last;
	t_employee data;	
} t_stack_item;


typedef void (*const t_push_func)(const t_employee* const ptr_item, const void* const ptrv_self);
typedef t_employee* (*const t_pop_func)(t_employee* const ret_item, const void* const ptrv_self);
typedef int (*const t_size_func)(const void* const ptrv_self);

typedef void (*const t_copy_item)(void* dest, const void* const stc);
typedef void (*const t_delete_item)(void* item);

typedef struct _t_module_stack
{
	t_push_func push;
	t_pop_func pop;
	t_size_func size;
	t_copy_item copy_item;
	t_delete_item delete_item;

	t_stack_item* ptr_head;
} t_module_stack;


t_module_stack init_stack(t_copy_item copy, t_delete_item delete);


#endif /* STACK_H */
