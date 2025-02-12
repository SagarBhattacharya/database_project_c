#ifndef DATABASE_UTILS_H
#define DATABASE_UTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

typedef size_t usize;
typedef int32_t i32;

typedef struct input_buffer_t {
  char* buffer;
  usize buffer_length;
  usize input_length;
} input_buffer;

input_buffer* input_buffer_new();
void input_buffer_close(input_buffer* buf);

void print_prompt();
void read_input(input_buffer* buf);


#endif