#ifndef BUF_OPERATIONS_H
#define BUF_OPERATIONS_H


int reverse_buf(char* buf);
int select_delete(char* buf, unsigned long __user arg);
int change_letter(char* buf, unsigned long __user arg);
int add_str_begin(char* buf, unsigned long __user arg);
int add_str_size(char* buf);


#endif /* BUF_OPERATIONS_H */

