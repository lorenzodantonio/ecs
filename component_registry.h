#pragma once

#include "datastructure.h"
#include "entity.h"
#include <stdint.h>

#define MAX_COMPONENTS 128

struct component_pool {
  size_t id;
  size_t capacity;
  size_t component_size;
  struct sparse_set entities;
  void *data;
};

int component_pool_remove(struct component_pool *pool, entity entity);

struct component_registry {
  size_t count;
  struct component_pool pools[MAX_COMPONENTS];
};

void component_registry_init(struct component_registry *registry);

size_t component_registry_add(struct component_registry *registry,
                              size_t component_size, size_t capacity);

int component_registry_purge_entity(struct component_registry *registry,
                                    entity e);

static inline struct component_pool *
component_registry_get(struct component_registry *registry,
                       size_t component_id) {
  return &registry->pools[component_id];
}

static inline void *component_pool_get_by_position(struct component_pool *pool,
                                                   uint32_t position) {
  return (char *)pool->data + position * pool->component_size;
}

static inline char *component_pool_get_by_entity(struct component_pool *pool,
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

struct iterator {
  size_t cursor;
  entity entity;
  size_t component_count;
  struct component_pool *leader;
  struct component_pool *followers[MAX_COMPONENTS - 1];
  void *data[MAX_COMPONENTS];
};

void iterator_init(struct iterator *iter, size_t component_count,
                   struct component_pool **pools);
int iterator_next(struct iterator *iter);
