#ifndef DATABASE_FRONTEND_H
#define DATABASE_FRONTEND_H

#include "utils.h"

typedef enum meta_command_result_t {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND,
} meta_command_result;

meta_command_result execute_meta_command(input_buffer* buf);

typedef enum prepare_result_t {
  PREPARE_SUCCESS,
  PREPARE_UNRECOGNIZED_STATEMENT,
} prepare_result;

typedef enum statement_type_t {
  STATEMENT_INSERT,
  STATEMENT_SELECT,
} statement_type;

typedef struct statement_t {
  statement_type type;
} statement;

prepare_result prepare_statement(input_buffer* buf, statement* stm);
void execute_statement(statement* stm);

#endif