#ifndef DATABASE_TABLE_H
#define DATABASE_TABLE_H

#include "common.h"

// FIELD REPRESENTATION

#define USERNAME_MAX_LEN 32
#define EMAIL_MAX_LEN 256

typedef struct field_t {
  u32 id;
  char username[USERNAME_MAX_LEN];
  char email[EMAIL_MAX_LEN];
} field;

void field_print(field* row);

// TABLE REPERSENTATION

#define MAX_PAGES_COUNT 100
#define MAX_ROWS_PER_PAGE 15
#define PAGE_SIZE (15 * sizeof(field))

typedef struct table_t {
  field* pages;
  u8 page_row_count;
  u8 page_offset;
} table;

table* table_new();
field* table_get(table* tbl, u32 row_index);
void table_insert(table* tbl, field* row);
void table_dispose(table* tbl);

#endif