#ifndef SPARSE_SET_INTERNAL_H
#define SPARSE_SET_INTERNAL_H

#include "ecs/sparse_set.h"
#include <stdlib.h>
#include <string.h>

static inline uint32_t *sparse_set_allocate_page_nocheck(struct sparse_set *set,
                                                         uint32_t page_num) {
  set->pages[page_num] = malloc(sizeof(uint32_t) * PAGE_SIZE);
  if (set->pages[page_num] == NULL) {
    return NULL;
  }
  memset(set->pages[page_num], UINT32_MAX, PAGE_SIZE * sizeof(uint32_t));
  return set->pages[page_num];
}

static inline int sparse_set_dense_realloc_nocheck(struct sparse_set *set) {
  const uint32_t new_cap = set->capacity * 2;
  entity *resized = realloc(set->dense, sizeof(entity) * new_cap);
  if (resized == NULL) {
    return -1;
  }

  set->dense = resized;
  set->capacity = new_cap;

  return 0;
}

static inline void sparse_set_map_nocheck(struct sparse_set *set, entity e, uint32_t position) {
  const uint32_t idx = entity_get_index(e);
  set->pages[sparse_set_get_page(idx)][sparse_set_get_offset(idx)] = position;
  set->dense[position] = e;
}

static inline void sparse_set_push_nocheck(struct sparse_set *set, entity e) {
  sparse_set_map_nocheck(set, e, set->count++);
}

#endif
