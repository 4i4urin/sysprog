#ifndef LIST_IO_CONSOLE_H
#define LIST_IO_CONSOLE_H

char get_char(void);
void show_employee(t_employee* ptr_item);

t_employee* get_user_employee(t_employee* emp);
void show_menu(void);
unsigned int get_uint_number(void);


#endif /* LIST_IO_CONSOLE_H */