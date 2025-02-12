#include "frontend.h"

i32 main(){
  input_buffer* buf = input_buffer_new();
  while(true){
    print_prompt();
    read_input(buf);

    if(buf->buffer[0] == '.'){
      switch( execute_meta_command(buf)) {
        case META_COMMAND_SUCCESS: continue;
        case META_COMMAND_UNRECOGNIZED_COMMAND:
          fprintf(stderr, "Unrecognized command %s \n", buf->buffer);
          continue;
      }
    }

    statement stm;
    switch( prepare_statement(buf, &stm)) {
      case PREPARE_SUCCESS:
        break;
      case PREPARE_UNRECOGNIZED_STATEMENT:
        fprintf(stderr, "Unrecognized keyword at start of %s \n", buf->buffer);
        continue;
    }

    execute_statement(&stm);
    printf("Executed\n");
  }
}

// *********** UTILITY FUNCTIONS *************

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


// *************** FRONTEND *********************

meta_command_result execute_meta_command(input_buffer* buf){
  if(strcmp(buf->buffer, ".exit") == 0){
    exit(EXIT_SUCCESS);
  }else {
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}

prepare_result prepare_statement(input_buffer* buf, statement* stm){
  char* commands[] = {"insert", "select"};
  for(int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++){
    if(strncmp(buf->buffer, commands[i], strlen(commands[i])) == 0){
      stm->type = (statement_type) i;
      return PREPARE_SUCCESS;
    }
  }
  return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(statement* stm){
  switch(stm->type){
    case STATEMENT_INSERT:
      printf("This is where we would handle an insert statement\n");
      break;
    case STATEMENT_SELECT:
      printf("This is where we would handle a select statement\n");
      break;
  }
}