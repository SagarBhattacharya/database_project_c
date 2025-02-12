#include "utils.h"

i32 main(){
  input_buffer* buf = input_buffer_new();
  while(true){
    print_prompt();
    read_input(buf);

    if(strcmp(buf->buffer, ".exit") == 0){
      input_buffer_close(buf);
      exit(EXIT_SUCCESS);
    }else{
      fprintf(stderr, "Unrecognized command '%s' \n", buf->buffer);
    }
  }
}

// creates a new input_buffer object
input_buffer* input_buffer_new(){
  input_buffer* buf = (input_buffer*) malloc(sizeof(input_buffer));
  buf->buffer = (char*) calloc(256, sizeof(char));
  buf->buffer_length = 256;
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

// reads a line from stdin and updates input_buffer accordingly
// exits if any error
void read_input(input_buffer* buf){
  char* bytes_read = fgets(buf->buffer, buf->buffer_length, stdin);
  if(bytes_read == NULL){
    fprintf(stderr, "Error reading input \n");
    input_buffer_close(buf);
    exit(EXIT_FAILURE);
  }
  buf->input_length = strlen(bytes_read) - 1;
  buf->buffer[buf->input_length] = '\0';
}