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


typedef void (*t_push_func)(const t_employee* const ptr_item, const void* const ptrv_self);
typedef t_employee* (*t_pop_func)(t_employee* const ret_item, const void* const ptrv_self);
typedef void (*t_clear_func)(const void* const ptrv_self);
typedef int (*t_size_func)(const void* const ptrv_self);


typedef struct _t_module_stack
{
	const t_push_func push;
	const t_pop_func pop;
	const t_clear_func clear;
	const t_size_func size;
	t_stack_item* ptr_head;
} t_module_stack;


t_module_stack init_stack(void);


#endif /* STACK_H */
