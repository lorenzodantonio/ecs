#include "ecs/sparse_set.h"
#include <stdlib.h>

int sparse_set_init(struct sparse_set *set, uint32_t capacity) {
  const uint32_t page_count = (ENTITY_IDX_MASK >> PAGE_SHIFT) + 1;
  uint32_t **pages = calloc(page_count, sizeof(uint32_t *));
  if (!pages) {
    return -1;
  }

  entity *dense = malloc(sizeof(entity) * capacity);
  if (!dense) {
    free(pages);
    return -1;
  }

  set->pages = pages;
  set->dense = dense;
  set->page_count = page_count;

  set->count = 0;
  set->capacity = capacity;

  return 0;
}

void sparse_set_deinit(struct sparse_set *set) {
  for (uint32_t i = 0; i < set->page_count; i++) {
    free(set->pages[i]);
  }

  free(set->pages);
  free(set->dense);
}
