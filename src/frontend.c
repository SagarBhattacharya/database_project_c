#include "../inc/frontend.h"

command_result execute_meta_command(input_buffer* buf, table* tbl){
  if(strcmp(buf->buffer, ".exit") == 0){
    input_buffer_close(buf);
    db_close(tbl);
    exit(EXIT_SUCCESS);
  }else {
    return CMD_UNRECOGNIZED;
  }
}

static txn_result insert_transaction(input_buffer* buf, statement* stm){
  stm->type = STMT_INSERT;

  char* keyword = strtok(buf->buffer, " ");
  char* id_string = strtok(NULL, " ");
  char* username = strtok(NULL, " ");
  char* email = strtok(NULL, " ");

  if(id_string == NULL || username == NULL || email == NULL){
    return TXN_SYNTAX_ERR;
  }

  int id = strtol(id_string, NULL, 10);
  if(id < 0){
    return TXN_NEGATIVE_ID;
  }

  if(strlen(username) > USERNAME_MAX_LEN){
    return TXN_STRING_TOO_LONG;
  }
  if(strlen(email) > EMAIL_MAX_LEN){
    return TXN_STRING_TOO_LONG;
  }

  stm->row.id = id;
  strcpy(stm->row.username, username);
  strcpy(stm->row.email, email);
  return TXN_SUCCESS;
}

txn_result transaction(input_buffer* buf, statement* stm){
  if(compare(buf->buffer, "insert")){
    return insert_transaction(buf, stm);
  }else if(compare(buf->buffer, "select")){
    stm->type = STMT_SELECT;
    return TXN_SUCCESS;
  }
  return TXN_UNRECOGNISED;
}

static execute_result execute_insert_transaction(statement* stmt, table* tbl){
  if(tbl->row_count >= MAX_PAGES_COUNT * MAX_ROWS_PER_PAGE){
    return EXEC_TABLEFULL;
  }
  field* ref = db_reference(tbl, tbl->row_count);
  *ref = stmt->row;
  tbl->row_count++;
  return EXEC_SUCCESS;
}

static execute_result execute_select_transaction(statement* stmt, table* tbl){
  for(u32 i = 0; i < tbl->row_count; i++){
    field* ref = db_reference(tbl, i);
    field_print(ref);
  }
  return EXEC_SUCCESS;
}

execute_result execute_transaction(statement* stm, table* tbl){
  switch(stm->type){
    case STMT_INSERT:
      return execute_insert_transaction(stm, tbl);
    case STMT_SELECT:
      return execute_select_transaction(stm, tbl);
  }
}