#ifndef DATABASE_UTILS_H
#define DATABASE_UTILS_H

#include "common.h"
#include "table.h"

typedef struct input_buffer_t {
  char* buffer;
  usize buffer_length;
  usize input_length;
} input_buffer;

input_buffer* input_buffer_new();
void input_buffer_close(input_buffer* buf);
void print_prompt();

// This function can possibly fail and exit - cleans up  
// memory of input_buffer and table before exit
void read_input(input_buffer* buf, table* tbl);

bool compare(char* master, char* slave);

#endif