#pragma once

#include "datastructure.h"
#include "entity.h"
#include <stdint.h>
#include <stdio.h>

struct component_pool {
  size_t id;
  size_t component_size;
  struct sparse_set entities;
  void *data;
};

void component_pool_init(struct component_pool *pool, size_t id,
                         size_t component_size, size_t capacity);
void component_pool_free(struct component_pool *pool);

static inline void *component_pool_get_by_position(struct component_pool *pool,
                                                   uint32_t position) {
  return (char *)pool->data + position * pool->component_size;
}

static inline void *component_pool_get_by_entity(struct component_pool *pool,
                                                 entity e) {
  uint32_t index = entity_get_index(e);
  uint32_t pos =
      pool->entities
          .pages[sparse_set_get_page(index)][sparse_set_get_offset(index)];
  if (pos == UINT32_MAX) {
    return NULL;
  }
  return component_pool_get_by_position(pool, pos);
}

void *component_pool_emplace(struct component_pool *pool, entity e);
int component_pool_remove(struct component_pool *pool, entity entity);
