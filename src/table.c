#include "../inc/table.h"
#include "io.h"
#include "fcntl.h"

void field_print(field* row){
  printf("[%04d] %s : %s\n", row->id, row->username, row->email);
}

pager* pager_open(const char* filename){
  FILE* file = fopen(filename, "rb+");
  if(file == NULL){
    file = fopen(filename, "wb+");
    if(file == NULL){
      printf("[ERROR] Unable to open file\n");
      exit(EXIT_FAILURE);
    }
  }

  fseek(file, 0, SEEK_END);
  long pos = ftell(file);
  fseek(file, 0, SEEK_SET);

  pager* pgr = (pager*) calloc(1, sizeof(pager));
  pgr->file = file;
  pgr->file_length = pos;
  printf("File reached here\n");
  for(u32 i = 0; i < MAX_PAGES_COUNT; i++){
    pgr->pages[i] = NULL;
  }

  return pgr;
}

field* get_page(pager* pgr, u32 page_index){
  if(page_index >= MAX_PAGES_COUNT){
    printf("[ERROR] Page Index Out of Bound, requested %d out of max %d\n", page_index, MAX_PAGES_COUNT);
    exit(EXIT_FAILURE);
  }

  if(pgr->pages[page_index] == NULL){
    field* page = malloc(PAGE_SIZE);
    u32 page_count = pgr->file_length / PAGE_SIZE;

    if(pgr->file_length % PAGE_SIZE){
      page_count++;
    }

    if(page_index < page_count){
      fseek(pgr->file, page_index * PAGE_SIZE, SEEK_SET);
      size_t bytes_read = fread(page, sizeof(field), MAX_ROWS_PER_PAGE, pgr->file);
      if(bytes_read == 0 && ferror(pgr->file)){
        printf("[ERROR] Could not read file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
      }
    } else {
      // Initialize the page with zeros if it is beyond the current file length
      memset(page, 0, PAGE_SIZE);
    }

    pgr->pages[page_index] = page;
  }
  return pgr->pages[page_index];
}

void pager_flush(pager* pgr, u32 page_index, u32 size){
  if(pgr->pages[page_index] == NULL){
    printf("[ERROR] Tried to flush NULL page\n");
    exit(EXIT_FAILURE);
  }

  i32 offset = fseek(pgr->file, page_index * PAGE_SIZE, SEEK_SET);
  if(offset == -1){
    printf("[ERROR] Could not seek to desired position\n");
    exit(EXIT_FAILURE);
  }

  isize bytes_written = fwrite(pgr->pages[page_index], size, 1, pgr->file);
  if(bytes_written != 1){
    printf("[ERROR] Could not write to file: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
}

table* db_open(const char* filename){
  table* t = (table*) malloc(sizeof(table));
  t->pgr = pager_open(filename);
  t->row_count = t->pgr->file_length / sizeof(field);
  return t;
}

field* db_reference(table* tbl, u32 row_index){
  u32 page_index = row_index / MAX_ROWS_PER_PAGE;
  field* page = get_page(tbl->pgr, page_index);
  u32 row_offset = row_index % MAX_ROWS_PER_PAGE;
  return page + row_offset; // POSSIBLE ERROR
}

void db_close(table* tbl){
  pager* pgr = tbl->pgr;
  u32 final_page_count = tbl->row_count / MAX_ROWS_PER_PAGE;

  for(u32 i = 0; i < final_page_count; i++){
    if(pgr->pages[i] == NULL){
      continue;
    }
    pager_flush(pgr, i, PAGE_SIZE);
    free(pgr->pages[i]);
    pgr->pages[i] = NULL;
  }

  u32 additional_rows = tbl->row_count % MAX_ROWS_PER_PAGE;
  if(additional_rows > 0){
    u32 page_index = final_page_count;
    if(pgr->pages[page_index] != NULL){
      pager_flush(pgr, page_index, additional_rows * sizeof(field));
      free(pgr->pages[page_index]);
      pgr->pages[page_index] = NULL;
    }
  }

  i32 result = fclose(pgr->file);
  if(result == -1){
    printf("[ERROR] Could not close file\n");
    exit(EXIT_FAILURE);
  }
  for(u32 i = 0; i < MAX_PAGES_COUNT; i++){
    field* page = pgr->pages[i];
    if(page){
      free(page);
      pgr->pages[i] = NULL;
    }
  }
  free(pgr);
  free(tbl);
}