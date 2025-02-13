#include "../inc/utils.h"

// creates a new input_buffer object
input_buffer* input_buffer_new(){
  input_buffer* buf = (input_buffer*) malloc(sizeof(input_buffer));
  buf->buffer = (char*) calloc(512, sizeof(char));
  buf->buffer_length = 512;
  buf->input_length = 0;
  return buf;
}

// frees the memory for input_buffer object
void input_buffer_close(input_buffer* buf){
  if(!buf->buffer) free(buf->buffer);
  if(!buf) free(buf);
}

// prints a prompt to stdout
void print_prompt(){
  printf("db > ");
}

// Compares two strings only upto size of slave
bool compare(char* master, char* slave){
  return strncmp(master, slave, strlen(slave)) == 0;
}

void read_input(input_buffer* buf, table* tbl){
  char* bytes_read = fgets(buf->buffer, buf->buffer_length, stdin);
  if(bytes_read == NULL){
    fprintf(stderr, "[ERROR] reading input failed \n");
    db_close(tbl);
    input_buffer_close(buf);
    exit(EXIT_FAILURE);
  }
  buf->input_length = strlen(bytes_read) - 1;
  buf->buffer[buf->input_length] = '\0';
}