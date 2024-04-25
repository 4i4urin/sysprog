#include "buf_operations.h"

#include <linux/module.h>
#include <linux/uaccess.h>


#include "ioctl.h"


#define SWAP(a, b) {a = a^b; b = a^b; a = a^b;}


int str_moveback(char* buf, int start_pos, int shift_val);
int str_moveforward(char* buf, int start_pos, int shift_val);
int find_str(char* buf, char* str);
int str_compare(char* src, char* find);
int int_to_str(char* str, int num);


// DONE
int reverse_buf(char* buf)
{
    for (int i = 0, max = strlen(buf) - 1; i < max; i++, max--)
        SWAP(buf[i], buf[max]);

    return 0;
}

// DONE
int delete(char* buf, unsigned long __user arg)
{
    t_seldel seldel_val;
    if ( copy_from_user(&seldel_val, (void*)arg, sizeof(seldel_val)) )
    {
        printk(KERN_WARNING "Can't copy from user select_delete");
        return 0;
    }
    printk(KERN_INFO "String to delete: %s len = %ld\n", seldel_val, strlen(seldel_val));

    int pos = 0;
    while (  (pos = find_str(buf, seldel_val)) >= 0  )
    {
        if ( str_moveforward(buf, pos, strlen(seldel_val)) < 0 )
        {
            printk(KERN_INFO "ERROR: Can't move string\n");
            break;
        }
    }

    return 0;
}

// DONE
int replace(char* buf, unsigned long __user arg)
{
    t_replet replet_val;
    if ( copy_from_user(&replet_val, (void*)arg, sizeof(replet_val)) )
    {
        printk(KERN_WARNING "Can't copy from user select_delete");
        return 0;
    }
    printk(KERN_INFO "letter: %c replace: %c\n", replet_val.find, replet_val.insert);

    for (int i = 0; i < strlen(buf); i++)
    {
        if (buf[i] == replet_val.find)
            buf[i] = replet_val.insert;
    }
    return 0;
}

// DONE
int add_str_begin(char* buf, unsigned long __user arg)
{
    t_addstr addstr_val;
    if ( copy_from_user(&addstr_val, (void*)arg, sizeof(addstr_val)) )
    {
        printk(KERN_WARNING "Can't copy from user select_delete\n");
        return 0;
    }
    printk(KERN_INFO "String to add: %s\n", addstr_val);
    
    if ( str_moveback(buf, 0, strlen(addstr_val)) < 0 )
        printk(KERN_INFO "ERROR: Can't add string\n");

    for (int i = 0; i < strlen(addstr_val); i++)
        buf[i] = addstr_val[i];

    return 0;
}


int add_str_size(char* buf)
{
    int pos = 0;
    int last_pos = 0;
    int num_size = 0;

    while (  (pos = find_str(buf + last_pos, "\n")) >= 0 )
    {
        char number[4] = { 0 };
        printk(KERN_INFO "pos = %d\n", pos);
        num_size = int_to_str(number, pos);
        pos += last_pos;

        if ( str_moveback(buf, pos-1, num_size + 1) < 0 )
        {
            printk(KERN_INFO "ERROR: Can't move string\n");
            break;
        }

        buf[pos] = ' ';
        for (int i = pos + 1, j = 0; j < num_size; i++, j++)
            buf[i] = number[j];

        // pos + num_size + ' ' + '\n'
        last_pos = pos + num_size + 1 + 1;
        printk(KERN_INFO "buf: %s\n", buf);
        printk(KERN_INFO "end pos = %d\n", last_pos);
    }

    return 0;
}


int str_moveback(char* buf, int start_pos, int shift_val)
{
    if (strlen(buf) + shift_val + start_pos + 1 >= BUF_LEN)
        return -1;

    for (int last = strlen(buf); last >= start_pos; last--)
        buf[last + shift_val] = buf[last];

    return 0;
}


int str_moveforward(char* buf, int start_pos, int shift_val)
{
    if (strlen(buf) + shift_val + 1  > BUF_LEN)
        return -1;

    for (int i = start_pos; i < strlen(buf); i++)
        buf[i] = buf[i + shift_val];

    return 0;
}


int find_str(char* buf, char* str)
{
    if (strlen(str) > strlen(buf))
        return -1;

    for (int i = 0; i < strlen(buf); i++)
    {
        if ( str_compare((buf + i), str) )
            return i;
    }

    return -1;
}


int str_compare(char* src, char* find)
{
    for (int i = 0; i < strlen(src) && src[i] && find[i]; i++)
        if (src[i] != find[i])
            return 0;
    return 1;
}


int int_to_str(char* str, int num)
{
    if ( num >= BUF_LEN )
        return -1;

    int i;
    for (i = 0; num > 0 ; num /= 10, i++)
        str[i] = (num % 10) + '0';

    reverse_buf(str);
    return i;
}

MODULE_LICENSE("GPL");

