#include "../inc/table.h"

void field_print(field* row){
  printf("[%04d] %s : %s\n", row->id, row->username, row->email);
}

table* table_new(){
  table* t = (table*) malloc(sizeof(table));
  t->page_row_count = 0;
  t->page_offset = 0;
  t->pages = (field*) calloc(MAX_ROWS_PER_PAGE, sizeof(field));
  return t;
}

void table_insert(table* tbl, field* row){
  if(tbl->page_row_count + 1 > MAX_ROWS_PER_PAGE){
    u32 size = (tbl->page_offset + 2) * MAX_ROWS_PER_PAGE * sizeof(field);
    tbl->pages = (field*) realloc(tbl->pages, size);
    tbl->page_offset++;
    tbl->page_row_count = 0;
  }
  tbl->pages[(tbl->page_offset * MAX_ROWS_PER_PAGE) + tbl->page_row_count] = *row;
  tbl->page_row_count++;
}

field* table_get(table* tbl, u32 row_index){
  if(row_index < (tbl->page_offset + 1) * MAX_ROWS_PER_PAGE){
    return &tbl->pages[row_index];
  }else{
    printf("[ERROR] Index out of bound : Table accessed at %d", row_index);
  }
}

void table_dispose(table* tbl){
  free(tbl->pages);
  free(tbl);
  // NOTE : This is where you might want to call free function of field if there is
  // any dynamically allocations inside field object
}