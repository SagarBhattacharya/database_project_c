#ifndef DATABASE_FRONTEND_H
#define DATABASE_FRONTEND_H

#include "utils.h"

// META COMMAND REPRESENTATION

typedef enum command_result_t {
  CMD_SUCCESS,
  CMD_UNRECOGNIZED,
} command_result;

command_result execute_meta_command(input_buffer* buf, table* tbl);

// STATEMENT REPRESENTATION

typedef enum execute_result_t {
  EXEC_SUCCESS,
  EXEC_TABLEFULL,
} execute_result;

typedef enum txn_result_t {
  TXN_SUCCESS,
  TXN_UNRECOGNISED,
  TXN_SYNTAX_ERR,
  TXN_STRING_TOO_LONG,
  TXN_NEGATIVE_ID,
} txn_result;

typedef enum statement_type_t {
  STMT_INSERT,
  STMT_SELECT,
} statement_type;

typedef struct statement_t {
  statement_type type;
  field row; // specific to insert statement
} statement;

txn_result transaction(input_buffer* buf, statement* stm);
execute_result execute_transaction(statement* stm, table* tbl);

#endif