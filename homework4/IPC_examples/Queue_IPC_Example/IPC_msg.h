#ifndef __IPC_MSG_H
#define __IPC_MSG_H

#define PATH "/IPC_Queue"
#define MAX_STRING_SIZE 1024

typedef struct string_struct
{
        char string[MAX_STRING_SIZE];
        unsigned int string_length;
}string_struct;

typedef struct msg_buffer
{
        string_struct string_message;
        bool led_state;
}msg_buffer;

#define STRUCT_SIZE sizeof(msg_buffer)


#endif 
