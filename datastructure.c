#include "datastructure.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

int sparse_set_init(struct sparse_set *set, uint32_t capacity) {
  uint32_t page_count = (ENTITY_IDX_MASK >> PAGE_SHIFT) + 1;

  set->pages = calloc(page_count, sizeof(uint32_t *));
  set->dense = malloc(sizeof(entity) * capacity);

  set->page_count = page_count;

  set->count = 0;
  set->capacity = capacity;

  return 0;
}

struct sparse_set *sparse_set_new(uint32_t capacity) {
  struct sparse_set *set = (struct sparse_set *)malloc(sizeof(*set));
  sparse_set_init(set, capacity);
  return set;
}

int sparse_set_push(struct sparse_set *set, entity e) {
  if (set->count >= set->capacity) {
    sparse_set_dense_realloc_nocheck(set);
  }

  uint32_t idx = entity_get_index(e);
  uint32_t page_num = sparse_set_get_page(idx);
  uint32_t offset = sparse_set_get_offset(idx);

  // handle pages
  if (!set->pages[page_num]) {
    sparse_set_allocate_page_nocheck(set, page_num);
    // handle allocation failure
  } else {
    if (set->pages[page_num][offset] != UINT32_MAX) {
      // already exists
      return -1;
    }
  }

  set->pages[page_num][offset] = set->count;
  set->dense[set->count++] = e;

  return 0;
}

int sparse_set_remove(struct sparse_set *set, entity e) {
  uint32_t idx = entity_get_index(e);

  uint32_t page_num = sparse_set_get_page(idx);
  if (set->pages[page_num] == NULL) {
    return -1;
  }

  uint32_t offset = sparse_set_get_offset(idx);

  uint32_t position = set->pages[page_num][offset];
  if (position == UINT32_MAX) {
    return -1;
  }

  set->pages[page_num][offset] = UINT32_MAX;
  if (position == --set->count) {
    return 0;
  }

  entity last_entity = set->dense[set->count];
  uint32_t last_idx = entity_get_index(last_entity);
  set->pages[sparse_set_get_page(last_idx)][sparse_set_get_offset(last_idx)] =
      position;
  set->dense[position] = last_entity;

  return 0;
}

void sparse_set_free(struct sparse_set *set) {
  for (uint32_t i = 0; i < set->page_count; i++) {
    free(set->pages[i]);
  }

  free(set->pages);
  free(set->dense);
}
