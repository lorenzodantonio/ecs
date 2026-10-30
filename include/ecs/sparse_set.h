#ifndef SPARSE_SET_H
#define SPARSE_SET_H

#include "entity.h"

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

// int sparse_set_push(struct sparse_set *set, entity e);
// int sparse_set_remove(struct sparse_set *set, entity e);
void sparse_set_deinit(struct sparse_set *set);

static inline uint32_t sparse_set_get_page(uint32_t index) {
  return index >> PAGE_SHIFT;
}

static inline uint32_t sparse_set_get_offset(uint32_t index) {
  return index & (PAGE_MASK);
}

static inline uint32_t sparse_set_find(struct sparse_set *s, entity e) {
  const uint32_t idx = entity_get_index(e);
  const uint32_t *page = s->pages[sparse_set_get_page(idx)];
  return page ? page[sparse_set_get_offset(idx)] : UINT32_MAX;
}

#endif
