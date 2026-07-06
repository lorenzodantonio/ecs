#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include "entity.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SHIFT 12
#define PAGE_SIZE (1U << PAGE_SHIFT)
#define PAGE_MASK (PAGE_SIZE - 1)

struct sparse_set {
  uint32_t **pages;
  entity *dense;
  uint32_t page_count;
  uint32_t count;
  uint32_t capacity;
};

int sparse_set_init(struct sparse_set *set, uint32_t capacity);
struct sparse_set *sparse_set_new(uint32_t capacity);
int sparse_set_push(struct sparse_set *set, entity e);
int sparse_set_remove(struct sparse_set *set, entity e);
void sparse_set_deinit(struct sparse_set *set);
void sparse_set_free(struct sparse_set *set);

static inline uint32_t sparse_set_get_page(uint32_t index) {
  return index >> PAGE_SHIFT;
}

static inline uint32_t sparse_set_get_offset(uint32_t index) {
  return index & (PAGE_MASK);
}

static inline uint32_t *sparse_set_allocate_page_nocheck(struct sparse_set *set,
                                                         uint32_t page_num) {
  set->pages[page_num] = malloc(sizeof(uint32_t) * PAGE_SIZE);
  assert(set->pages[page_num]);
  memset(set->pages[page_num], UINT32_MAX, PAGE_SIZE * sizeof(uint32_t));
  return set->pages[page_num];
}

static inline void sparse_set_dense_realloc_nocheck(struct sparse_set *set) {
  const uint32_t new_cap = set->capacity * 2;
  entity *new_dense = realloc(set->dense, sizeof(entity) * new_cap);
  assert(new_dense);
  set->dense = new_dense;
  set->capacity = new_cap;
}

static inline void sparse_set_map_nocheck(struct sparse_set *set, uint32_t page,
                                          uint32_t offset, uint32_t dense_pos,
                                          entity entity) {
  set->pages[page][offset] = dense_pos;
  set->dense[dense_pos] = entity;
}

static inline void sparse_set_push_nocheck(struct sparse_set *set,
                                           uint32_t page, uint32_t offset,
                                           uint32_t entity) {
  sparse_set_map_nocheck(set, page, offset, set->count++, entity);
}

#endif
