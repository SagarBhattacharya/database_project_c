#include "../inc/frontend.h"

i32 main(){
  table* tbl = table_new();
  input_buffer* buf = input_buffer_new();

  while(true){
    print_prompt();
    read_input(buf, tbl);

    if(buf->buffer[0] == '.'){
      switch(execute_meta_command(buf, tbl)) {
        case CMD_SUCCESS: continue;
        case CMD_UNRECOGNIZED:
          fprintf(stderr, "[ERROR] Unrecognized command %s \n", buf->buffer);
          continue;
      }
    }

    statement stm;
    switch( transaction(buf, &stm)) {
      case TXN_SUCCESS: break;
      case TXN_STRING_TOO_LONG:
        printf("[ERROR] String is too long\n");
        continue;
      case TXN_NEGATIVE_ID:
        printf("[ERROR] Id must be a positive value\n");
        continue;
      case TXN_SYNTAX_ERR:
        fprintf(stderr, "[ERROR] Syntax Error, could not parse statement\n");
        continue;
      case TXN_UNRECOGNISED:
        fprintf(stderr, "[ERROR] Unrecognized keyword at start of %s \n", buf->buffer);
        continue;
    }

    switch(execute_transaction(&stm, tbl)){
      case EXEC_SUCCESS:
        printf("[INFO] Executed\n");
        break;
      case EXEC_TABLEFULL:
        printf("[ERROR] Table Full\n");
        break;
    }
  }
}