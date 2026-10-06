#ifndef COMPONENT_POOL_H
#define COMPONENT_POOL_H

#include "sparse_set.h"
#include "entity.h"
#include <stdlib.h>

struct component_pool {
  size_t id;
  size_t component_size;
  struct sparse_set entities;
  void *data;
};

int component_pool_init(struct component_pool *pool, size_t id,
                         size_t component_size, size_t capacity);
void component_pool_deinit(struct component_pool *pool);

static inline void *component_pool_get_by_position(struct component_pool *pool,
                                                   uint32_t position) {
  return (char *)pool->data + position * pool->component_size;
}

static inline void *component_pool_get_by_entity(struct component_pool *pool,
                                                 entity e) {
  const uint32_t index = entity_get_index(e);
  const uint32_t *page = pool->entities.pages[sparse_set_get_page(index)];
  if (!page) {
    return NULL;
  }

  const uint32_t pos = page[sparse_set_get_offset(index)];
  if (pos == UINT32_MAX) {
    return NULL;
  }
  return component_pool_get_by_position(pool, pos);
}

void *component_pool_emplace(struct component_pool *pool, entity e);
int component_pool_remove(struct component_pool *pool, entity e);

#endif
