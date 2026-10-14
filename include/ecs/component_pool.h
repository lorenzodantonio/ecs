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

static inline uint32_t component_pool_find(struct component_pool *pool, entity e) {
  return sparse_set_find(&pool->entities, e);
}

static inline void *component_pool_get_by_entity(struct component_pool *pool,
                                                 entity e) {
  const uint32_t pos = sparse_set_find(&pool->entities, e);
  if (pos == UINT32_MAX) {
    return NULL;
  }
  return component_pool_get_by_position(pool, pos);
}

void *component_pool_emplace(struct component_pool *pool, entity e);
int component_pool_remove(struct component_pool *pool, entity e);

#endif
