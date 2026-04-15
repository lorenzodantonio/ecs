#pragma once

#include "datastructure.h"
#include "entity.h"
#include <stdint.h>

struct component_pool {
  size_t id;
  size_t capacity;
  size_t component_size;
  struct sparse_set entities;
  void *data;
};

int component_pool_remove(struct component_pool *pool, entity entity);

static inline void *component_pool_get_by_position(struct component_pool *pool,
                                                   uint32_t position) {
  return (char *)pool->data + position * pool->component_size;
}

static inline void *component_pool_get_by_entity(struct component_pool *pool,
                                                 entity e) {
  uint32_t pos = pool->entities.sparse[entity_get_index(e)];
  return component_pool_get_by_position(pool, pos);
}

static inline void *component_pool_emplace(struct component_pool *pool,
                                           entity e) {
  int res = sparse_set_push(&pool->entities, e);
  if (res == -1) {
    fprintf(stderr, "error mapping entity to sparse set\n");
    return NULL;
  }

  return component_pool_get_by_position(pool, pool->entities.count - 1);
}
