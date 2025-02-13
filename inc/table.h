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
#define PAGE_SIZE (MAX_ROWS_PER_PAGE * sizeof(field))

typedef struct pager_t {
  FILE* file;
  u32 file_length;
  field* pages[MAX_PAGES_COUNT];
} pager;

pager* pager_open(const char* filename);

typedef struct table_t {
  pager* pgr;
  u8 row_count;
} table;

table* db_open(const char* filename);
field* db_reference(table* tbl, u32 row_index);
void db_close(table* tbl);

#endif