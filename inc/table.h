#ifndef DATABASE_TABLE_H
#define DATABASE_TABLE_H

#include "common.h"

// FIELD REPRESENTATION

typedef struct field_t {
  u32 id;
  char username[32];
  char email[256];
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